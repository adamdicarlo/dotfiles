<?php
// $Id: table_copy.inc,v 1.1.4.5 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * Copies data_row into a table using drupal_write_record()
 */

/**
 * Destination class implementing migration into a single table.
 */
class MigrateDestinationTableCopy extends MigrateDestination {
  public function __construct(array $key_schema, $tableName) {
    parent::__construct($key_schema);
    $this->tableName = $tableName;
  }

  public function __toString() {
    $output = t('Table copy');
    return $output;
  }

  /**
   * Delete a batch of rows at once.
   *
   * @param $ids
   *  Array of IDs to be deleted.
   */
  public function bulkRollback(array $ids) {
    migrate_instrument_start('table_copy bulkRollback');
    db_delete($this->tableName)
      ->condition(key($this->keySchema), $ids, 'IN')
      ->execute();
    migrate_instrument_stop('table_copy bulkRollback');
  }

  /**
   * Import a single row.
   *
   * @param $entity
   *  Object object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields of the object that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $entity, stdClass $row) {
    $fields = clone $row;
    $key = key($this->keySchema);
    $valkey = $row->$key;
    unset($fields->migrate_map_destid1);
    $query = db_merge($this->tableName)->key(array($key => $valkey))->fields((array)$fields);
    try {
      $query->execute();
      return array($valkey);
    }
    catch (MigrateException $e) {
      $this->saveMessage($e->getMessage(), $e->getLevel());
      $this->showMessage($e->getMessage());
    }
    catch (Exception $e) {
      $this->saveMessage($e->getMessage());
      $this->showMessage($e->getMessage());
    }
  }

  public function fields() {
    return array();
  }

}
