<?php
// $Id: wine.inc,v 1.1.4.14 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * Advanced migration examples. These serve two purposes:
 *
 * 1. To demonstrate some of the more advanced usages of the Migrate module.
 *    Search for "TIP:" below for features not found in the basic example.
 * 2. To provide thorough test cases for the simpletest suite.
 *
 */

/**
 * Abstract intermediate class holding common settings.
 */
abstract class AdvancedExampleMigration extends Migration {
  public $basicFormat;

  public function __construct() {
    parent::__construct();
    $this->team = array(
      new MigrateTeamMember('Jack Kramer', 'jkramer@example.com', t('Taster')),
      new MigrateTeamMember('Linda Madison', 'lmadison@example.com', t('Winemaker')),
    );
    $this->issuePattern = 'http://drupal.org/node/:id';

    // A format of our own, for testing migration of formats
    $formats = filter_formats();
    foreach ($formats as $format) {
      if ($format->name == 'Migrate example format') {
        $this->basicFormat = $format->format;
        break;
      }
    }
  }
}

/**
 * TIP: While usually you'll create true migrations - processes that copy data
 * from some source into Drupal - you can also define processing steps for either
 * the import or rollback stages that take other actions. In this case, we want
 * to disable pathauto while the migration steps run.
 */
class WinePrepMigration extends MigrationBase {
  public function __construct() {
    parent::__construct();
    $this->description = t('If pathauto is present, disable it for the duration');
    // TIP: Regular dependencies, besides enforcing (in the absence of --force)
    // the run order of migrations, affect the sorting of migrations on display.
    // You can use soft dependencies to affect just the display order when the
    // migrations aren't technically required to run in a certain order. In this
    // case, we want the wine migrations to appear after the beer migrations -
    // without this line, they would be intermingled due to their lack of
    // (formal) interdependencies.
    $this->softDependencies = array('BeerCommentMigration');
  }
  // Define isComplete(), returning a boolean, to indicate whether dependent
  // migrations may proceed
  public function isComplete() {
    // If pathauto is disabled, other migrations are free to go
    if (module_exists('pathauto')) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }
  // Implement any action you want to occur during an import process in an
  // import() method (alternatively, if you have an action which you want to
  // run during rollbacks, define a rollback() method).
  public function import() {
    if (module_exists('pathauto')) {
      module_disable(array('pathauto'));
      $this->showMessage(t('Disabled pathauto module'), 'success');
    }
    else {
      $this->showMessage(t('Pathauto is already disabled'), 'success');
    }
    // Must return one of the MigrationBase RESULT constants
    return MigrationBase::RESULT_COMPLETED;
  }
}

// The term migrations are very similar - implement the commonalities here
abstract class WineTermMigration extends AdvancedExampleMigration {
  public function __construct($type, $vocabulary_name, $description) {
    parent::__construct();
    $this->description = $description;
    $this->dependencies = array('WinePrepMigration');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'categoryid' => array('type' => 'int',
                                'unsigned' => TRUE,
                                'not null' => TRUE,
                               )
        ),
        MigrateDestinationTerm::getKeySchema()
      );

    $query = db_select('migrate_example_wine_categories', 'wc')
             ->fields('wc', array('categoryid', 'name', 'details', 'category_parent', 'ordering'))
             ->condition('type', $type)
             // This sort assures that parents are saved before children.
             ->orderBy('category_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm($vocabulary_name);

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('description', 'details');
    $this->addFieldMapping('parent', 'category_parent')
         ->sourceMigration(get_class($this));
    $this->addFieldMapping('weight', 'ordering');
    $this->addFieldMapping('format')
         ->defaultValue($this->basicFormat);

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('parent_name')
         ->issueGroup(t('DNM'));
  }
}

class WineVarietyMigration extends WineTermMigration {
  public function __construct() {
    parent::__construct('variety', 'Migrate Example Wine Varieties',
      t('Migrate varieties from the source database to taxonomy terms'));
  }
}

