<?php
// $Id: example.table_copy.inc,v 1.1.4.4 2010/09/04 20:05:49 mikeryan Exp $

/**
 * @file
 * Copy legacy urls table from mssql to drupal
 */

class LegacyURLsMigration extends Migration {
  public function __construct() {
    parent::__construct();
    $this->dependencies = array();
    $this->description = '<p>Copy legacy urls table from as an example of table_copy plugin.</p>';
    $this->team[] = new MigrateTeamMember('Moshe Weitzman', 'weitzman@tejasa.com', 'Implementors');
    $this->highwaterField = 'modificationdatetime';

    $query = db_select('migrate_example_legacy_urls', 'lu')->fields('lu');
    $this->source = new MigrateSourceSQL($query, array(
      'id' => array(
        'type' => 'int',
        'size' => 'big',
        'not null' => TRUE,
        'alias' => 'lu',
        'default' => 0,
      ),
    ));
    // By default, MigrateSourceSQL derives a count query from the main query -
    // but we can override it
    // $this->source->countQuery = db_select('migrate_example_beer', 'b');
    // $this->source->countQuery->addExpression('COUNT(bid)', 'cnt');

    $destination_key = array(
      'id' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
    );
    $this->destination = new MigrateDestinationTableCopy($destination_key, 'migrate_example_copy_urls');
  }
}
