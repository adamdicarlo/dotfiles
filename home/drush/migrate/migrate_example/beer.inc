<?php
// $Id: beer.inc,v 1.1.4.15 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * A basic migration example.
 */

/**
 * To define a migration process from a set of source data to a particular
 * kind of Drupal object (for example, a specific node type), you define
 * a class derived from Migration. You must define a constructor to initialize
 * your migration object. The name of the class must end in "Migration", and the
 * portion of the name before "Migration" will be the "machine name" of your migration -
 * this is the name you will use in drush commands.
 *
 * In any serious migration project, you will find there are some options
 * which are common to the individual migrations you're implementing. You can
 * define an abstract intermediate class derived from Migration, then derive your
 * individual migrations from that, to share settings, utility functions, etc.
 */
abstract class BasicExampleMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    parent::__construct();

    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default the
    // list in the shared class; it can be overridden for specific migrations.
    $this->team = array(
      new MigrateTeamMember('Liz Taster', 'ltaster@example.com', t('Product Owner')),
      new MigrateTeamMember('Larry Brewer', 'lbrewer@example.com', t('Implementor')),
    );

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    $this->issuePattern = 'http://drupal.org/node/:id';
  }
}

/**
 * There are four essential components to set up in your constructor:
 *  $this->source - An instance of a class derived from MigrateSource, this
 *    will feed data to the migration.
 *  $this->destination - An instance of a class derived from MigrateDestination,
 *    this will receive data that originated from the source and has been mapped
 *    by the Migration class, and create Drupal objects.
 *  $this->map - An instance of a class derived from MigrateMap, this will keep
 *    track of which source items have been imported and what destination objects
 *    they map to.
 *  Mappings - Use $this->addFieldMapping to tell the Migration class what source
 *    fields correspond to what destination fields, and additional information
 *    associated with the mappings.
 */
class BeerTermMigration extends BasicExampleMigration {
  public function __construct() {
    parent::__construct();
    // Human-friendly description of your migration process. Be as detailed as you
    // like.
    $this->description = t('Migrate styles from the source database to taxonomy terms');

    // Create a map object for tracking the relationships between source rows
    // and their resulting Drupal objects. Usually, you'll use the MigrateSQLMap
    // class, which uses database tables for tracking. Pass the machine name
    // (BeerTerm) of this migration to use in generating map and message tables.
    // And, pass schema definitions for the primary keys of the source and
    // destination - we need to be explicit for our source, but the destination
    // class knows its schema already.
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'style' => array('type' => 'varchar',
                           'length' => 255,
                           'not null' => TRUE,
                           'description' => 'Topic ID',
                          )
        ),
        MigrateDestinationTerm::getKeySchema()
      );

    // We are getting data from tables in the Drupal default database - first,
    // set up a query for this data.
    $query = db_select('migrate_example_beer_topic', 'met')
             ->fields('met', array('style', 'details', 'style_parent', 'region', 'hoppiness'))
             // This sort assures that parents are saved before children.
             ->orderBy('style_parent', 'ASC');

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($query);

    // Set up our destination - terms in the migrate_example_beer_styles vocabulary
    $this->destination = new MigrateDestinationTerm('Migrate Example Beer Styles');

    // Assign mappings TO destination fields FROM source fields. To discover
    // the names used in these calls, use the drush commands
    // drush migrate-fields-destination beerterm
    // drush migrate-fields-source beerterm
    $this->addFieldMapping('name', 'style');
    $this->addFieldMapping('description', 'details');

    // Documenting your mappings makes it easier for the whole team to see
    // exactly what the status is when developing a migration process.
    $this->addFieldMapping('parent_name', 'style_parent')
         ->description(t('The incoming style_parent field is the name of the term parent'));

    // Mappings are assigned issue groups, by which they are grouped on the
    // migration info page when the migrate_ui module is enabled. The default
    // is 'Done', indicating active mappings which need no attention. A
    // suggested practice is to use groups of:
    // Do Not Migrate (or DNM) to indicate source fields which are not being used,
    //  or destination fields not to be populated by migration.
    // Client Issues to indicate input from the client is needed to determine
    //  how a given field is to be migrated.
    // Implementor Issues to indicate that the client has provided all the
    //  necessary information, and now the implementor needs to complete the work.
    $this->addFieldMapping(NULL, 'hoppiness')
         ->description(t('This info will not be maintained in Drupal'))
         ->issueGroup(t('DNM'));

    // Open mapping issues can be assigned priorities (the default is
    // MigrateFieldMapping::ISSUE_PRIORITY_OK). If you're using an issue
    // tracking system, and have defined issuePattern (see ExampleMigration
    // above), you can specify a ticket/issue number in the system on the
    // mapping and migrate_ui will link directory to it.
    $this->addFieldMapping(NULL, 'region')
         ->description('Will a field be added to the vocabulary for this?')
         ->issueGroup(t('Client Issues'))
         ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_MEDIUM)
         ->issueNumber(770064);

    // It is good practice to account for all source and destination fields
    // explicitly - this makes sure that everyone understands exactly what is
    // being migrated and what is not. Also, migrate_ui highlights unmapped
    // fields, or mappings involving fields not in the source and destination,
    // so if (for example) a new field is added to the destination field it's
    // immediately visible, and you can find out if anything needs to be
    // migrated into it.
    $this->addFieldMapping('format')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('weight')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('parent')
         ->issueGroup(t('DNM'));
  }
}

