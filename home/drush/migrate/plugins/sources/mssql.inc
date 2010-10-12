<?php
// $Id: mssql.inc,v 1.1.4.5 2010/10/09 17:34:52 mikeryan Exp $

/**
 * @file
 * Define a MigrateSource for importing from Microsoft SQL Server databases. This
 * plugin has limited capabilities compared to MigrateSourceSQL which can join
 * with the map/message tables and only fetch un-migrated content. This plugin
 * fetched everything unless an idlist or itemlimit are supplied. Note that
 * this is not completely inefficient; already migrated content does not actually
 * get re-imported, it just gets re-fetched.
 */

/**
 * Implementation of MigrateSource, to handle imports from remote DB servers.
 */
class MigrateSourceMSSQL extends MigrateSource {
  /**
   * Array containing information for connecting to SQL Server:
   *  servername - Hostname of the SQL Server
   *  username - Username to connect as
   *  password - Password for logging in
   *  database (optional) - Database to select after connecting
   *
   * @var array
   */
  protected $configuration;

  /**
   * The active MS SQL Server connection for this source.
   *
   * @var resource
   */
  protected $connection;

  /**
   * The SQL query from which to obtain data. Is a string.
   */
  protected $query;

  /**
   * The result object from executing the query - traversed to process the
   * incoming data.
   */
  protected $result;

  /**
   * Number of eligible rows processed so far (used for itemlimit checking)
   *
   * @var int
   */
  protected $numProcessed = 0;

  /**
   * By default, mssql_query fetches all results - severe memory problems with
   * big tables. So, we will fetch a batch at a time.
   *
   * @var int
   */
  protected $batchSize;

  /**
   * Simple initialization.
   */
  public function __construct(array $configuration, $query, $count_query,
      array $fields, $batch_size = 500) {
    parent::__construct();
    $this->query = $query;
    $this->countQuery = $count_query;
    $this->configuration = $configuration;
    $this->fields = $fields;
    $this->batchSize = $batch_size;
  }

  /**
   * Return a string representing the source query.
   *
   * @return string
   */
  public function __toString() {
    return (string) $this->query;
  }

  /**
   * Connect lazily to the DB server.
   */
  protected function connect() {
    if (!isset($this->connection)) {
      if (isset($this->configuration['port'])) {
        $host = $this->configuration['servername'] . ':' . $this->configuration['port'];
      }
      else {
        $host = $this->configuration['servername'];
      }
      $this->connection = mssql_connect(
        $host,
        $this->configuration['username'],
        $this->configuration['password'],
        TRUE);
      if (isset($this->configuration['database'])) {
        return mssql_select_db($this->configuration['database'], $this->connection);
      }
    }
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    // The fields are defined in the Constructor for this plugin.
    return $this->fields;
  }

  /**
   * Return a count of all available source records.
   *
   * @param boolean $refresh
   *  If TRUE, or if there is no cached count, perform a SQL COUNT query to
   *  retrieve and cache the number of rows in the query. Otherwise, return
   *  the last cached value.
   */
  public function count($refresh = FALSE) {
    if ($refresh) {
      cache_clear_all(__METHOD__, 'cache');
    }

    $count = cache_get(__METHOD__, 'cache');
    if (!is_object($count)) {
      // Cache miss.
      if ($this->connect()) {
        $result = mssql_query($this->countQuery);
        $count = reset(mssql_fetch_object($result));
        $return = cache_set(__METHOD__, $count, 'cache');
      }
      else {
        // Do something else?
        return FALSE;
      }
    }
    else {
      $count = $count->data;
    }
    return $count;
  }

  /**
   * Implementation of Iterator::rewind() - called before beginning a foreach loop.
   */
  public function rewind() {
    $migration = Migration::currentMigration();
    $this->result = NULL;
    $this->currentRow = NULL;
    $this->numProcessed = 0;
    $map = $migration->getMap();

    $keys = array();
    foreach ($map->getSourceKey() as $field_name => $field_schema) {
      // Allow caller to provide an alias to table containing the primary key.
      // TODO: Alias should be determined automatically
      if (!empty($field_schema['alias'])) {
        $field_name = $field_schema['alias'] . '.' . $field_name;
      }
      $keys[] = $field_name;
    }

    /*
     * Replace :criteria placeholder with idlist or highwater clauses. We
     * considered supporting both but it is not worth the complexity. Run twice
     * instead.
     */
    $idlist = $migration->getOption('idlist');
    if (isset($idlist)) {
      // TODO: Sanitize. not critical as this is admin supplied data in drush.
      $this->query = str_replace(':criteria',
        $keys[0] . ' IN (' . $idlist . ')', $this->query);
    }
    else {
      $highwaterField = $migration->getHighwaterField();
      if (isset($highwaterField['name']) && $highwater = $migration->getHighwater()) {
        if (empty($highwaterField['alias'])) {
          $highwater_name = $highwaterField['name'];
        }
        else {
          $highwater_name = $highwaterField['alias'] . '.' . $highwaterField['name'];
        }
        $this->query = str_replace(':criteria', "$highwater_name > '$highwater'", $this->query);
      }
      else {
        // No idlist or highwater. Replace :criteria placeholder with harmless WHERE
        // clause instead of empty since we don't know if an AND follows.
        $this->query = str_replace(':criteria', '1=1', $this->query);
      }
    }

    migrate_instrument_start('mssql_query');
    $this->connect();
    $this->result = mssql_query($this->query, $this->connection, $this->batchSize);
    migrate_instrument_stop('mssql_query');

    $this->next();
  }

  /**
   * Implementation of Iterator::current() - called when entering a loop
   * iteration, returning the current row
   */
  public function current() {
    return $this->currentRow;
  }

  /**
   * Implementation of Iterator::key - called when entering a loop iteration, returning
   * the key of the current row. It must be a scalar - we will serialize
   * to fulfill the requirement, but using getCurrentKey() is preferable.
   */
  public function key() {
    return serialize($this->currentKey);
  }

  /**
   * Implementation of Iterator::next() - called at the bottom of the loop implicitly,
   * as well as explicitly from rewind().
   */
  public function next() {
    $migration = Migration::currentMigration();
    migrate_instrument_start('mssql_fetch_object');
    $this->currentRow = mssql_fetch_object($this->result);
    migrate_instrument_stop('mssql_fetch_object');
    // Might be totally out of data, or just out of this batch - request another
    // batch and see
    if (!is_object($this->currentRow)) {
      migrate_instrument_start('mssql_fetch_batch');
      mssql_fetch_batch($this->result);
      migrate_instrument_stop('mssql_fetch_batch');

      migrate_instrument_start('mssql_fetch_object');
      $this->currentRow = mssql_fetch_object($this->result);
      migrate_instrument_stop('mssql_fetch_object');
    }
    if (!is_object($this->currentRow)) {
      $this->currentRow = NULL;
    }

    if ($this->currentRow) {
      $map = $migration->getMap();
      $this->numProcessed++;
      $this->currentKey = array();
      foreach ($map->getSourceKey() as $field_name => $field_schema) {
        $this->currentKey[$field_name] = $this->currentRow->$field_name;
      }
    }
  }

  /**
   * Implementation of Iterator::valid() - called at the top of the loop, returning
   * TRUE to process the loop and FALSE to terminate it
   */
  public function valid() {
    return !is_null($this->currentRow);
  }
}
