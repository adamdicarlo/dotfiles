<?php
// $Id: xml.inc,v 1.1.4.17 2010/10/09 17:34:52 mikeryan Exp $

/**
 * @file
 * Support for migration from XML sources.
 */

/**
 * Implementation of MigrateList, for retrieving a list of IDs to be migrated
 * from an XML document.
 */
class MigrateListXML extends MigrateList {
  /**
   * A URL pointing to an XML document containing a list of IDs to be processed.
   *
   * @var string
   */
  protected $listUrl;

  public function __construct($list_url) {
    parent::__construct();
    $this->listUrl = $list_url;
    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);
  }

  /**
   * Our public face is the URL we're getting items from
   *
   * @return string
   */
  public function __toString() {
    return $this->listUrl;
  }

  /**
   * Load the XML at the given URL, and return an array of the IDs found within it.
   *
   * @return array
   */
  public function getIdList() {
    migrate_instrument_start("Retrieve $this->listUrl");
    $xml = simplexml_load_file($this->listUrl);
    migrate_instrument_stop("Retrieve $this->listUrl");
    if ($xml) {
      return $this->getIDsFromXML($xml);
    }
    else {
      $migration = Migration::currentMigration();
      $migration->showMessage(t('Loading of !listurl failed:',
          array('!listurl' => $this->listUrl)));
      foreach (libxml_get_errors() as $error) {
        $migration->showMessage($error);
      }
      return NULL;
    }
  }

  /**
   * Given an XML object, parse out the IDs for processing and return them as an
   * array. The default implementation assumes the IDs are simply the values of
   * the top-level elements - in most cases, you will need to override this to
   * reflect your particular XML structure.
   *
   * @param SimpleXMLElement $xml
   *
   * @return array
   */
  protected function getIDsFromXML(SimpleXMLElement $xml) {
    $ids = array();
    foreach ($xml as $element) {
      $ids[] = (string)$element;
    }
    return $ids;
  }

  /**
   * Return a count of all available IDs from the source listing. The default
   * implementation assumes the count of top-level elements reflects the number
   * of IDs available - in many cases, you will need to override this to reflect
   * your particular XML structure.
   */
  public function count($refresh = FALSE) {
    $xml = simplexml_load_file($this->listUrl);
    // Number of sourceid elements beneath the top-level element
    $count = count($xml);
    return $count;
  }
}

/**
 * Implementation of MigrateItem, for retrieving a parsed XML document given
 * an ID provided by a MigrateList class.
 */
class MigrateItemXML extends MigrateItem {
  /**
   * A URL pointing to an XML document containing the data for one item to be
   * migrated.
   *
   * @var string
   */
  protected $itemUrl;

  public function __construct($item_url) {
    parent::__construct();
    $this->itemUrl = $item_url;
    // Suppress errors during parsing, so we can pick them up after
    libxml_use_internal_errors(TRUE);
  }

  /**
   * Implementors are expected to return an object representing a source item.
   *
   * @param mixed $id
   *
   * @return stdClass
   */
  public function getItem($id) {
    $item_url = $this->constructItemUrl($id);
    // Get the XML object at the specified URL;
    $xml = $this->loadXmlUrl($item_url);
    if ($xml) {
      $return = new stdclass;
      $return->xml = $xml;
      return $return;
    }
    else {
      $migration = Migration::currentMigration();
      $message =  t('Loading of !objecturl failed:', array('!objecturl' => $object_url));
      foreach (libxml_get_errors() as $error) {
        $message .= "\n" . $error->message;
      }
      $migration->getMap()->saveMessage(
        array($this->id), $message, MigrationBase::MESSAGE_ERROR);
      libxml_clear_errors();
      return NULL;
    }
  }

  /**
   * The default implementation simply replaces the :id token in the URL with
   * the ID obtained from MigrateListXML. Override if the item URL is not
   * so easily expressed from the ID.
   *
   * @param mixed $id
   */
  protected function constructItemUrl($id) {
    return str_replace(':id', $id, $this->itemUrl);
  }

  /**
   * Default XML loader - just use Simplexml directly. This can be overridden for
   * preprocessing of XML (removal of unwanted elements, caching of XML if the
   * source service is slow, etc.)
   */
  protected function loadXmlUrl($item_url) {
    return simplexml_load_file($item_url);
  }
}

/**
 * Adds xpath info to field mappings for XML sources
 */
class MigrateXMLFieldMapping extends MigrateFieldMapping {
  /**
   * The xpath used to retrieve the data for this field from the XML.
   *
   * @var string
   */
  protected $xpath;
  public function getXpath() {
    return $this->xpath;
  }

  /**
   * Add an xpath to this field mapping
   *
   * @param string $xpath
   */
  public function xpath($xpath) {
    $this->xpath = $xpath;
    return $this;
  }
}

/**
 * Migrations using XML sources should extend this class instead of Migration.
 */
abstract class XMLMigration extends Migration {
  /**
   * Override the default addFieldMapping(), so we can create our special
   * field mapping class.
   * TODO: Find a cleaner way to just substitute a different mapping class
   *
   * @param string $destinationField
   *  Name of the destination field.
   * @param string $sourceField
   *  Name of the source field (optional).
   */
  protected function addFieldMapping($destination_field, $source_field = NULL) {
    // Warn of duplicate mappings
    if (!is_null($destination_field) && isset($this->fieldMappings[$destination_field])) {
      $this->showMessage(
        t('!name addFieldMapping: !dest was previously mapped, overridden',
          array('!name' => $this->machineName, '!dest' => $destination_field)),
        'warning');
    }
    $mapping = new MigrateXMLFieldMapping($destination_field, $source_field);
    if (is_null($destination_field)) {
      $this->fieldMappings[] = $mapping;
    }
    else {
      $this->fieldMappings[$destination_field] = $mapping;
    }
    return $mapping;
  }

  /**
   * A normal $data_row has all the input data as top-level fields - in this
   * case, however, the data is embedded within a SimpleXMLElement object in
   * $data_row->xml. Explode that out to the normal form, and pass on to the
   * normal implementation.
   *
   * @param stdClass $data_row
   */
  protected function applyMappings(stdClass $data_row) {
    // We only know what data to pull from the xpaths in the mappings.
    foreach ($this->fieldMappings as $mapping) {
      $source = $mapping->getSourceField();
      if ($source) {
        $xpath = $mapping->getXpath();
        if ($xpath) {
          // Derived class may override applyXpath()
          $data_row->$source = $this->applyXpath($data_row, $xpath);
        }
      }
    }
    return parent::applyMappings($data_row);
  }

  /**
   * Default implementation - straightforward xpath application
   *
   * @param $data_row
   * @param $xpath
   */
  public function applyXpath($data_row, $xpath) {
    $result = $data_row->xml->xpath($xpath);
    if ($result) {
      if (count($result) > 1) {
        $return = array();
        foreach ($result as $record) {
          $return[] = (string)$record;
        }
        return $return;
      }
      else {
        return (string)$result[0];
      }
    }
    else {
      return NULL;
    }
  }
}
