<?php
// $Id: list.inc,v 1.1.4.3 2010/10/09 17:34:52 mikeryan Exp $

/**
 * @file
 * Support for migration from sources with distinct means of listing items to
 * import and obtaining the items themselves.
 *
 * TODO: multiple-field source keys
 */


/**
 * Extend the MigrateList class to provide a means to obtain a list of IDs to
 * be migrated from a given source (e.g., MigrateListXML extends MigrateList to
 * obtain a list of IDs from an XML document).
 */
abstract class MigrateList {
  public function __construct() {}

  /**
   * Implementors are expected to return a string representing where the listing
   * is obtained from (a URL, file directory, etc.)
   *
   * @return string
   */
  abstract public function __toString();

  /**
   * Implementors are expected to return an array of unique IDs, suitable for
   * passing to the MigrateItem class to retrieve the data for a single item.
   *
   * @return array
   */
  abstract public function getIdList();

  /**
   * Implementors are expected to return a count of IDs available to be migrated.
   *
   * @param boolean $refresh
   *
   * @return int
   */
  abstract public function count($refresh = FALSE);
}

/**
 * Extend the MigrateItem class to provide a means to obtain the data for a
 * given migratable item given its ID as provided by the MigrateList class.
 */
abstract class MigrateItem {
  public function __construct() {}

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  abstract public function getItem($id);
}

/**
 * Implementation of MigrateSource, providing the semantics of iterating over
 * IDs provided by a MigrateList and retrieving data from a MigrateItem.
 */
class MigrateSourceList extends MigrateSource {
  /**
   * MigrateList object used to obtain ID lists.
   *
   * @var MigrateList
   */
  protected $listClass;

  /**
   * MigrateItem object used to obtain the source object for a given ID.
   *
   * @var MigrateItem
   */
  protected $itemClass;

  /**
   * List of IDs from the listing class.
   *
   * @var array
   */
  protected $idList = array();
  public function getIdList() {
    return $this->idList;
  }

  /**
   * List of available source fields.
   *
   * @var array
   */
  protected $fields = array();

  /**
   * Keep the current source ID handy while iterating.
   * @var string
   */
  protected $id;

  /**
   * Simple initialization.
   */
  public function __construct(MigrateList $list_class, MigrateItem $item_class, $fields = array()) {
    parent::__construct();
    $this->listClass = $list_class;
    $this->itemClass = $item_class;
    $this->fields = $fields;
  }

  /**
   * Return a string representing the source.
   *
   * @return string
   */
  public function __toString() {
    return (string) $this->listClass;
  }

  /**
   * Returns a list of fields available to be mapped from the source query.
   * Since we can't reliably figure out what "fields" are in the source,
   * it's up to the implementing Migration constructor to fill them in.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    return $this->fields;
  }

  /**
   * It's the list class that knows how many records are available, so ask it.
   *
   * @param boolean $refresh
   *
   * @return int
   */
  public function count($refresh = FALSE) {
    return $this->listClass->count($refresh);
  }

  /**
   * Implementation of Iterator::rewind() - called before beginning a foreach loop.
   */
  public function rewind() {
    $migration = Migration::currentMigration();
    migrate_instrument_start('MigrateSourceList rewind');
    $this->numProcessed = 0;
    $idlist = $migration->getOption('idlist');
    if ($idlist) {
      $this->idList = explode(',', $idlist);
    }
    else {
      $this->idList = $this->listClass->getIdList();
    }
    migrate_instrument_stop('MigrateSourceList rewind');
    // Load up the first row
    $this->next();
  }

  /**
   * Implementation of Iterator::next() - called at the bottom of the loop implicitly,
   * as well as explicitly from rewind().
   */
  public function next() {
    $migration = Migration::currentMigration();
    migrate_instrument_start('MigrateSourceList next');
    $this->currentRow = NULL;
    $this->currentKey = NULL;

    // Enforce the itemlimit
    $itemlimit = $migration->getOption('itemlimit');

    // Get next item (next ID not already in the map, unless needs_update=1)
    while ($this->id = array_shift($this->idList)) {
      // Enforce the itemlimit
      if ($itemlimit && $this->numProcessed >= $itemlimit) {
        return;
      }
      // Check the map - if it's already mapped, and not marked for update, skip it
      $map_row = $migration->getMap()->getRowBySource(array($this->id));
      if ($map_row && $map_row['needs_update'] == 0) {
        continue;
      }
      // TODO: Also check message table (non-informational entries here indicate failed items, we don't
      // want to run through them again)
      $this->currentRow = $this->itemClass->getItem($this->id);
      $this->numProcessed++;
      if ($this->currentRow) {
        $this->currentKey = array($this->id);
        // Save the ID using the map source key - it will be used for mapping
        $sourceKey = $migration->getMap()->getSourceKey();
        $key_name = key($sourceKey);
        $this->currentRow->$key_name = $this->id;
        // Add map info to the row, if present
        if ($map_row) {
          foreach ($map_row as $field => $value) {
            $field = 'migrate_map_' . $field;
            $this->currentRow->$field = $value;
          }
        }
        // OK, got a valid one, break out and return
        break;
      }
      else {
        $this->currentKey = NULL;
      }
    }

    migrate_instrument_stop('MigrateSourceList next');
  }
}