/**
 * And that's it for the BeerTerm migration! For a simple migration, all you
 * have to do is define the source, the destination, and mappings between the
 * two - to import the data you simply do:
 * drush migrate-import beerterm
 *
 * However, in real-world migrations not everything can be represented simply
 * through static mappings - you will frequently need to do some run-time
 * transformations of the data.
 */
class BeerUserMigration extends BasicExampleMigration {
  public function __construct() {
    // The basic setup is similar to BeerTermMigraiton
    parent::__construct();
    $this->description = t('Beer Drinkers of the world');
    $this->map = new MigrateSQLMap($this->machineName,
        array('aid' => array(
                'type' => 'int',
                'not null' => TRUE,
                'description' => 'Account ID.'
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = db_select('migrate_example_beer_account', 'mea')
             ->fields('mea', array('aid', 'status', 'posted', 'name', 'nickname', 'password', 'mail', 'sex', 'beers'));
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();

    // One good way to organize your mappings is in three groups - mapped fields,
    // unmapped source fields, and unmapped destination fields

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('status', 'status');

    // Here we have a special case - we want to transform a date/time string
    // into a UNIX timestamp, and also apply a specific timezone. The
    // mapping of posted to created here is primarily for the sake of
    // documentation - the prepare method below actually populates the field.
    $this->addFieldMapping('created', 'posted')
         ->description('See prepare method');
    // We set the timezone here for efficiency - it will be used in the prepare
    // method
    // TODO: Don't set site timezone, affects lastupdatetime
//    date_default_timezone_set('US/Mountain');

    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('pass', 'password');
    // Instead of mapping a source field to a destination field, you can
    // hardcode a default value. You can also use both together - if a default
    // value is provided in addition to a source field, the default value will
    // be applied to any rows where the source field is empty or NULL.
    $this->addFieldMapping('roles')
         ->defaultValue(drupal_map_assoc(array(2)));

    // Unmapped source fields
    $this->addFieldMapping(NULL, 'nickname')
         ->issueGroup(t('DNM'));

    // Unmapped destination fields
    $this->addFieldMapping('theme')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('signature')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('access')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('login')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('timezone')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('picture')
         ->issueGroup(t('DNM'));

    // Oops, we made a typo - this should have been 'init'! If you have
    // migrate_ui enabled, look at the BeerUser info page - you'll see that it
    // displays a warning "int used as destination field in mapping but not in
    // list of destination fields", and also lists "1 unmapped" under Destination,
    // where it highlights "init" as unmapped.
    $this->addFieldMapping('int')
         ->issueGroup(t('DNM'));
  }

  // If you define a prepare() method in your migration, it will be called
  // after the mappings have been applied and before the destination object is
  // saved. The first argument is the destination object as built up using
  // the mappings; the second argument is the raw source data.
  public function prepare(stdClass $account, stdClass $row) {
    // Source dates are in ISO format.
    // Because the mappings above have been applied, $account->created contains
    // the date/time string now - we could also pass $row->posted here.
    $account->created = strtotime($account->created);
  }
}

class BeerNodeMigration extends BasicExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Beers of the world');

    // You may optionally declare dependencies for your migration - other migrations
    // which should run first. In this case, terms assigned to our nodes and
    // the authors of the nodes should be migrated before the nodes themselves.
    $this->dependencies = array('BeerTermMigration', 'BeerUserMigration');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'bid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'Beer ID.',
          'alias' => 'b',
        )
      ),
      MigrateDestinationNode::getKeySchema()
    );

    // We have a more complicated query. The Migration class fundamentally
    // depends on taking a single source row and turning it into a single
    // Drupal object, so how do we deal with zero or more terms attached to
    // each node? One way (demonstrated for MySQL) is to pull them into a single
    // comma-separated list.
    $query = db_select('migrate_example_beer_node', 'b')
             ->fields('b', array('bid', 'name', 'body', 'excerpt', 'aid', 'countries', 'image'));
    $query->leftJoin('migrate_example_beer_topic_node', 'tb', 'b.bid = tb.bid');
    // Gives a single comma-separated list of related terms
    $query->groupBy('tb.bid');
    $query->addExpression('GROUP_CONCAT(tb.style)', 'terms');

    // By default, MigrateSourceSQL derives a count query from the main query -
    // but we can override it if we know a simpler way
    $count_query = db_select('migrate_example_beer_node', 'b');
    $count_query->addExpression('COUNT(bid)', 'cnt');

    $this->source = new MigrateSourceSQL($query, array(), $count_query);

    // Set up our destination - nodes of type migrate_example_beer
    $this->destination = new MigrateDestinationNode('migrate_example_beer');

    // Mapped fields
    $this->addFieldMapping('title', 'name')
         ->description(t('Mapping beer name in source to node title'));
    $this->addFieldMapping('sticky')
         ->description(t('Should we default this to 0 or 1?'))
         ->issueGroup(t('Client questions'))
         ->issueNumber(765736)
         ->issuePriority(MigrateFieldMapping::ISSUE_PRIORITY_LOW);

    // References to related objects (such as the author of the content) are
    // most likely going to be identifiers from the source data, not Drupal
    // identifiers (such as uids). You can use the mapping from the relevant
    // migration to translate from the old ID to the Drupal identifier.
    // Note that we also provide a default value of 1 - if the lookup fails to
    // find a corresponding uid for the aid, the owner will be the administrative
    // account.
    $this->addFieldMapping('uid', 'aid')
         ->sourceMigration('BeerUserMigration')
         ->defaultValue(1);

    // This is a multi-value text field
    $this->addFieldMapping('field_migrate_example_country', 'countries')
         ->separator('|');
    // These are related terms, which by default will be looked up by name
    $this->addFieldMapping('migrate_example_beer_styles', 'terms')
         ->separator(',');

    $arguments = MigrateTextFieldHandler::arguments(array('source_field' => 'excerpt'));
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('teaser', 'excerpt');

    // Copy an image file, write DB record to files table, and save in Field storage.
    $arguments = MigrateFileFieldHandler::arguments(drupal_get_path('module', 'migrate_example'), 'file_copy', FILE_EXISTS_RENAME);
    $this->addFieldMapping('field_migrate_example_image', 'image')
         ->arguments($arguments);

    // Oops, we've added a second mapping for sticky... Fortunately, the
    // Migrate module will warn you that the second mapping overrides the first
    $this->addFieldMapping('sticky')
         ->defaultValue(0);

    // No unmapped source fields

    // Unmapped destination fields
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