class WineRegionMigration extends WineTermMigration {
  public function __construct() {
    parent::__construct('region', 'Migrate Example Wine Regions',
      t('Migrate regions from the source database to taxonomy terms'));
  }
}

class WineBestWithMigration extends WineTermMigration {
  public function __construct() {
    parent::__construct('best_with', 'Migrate Example Wine Best With',
      t('Migrate "Best With" from the source database to taxonomy terms'));
  }
}

class WineUserMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wine Drinkers of the world');
    $this->dependencies = array('WinePrepMigration');
    $this->map = new MigrateSQLMap($this->machineName,
        array('accountid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Account ID.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = db_select('migrate_example_wine_account', 'wa')
             ->fields('wa', array('accountid', 'status', 'posted', 'name',
                'password', 'mail', 'last_access', 'last_login',
                'original_mail', 'sig', 'sex'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('status', 'status');
    $this->addFieldMapping('created', 'posted')
         ->description('See prepare method');
    $this->addFieldMapping('access', 'last_access')
         ->description('See prepare method');
    $this->addFieldMapping('login', 'last_login')
         ->description('See prepare method');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('pass', 'password');
    $this->addFieldMapping('roles')
         ->defaultValue(drupal_map_assoc(array(2)));
    $this->addFieldMapping('signature', 'sig');
    $this->addFieldMapping('signature_format')
         ->defaultValue($this->basicFormat);
    $this->addFieldMapping('init', 'original_mail');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('theme')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('timezone')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('picture')
         ->issueGroup(t('DNM'));
  }

  public function prepare(stdClass $account, stdClass $row) {
    // Source dates are in ISO format.
    // Because the mappings above have been applied, $account->created contains
    // the date/time string now - we could also pass $row->posted here.
    $account->created = strtotime($account->created);
    $account->access = strtotime($account->access);
    $account->login = strtotime($account->login);
  }
}

class WineProducerMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wine producers of the world');
    $this->dependencies = array('WineRegionMigration', 'WineUserMigration');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'producerid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'alias' => 'p',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine_producer', 'p')
             ->fields('p', array('producerid', 'name', 'body', 'excerpt', 'accountid'));
    // Region term is singletons, handled straighforwardly
    $query->leftJoin('migrate_example_wine_category_producer', 'reg',
      "p.producerid = reg.producerid");
    $query->addField('reg', 'categoryid', 'region');

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // Mapped fields
    $this->addFieldMapping('title', 'name')
         ->description(t('Mapping producer name in source to node title'));
    $this->addFieldMapping('uid', 'accountid')
         ->sourceMigration('WineUserMigration')
         ->defaultValue(1);
    $this->addFieldMapping('Migrate Example Wine Regions', 'region')
         ->sourceMigration('WineRegionMigration')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('teaser', 'excerpt');
    $this->addFieldMapping('sticky')
         ->defaultValue(0);

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('is_new')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('name')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('created')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('changed')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('status')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('promote')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('revision')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }
}

/**
 * Note that, if basing a migration on an XML source, you need to derive it
 * from XMLMigration instead of Migration.
 */