class BeerCommentMigration extends BasicExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->description = 'Comments about beers';
    $this->dependencies = array('BeerUserMigration', 'BeerNodeMigration');
    $this->map = new MigrateSQLMap($this->machineName,
      array('cid' => array(
            'type' => 'int',
            'not null' => TRUE,
           )
         ),
      MigrateDestinationComment::getKeySchema()
    );
    $query = db_select('migrate_example_beer_comment', 'mec')
             ->fields('mec', array('cid', 'cid_parent', 'name', 'mail', 'aid', 'body', 'bid', 'subject'))
             ->orderBy('cid_parent', 'ASC');
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationComment('comment_node_migrate_example_beer');

    // Mapped fields
    $this->addFieldMapping('name', 'name');
    $this->addFieldMapping('subject', 'subject');
    $this->addFieldMapping('mail', 'mail');
    $this->addFieldMapping('status')
         ->defaultValue(COMMENT_PUBLISHED);

    // Map the incoming beer ID to the resulting Drupal nid
    $this->addFieldMapping('nid', 'bid')
         ->sourceMigration('BeerNodeMigration');

    $this->addFieldMapping('uid', 'aid')
         ->sourceMigration('BeerUserMigration')
         ->defaultValue(0);
    $this->addFieldMapping('pid', 'cid_parent')
         ->sourceMigration('BeerCommentMigration')
         ->description('Parent comment.');

    $this->addFieldMapping('comment', 'body');

    // No unmapped source fields

    // Unmapped destination fields
    $this->addFieldMapping('user_name')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('user_email')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('hostname')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('created')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('changed')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('thread')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('homepage')
         ->issueGroup(t('DNM'));
    $this->addFieldMapping('language')
         ->issueGroup(t('DNM'));
  }
}