class WineProducerXMLMigration extends XMLMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('XML feed of wine producers of the world');
    $this->dependencies = array('WineRegionMigration', 'WineUserMigration');

    // There isn't a consistent way to automatically identify appropriate "fields"
    // from an XML feed, so we pass an explicit list of source fields
    $fields = array(
      'name' => t('Producer name'),
      'description' => t('Description of producer'),
      'authorid' => t('Numeric ID of the author'),
      'region' => t('Name of region'),
    );

    // The source ID here is the one retrieved from the XML listing file, and
    // used to identify the specific object's file
    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'sourceid' => array(
          'type' => 'varchar',
          'length' => 4,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $xml_folder = 'http://infopeople.dev/' . drupal_get_path('module', 'migrate_example') . '/xml/';
    $list_url = $xml_folder . 'index.xml';
    // Each ID retrieved from the list URL will be plugged into :id in the
    // item URL to fetch the specific objects.
    $item_url = $xml_folder . ':id.xml';

    // We use the MigrateSourceList class for any source where we obtain the list
    // of IDs to process separately from the data for each item. The listing
    // and item are represented by separate classes, so for example we could
    // replace the XML listing with a file directory listing, or the XML item
    // with a JSON item.
    $this->source = new MigrateSourceList(new MigrateListXML($list_url),
      new MigrateItemXML($item_url), $fields);


    $this->destination = new MigrateDestinationNode('migrate_example_producer');

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.
    $this->addFieldMapping('title', 'title')
         ->xpath('/producer/name');
    $this->addFieldMapping('uid', 'authorid')
         ->xpath('/producer/authorid')
         ->sourceMigration('WineUserMigration')
         ->defaultValue(1);
    $this->addFieldMapping('Migrate Example Wine Regions', 'region')
         ->xpath('/producer/region');
    $this->addFieldMapping('body', 'description')
         ->xpath('/producer/description');
  }
}

// TODO: Add node_reference field pointing to producer
class WineWineMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Wines of the world');
    $this->dependencies = array('WineVarietyMigration', 'WineRegionMigration',
      'WineBestWithMigration', 'WineUserMigration', 'WineProducerMigration');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine', 'w')
             ->fields('w', array('wineid', 'name', 'body', 'excerpt', 'accountid',
              'image', 'posted', 'last_changed', 'variety', 'region', 'rating',
              'last_reviewed'));
    $query->leftJoin('migrate_example_wine_category_wine', 'cwbw',
      "w.wineid = cwbw.wineid");
    $query->leftJoin('migrate_example_wine_categories', 'bw',
      "cwbw.categoryid = bw.categoryid AND bw.type = 'best_with'");
    // Gives a single comma-separated list of related terms
    $query->groupBy('cwbw.wineid');
    $query->addExpression('GROUP_CONCAT(bw.categoryid)', 'best_with');

    $count_query = db_select('migrate_example_wine', 'w');
    $count_query->addExpression('COUNT(wineid)', 'cnt');

    // TIP: By passing an array of source fields to the MigrateSourceSQL constructor,
    // we can modify the descriptions of source fields (which just default, for
    // SQL migrations, to table_alias.column_name), as well as add additional fields
    // (which may be populated in prepareRow()).
    $source_fields = array(
      'wineid' => t('Wine ID in the old system'),
      'name' => t('The name of the wine'),
      'best_vintages' => t('What years were best for this wine?'),
    );

    $this->source = new MigrateSourceSQL($query, $source_fields, $count_query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');

    // Mapped fields
    $this->addFieldMapping('title', 'name')
         ->description(t('Mapping wine name in source to node title'));
    $this->addFieldMapping('uid', 'accountid')
         ->sourceMigration('WineUserMigration')
         ->defaultValue(1);
    // TIP: By default, term relationship are assumed to be passed by name.
    // In this case, the source values are IDs, so we specify the relevant
    // migration (so the tid can be looked up in the map), and tell the term
    // field handler that it is receiving tids instead of names
    $this->addFieldMapping('Migrate Example Wine Varieties', 'variety')
         ->sourceMigration('WineVarietyMigration')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('Migrate Example Wine Regions', 'region')
         ->sourceMigration('WineRegionMigration')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('Migrate Example Wine Best With', 'best_with')
         ->separator(',')
         ->sourceMigration('WineBestWithMigration')
         ->arguments(array('source_type' => 'tid'));
    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');
    $this->addFieldMapping('field_migrate_example_wine_rvw', 'last_reviewed');
    $this->addFieldMapping('field_migrate_example_top_vintag', 'best_vintages');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('teaser', 'excerpt');
/*    $arguments = MigrateFileFieldHandler::arguments(drupal_get_path('module', 'migrate_example'), 'file_copy', FILE_EXISTS_RENAME);
    $this->addFieldMapping('field_migrate_example_image', 'image')
         ->arguments($arguments);*/
    $this->addFieldMapping('sticky')
         ->defaultValue(0);
    // These are already UNIX timestamps, so just pass through
    $this->addFieldMapping('created', 'posted');
    $this->addFieldMapping('changed', 'last_changed');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('is_new')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('status')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('promote')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('revision')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }

  // TIP: Implement a prepareRow() method to manipulate the source row between
  // retrieval from the database and the automatic applicaton of mappings
  public function prepareRow($current_row) {
    // We can only handle a single multi-value source field using GROUP_CONCAT
    // as we did above - insert others with a query against the related table
    // with multiple values here, so the values can run through the mapping process
    $source_id = $current_row->wineid;
    $result = db_select('migrate_example_wine_vintages', 'v')
              ->fields('v', array('vintage'))
              ->condition('wineid', $source_id)
              ->execute();
    foreach ($result as $row) {
      $current_row->best_vintages[] = $row->vintage;
    }
    // We could also have used this function to decide to skip a row, in cases
    // where that couldn't easy be done through the original query. Simply
    // return FALSE in such cases.
    return TRUE;
  }
}

class WineCommentMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Comments about wines';
    $this->dependencies = array('WineUserMigration', 'WineWineMigration');
    $this->map = new MigrateSQLMap($this->machineName,
      array('commentid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
             )
           ),
        MigrateDestinationComment::getKeySchema()
      );
    $query = db_select('migrate_example_wine_comment', 'wc')
             ->fields('wc', array('commentid', 'comment_parent', 'name', 'mail',
              'accountid', 'body', 'wineid', 'subject', 'commenthost', 'userpage',
              'posted', 'lastchanged'))
             ->orderBy('comment_parent');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_wine');

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('status')
         ->defaultValue(COMMENT_PUBLISHED);
    $this->addFieldMapping('nid', 'wineid')
         ->sourceMigration('WineWineMigration');
    $this->addFieldMapping('uid', 'accountid')
         ->sourceMigration('WineUserMigration')
         ->defaultValue(0);
    $this->addFieldMapping('pid', 'comment_parent')
         ->sourceMigration('WineCommentMigration')
         ->description('Parent comment');
    $this->addFieldMapping('comment', 'body');
    $this->addFieldMapping('hostname', 'commenthost');
    $this->addFieldMapping('timestamp', 'lastchanged');
    $this->addFieldMapping('homepage', 'userpage');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('thread')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }
}

class WineFinishMigration extends MigrationBase {
  public function __construct() {
    parent::__construct();
    $this->description = t('If pathauto is present and was previously enabled,
      re-enable it');
    $this->dependencies = array('WineCommentMigration');
  }
  public function isComplete() {
    if (module_exists('pathauto')) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  public function import() {
    if (!module_exists('pathauto')) {
      // TODO: Really should keep track of whether it was originally enabled
      module_enable(array('pathauto'));
      $this->showMessage(t('Re-enabled pathauto module'), 'success');
    }
    else {
      $this->showMessage(t('Pathauto module already enabled', 'success'));
    }
    return Migration::RESULT_COMPLETED;
  }
}

/**
 * TIP: This demonstrates a migration designed not to import new content, but
 * to update existing content (in this case, revised wine ratings)
 */
class WineUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Update wine ratings');
    $this->dependencies = array('WineWineMigration');
    $this->softDependencies = array('WineFinishMigration');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'wineid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'Wine ID',
          'alias' => 'w',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $query = db_select('migrate_example_wine_updates', 'w')
             ->fields('w', array('wineid', 'rating'));

    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationNode('migrate_example_wine');

    // Indicate we're updating existing data. The default, Migration::SOURCE, would
    // cause existing nodes to be completely replaced by the source data. In this
    // case, the existing node will be loaded and only the rating altered.
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    // The destination handler needs the nid to change - since the incoming data
    // has a source id, not a nid, we need to apply the original wine migration
    // mapping to populate the nid.
    $this->addFieldMapping('nid', 'wineid')
         ->sourceMigration('WineWineMigration');
    $this->addFieldMapping('field_migrate_example_wine_ratin', 'rating');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('uid');
    $this->addFieldMapping('migrate_example_wine_varieties');
    $this->addFieldMapping('migrate_example_wine_regions');
    $this->addFieldMapping('migrate_example_wine_best_with');
    $this->addFieldMapping('body');
    $this->addFieldMapping('field_migrate_example_image');
    $this->addFieldMapping('sticky');
    $this->addFieldMapping('created');
    $this->addFieldMapping('changed');
    $this->addFieldMapping('is_new')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('status')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('promote')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('revision')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }
}

class WineCommentUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Update wine comments';
    $this->dependencies = array('WineCommentMigration');
    $this->softDependencies = array('WineUpdatesMigration');
    $this->map = new MigrateSQLMap($this->machineName,
      array('commentid' => array(
              'type' => 'int',
              'unsigned' => TRUE,
              'not null' => TRUE,
             )
           ),
        MigrateDestinationComment::getKeySchema()
      );
    $query = db_select('migrate_example_wine_comment_updates', 'wc')
             ->fields('wc', array('commentid', 'subject'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_wine');
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('cid', 'commentid')
         ->sourceMigration('WineCommentMigration');
    $this->addFieldMapping('subject', 'subject');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('mail');
    $this->addFieldMapping('status');
    $this->addFieldMapping('nid');
    $this->addFieldMapping('uid');
    $this->addFieldMapping('pid');
    $this->addFieldMapping('comment_body');
    $this->addFieldMapping('hostname');
    $this->addFieldMapping('created');
    $this->addFieldMapping('changed');
    $this->addFieldMapping('homepage');
    $this->addFieldMapping('thread')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }
}

class WineVarietyUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate varieties from the source database to taxonomy terms');
    $this->dependencies = array('WineVarietyMigration');
    $this->softDependencies = array('WineUpdatesMigration');
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'categoryid' => array('type' => 'int',
                                'unsigned' => TRUE,
                                'not null' => TRUE,
                               )
        ),
        MigrateDestinationTerm::getKeySchema()
      );

    $query = db_select('migrate_example_wine_variety_updates', 'wc')
             ->fields('wc', array('categoryid', 'details'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationTerm('migrate_example_wine_varieties');
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('tid', 'categoryid')
         ->sourceMigration('WineVarietyMigration');
    $this->addFieldMapping('description', 'details');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('parent');
    $this->addFieldMapping('weight');
    $this->addFieldMapping('format');
    $this->addFieldMapping('parent_name')
         ->issueGroup(t('DNM'));
  }
}


class WineUserUpdatesMigration extends AdvancedExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Account updates');
    $this->dependencies = array('WineUserMigration');
    $this->softDependencies = array('WineUpdatesMigration');
    $this->map = new MigrateSQLMap($this->machineName,
        array('accountid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => 'Account ID.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = db_select('migrate_example_wine_account_updates', 'wa')
             ->fields('wa', array('accountid', 'sex'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    $this->systemOfRecord = Migration::DESTINATION;

    // Mapped fields
    $this->addFieldMapping('uid', 'accountid')
         ->sourceMigration('WineUserMigration');

    // Unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('name');
    $this->addFieldMapping('status');
    $this->addFieldMapping('created');
    $this->addFieldMapping('access');
    $this->addFieldMapping('login');
    $this->addFieldMapping('mail');
    $this->addFieldMapping('pass');
    $this->addFieldMapping('roles');
    $this->addFieldMapping('signature');
    $this->addFieldMapping('signature_format');
    $this->addFieldMapping('init');
    $this->addFieldMapping('theme')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('timezone')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('picture')
         ->issueGroup(t('DNM'));
  }
}
