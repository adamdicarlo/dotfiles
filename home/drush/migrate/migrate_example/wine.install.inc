<?php
// $Id: wine.install.inc,v 1.1.4.9 2010/09/27 19:24:21 mikeryan Exp $

/**
 * @file
 * Set up for the wine (advanced) example.
 */
function migrate_example_wine_schema() {
  $schema['migrate_example_wine_account'] = migrate_example_wine_schema_account();
  $schema['migrate_example_wine_account_updates'] = migrate_example_wine_schema_account_updates();
  $schema['migrate_example_wine_categories'] = migrate_example_wine_schema_categories();
  $schema['migrate_example_wine_vintages'] = migrate_example_wine_schema_vintages();
  $schema['migrate_example_wine_variety_updates'] = migrate_example_wine_schema_variety_updates();
  $schema['migrate_example_wine'] = migrate_example_wine_schema_wine();
  $schema['migrate_example_wine_updates'] = migrate_example_wine_schema_updates();
  $schema['migrate_example_wine_producer'] = migrate_example_wine_schema_producer();
  $schema['migrate_example_wine_category_wine'] = migrate_example_wine_schema_category_wine();
  $schema['migrate_example_wine_category_producer'] = migrate_example_wine_schema_category_producer();
  $schema['migrate_example_wine_comment'] = migrate_example_wine_schema_comment();
  $schema['migrate_example_wine_comment_updates'] = migrate_example_wine_schema_comment_updates();

  return $schema;
}

function migrate_example_wine_install() {
  migrate_example_wine_content_types();
  migrate_example_wine_categories();
  migrate_example_wine_fields();

  // Populate our tables.
  migrate_example_wine_data_account();
  migrate_example_wine_data_account_updates();
  migrate_example_wine_data_categories();
  migrate_example_wine_data_vintages();
  migrate_example_wine_data_variety_updates();
  migrate_example_wine_data_wine();
  migrate_example_wine_data_updates();
  migrate_example_wine_data_producer();
  migrate_example_wine_data_category_wine();
  migrate_example_wine_data_category_producer();
  migrate_example_wine_data_comment();
  migrate_example_wine_data_comment_updates();
}

function migrate_example_wine_uninstall() {
  $vocabs = taxonomy_get_vocabularies('migrate_example_wine');
  foreach ($vocabs as $vocab) {
    taxonomy_del_vocabulary($vocab->vid);
  }
  migrate_example_wine_content_type_delete();
}

function migrate_example_wine_schema_wine() {
  return array(
    'description' => 'Wines of the world',
    'fields' => array(
      'wineid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Wine ID',
      ),
      'name'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Full description of the wine.',
      ),
      'excerpt' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Abstract for this wine.',
      ),
      'accountid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'ID of the author.',
      ),
      'image' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Image path',
      ),
      'posted' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Original creation date',
      ),
      'last_changed' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Last change date',
      ),
      'variety' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Wine variety',
      ),
      'region' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Wine region',
      ),
      'rating' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Rating (100-point scale)',
      ),
      'last_reviewed' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'When it was last reviewed',
      ),
    ),
    'primary key' => array('wineid'),
  );
}

function migrate_example_wine_schema_updates() {
  return array(
    'description' => 'Updated wine ratings',
    'fields' => array(
      'wineid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Wine ID',
      ),
      'rating' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Rating (100-point scale)',
      ),
    ),
    'primary key' => array('wineid'),
  );
}

function migrate_example_wine_schema_producer() {
  return array(
    'description' => 'Wine producers of the world',
    'fields' => array(
      'producerid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Producer ID',
      ),
      'name'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Full description of the producer.',
      ),
      'excerpt' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Abstract for this producer.',
      ),
      'accountid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Account ID of the author.',
      ),
    ),
    'primary key' => array('producerid'),
  );
}

function migrate_example_wine_schema_categories() {
  return array(
    'description' => 'Categories',
    'fields' => array(
      'categoryid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
        'description' => 'Category ID',
      ),
      'type' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Type of category: variety, region, best_with',
      ),
      'name'  => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
      ),
      'details' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
      'category_parent' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Parent category, if any',
      ),
      'ordering' => array(
        'type' => 'int',
        'unsigned' => FALSE,
        'not null' => FALSE,
        'description' => 'Order in which to display categories',
      ),
    ),
    'primary key' => array('categoryid'),
  );
}

function migrate_example_wine_schema_vintages() {
  return array(
    'description' => 'Wine vintages',
    'fields' => array(
      'wineid'  => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Wine ID',
      ),
      'vintage'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Vintage (year)',
      ),
    ),
    'primary key' => array('wineid', 'vintage'),
  );
}

function migrate_example_wine_schema_variety_updates() {
  return array(
    'description' => 'Variety updates',
    'fields' => array(
      'categoryid' => array(
        'type' => 'int',
        'not null' => TRUE,
        'unsigned' => TRUE,
        'description' => 'Category ID',
      ),
      'details' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
      ),
    ),
    'primary key' => array('categoryid'),
  );
}

function migrate_example_wine_schema_category_wine() {
  return array(
    'description' => 'Wine category assignments',
    'fields' => array(
      'wineid'  => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Wine ID',
      ),
      'categoryid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Category ID',
      ),
    ),
    'primary key' => array('categoryid', 'wineid'),
  );
}

function migrate_example_wine_schema_category_producer() {
  return array(
    'description' => 'Producer category assignments',
    'fields' => array(
      'producerid'  => array(
        'type' => 'int',
        'not null' => TRUE,
        'description' => 'Producer ID',
      ),
      'categoryid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Category ID',
      ),
    ),
    'primary key' => array('categoryid', 'producerid'),
  );
}

function migrate_example_wine_schema_comment() {
  return array(
    'description' => 'Wine comments',
    'fields' => array(
      'commentid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Comment ID',
      ),
      'wineid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Wine ID that is being commented upon',
      ),
      'comment_parent' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Parent comment ID in case of comment replies.',
      ),
      'subject' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment subject',
      ),
      'body' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment body',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment name (if anon)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment email (if anon)',
      ),
      'accountid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
        'description' => 'Account ID (if any).',
      ),
      'commenthost' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'IP/domain of host posted from',
      ),
      'userpage' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'User homepage',
      ),
      'posted' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Date comment posted',
      ),
      'lastchanged' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Date comment last changed',
      ),
    ),
    'primary key' => array('commentid'),
  );
}

function migrate_example_wine_schema_comment_updates() {
  return array(
    'description' => 'Wine comment updates',
    'fields' => array(
      'commentid'  => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'Comment ID',
      ),
      'subject' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Comment subject',
      ),
    ),
    'primary key' => array('commentid'),
  );
}

function migrate_example_wine_schema_account() {
  return array(
    'description' => 'Wine accounts.',
    'fields' => array(
      'accountid'  => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Account ID',
      ),
      'status'  => array(
        'type' => 'int',
        'length' => 11,
        'not null' => TRUE,
        'description' => 'Blocked_Allowed',
      ),
      'posted' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Registration date',
      ),
      'last_access' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Last access date',
      ),
      'last_login' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Last login date',
      ),
      'name' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account name (for login)',
      ),
      'sex' => array(
        'type' => 'char',
        'length' => 1,
        'not null' => FALSE,
        'description' => 'Gender',
      ),
      'password' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account password (raw)',
      ),
      'mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Account email',
      ),
      'original_mail' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => FALSE,
        'description' => 'Original account email',
      ),
      'sig' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
        'description' => 'Signature for comments',
      ),
    ),
    'primary key' => array('accountid'),
  );
}

function migrate_example_wine_schema_account_updates() {
  return array(
    'description' => 'Wine account updates',
    'fields' => array(
      'accountid'  => array(
        'type' => 'serial',
        'not null' => TRUE,
        'description' => 'Account ID',
      ),
      'sex' => array(
        'type' => 'char',
        'length' => 1,
        'not null' => FALSE,
        'description' => 'Gender',
      ),
    ),
    'primary key' => array('accountid'),
  );
}

function migrate_example_wine_content_types() {
  // This code based on from standard.profile.
  // Insert default user-defined node types into the database.
  $types = array();
  $types[] = array(
    'type' => 'migrate_example_wine',
    'name' => st('Wine'),
    'module' => 'node',
    'description' => st("Wine is what we drink."),
    'custom' => 1,
    'modified' => 1,
    'locked' => 1,
  );
  $types[] = array(
    'type' => 'migrate_example_producer',
    'name' => st('Wine Producer'),
    'module' => 'node',
    'description' => st("Wineries, vineyards, and large producers"),
    'custom' => 1,
    'modified' => 1,
    'locked' => 1,
  );

  foreach ($types as $type) {
    $type = node_type_set_defaults($type);
    node_type_save($type);
  }
}

function migrate_example_wine_categories() {
  // Create vocabularies for variety, region, and "best with"
  $description = st('Wine varieties');
  $help = st('Select the variety of this wine');
  $vocabulary = array(
    'name' => 'Migrate Example Wine Varieties',
    'description' => $description,
    'machine_name' => 'migrate_example_wine_varieties',
    'help' => $help,
    'multiple' => 1,
    'required' => 0,
    'hierarchy' => 1,
    'relations' => 0,
    'module' => 'migrate_example',
    'nodes' => array('migrate_example_wine' => 1),
  );
  taxonomy_vocabulary_save($vocabulary);

  $description = st('Wine regions');
  $help = st('Select the region this wine comes from');
  $vocabulary = (object) array(
    'name' => 'Migrate Example Wine Regions',
    'description' => $description,
    'machine_name' => 'migrate_example_wine_regions',
    'help' => $help,
    'multiple' => 0,
    'required' => 0,
    'hierarchy' => 1,
    'relations' => 0,
    'module' => 'migrate_example',
    'nodes' => array('migrate_example_wine' => 1, 'migrate_example_producer' => 1),
  );
  taxonomy_vocabulary_save($vocabulary);

  $description = st('Foods the wine goes best with');
  $help = st('Enter any foods this wine may be paired with, separated by commas');
  $vocabulary = (object) array(
    'name' => 'Migrate Example Wine Best With',
    'description' => $description,
    'machine_name' => 'migrate_example_wine_best_with',
    'help' => $help,
    'multiple' => 1,
    'required' => 0,
    'hierarchy' => 0,
    'relations' => 0,
    'module' => 'migrate_example',
    'nodes' => array('migrate_example_wine' => 1),
  );
  taxonomy_vocabulary_save($vocabulary);
}

// Create CCK fields for our node types
function migrate_example_wine_fields() {
  $field = array(
    'field_name' => 'field_migrate_example_wine_ratin',
    'type' => 'number_decimal',
    'widget_type' => 'number',
    'type_name' => 'migrate_example_wine',
    'label' => t('Rating'),
    'scale' => 0,
  );
  content_field_instance_create($field);
  $field = array(
    'field_name' => 'field_migrate_example_wine_rvw',
    'type' => 'datestamp',
    'type_name' => 'migrate_example_wine',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'required' => '0',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'site',
    'todate' => '',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'medium',
    'widget' => array(
      'default_value' => 'blank',
      'default_value_code' => '',
      'default_value2' => 'same',
      'default_value_code2' => '',
      'input_format' => 'M j Y',
      'input_format_custom' => '',
      'increment' => '15',
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Last Reviewed',
      'weight' => '10',
      'description' => '',
      'type' => 'date_text',
      'module' => 'date',
    ),
  );
  content_field_instance_create($field);

  $field = array (
    'label' => 'Recommended vintages',
    'field_name' => 'field_migrate_example_top_vintag',
    'type' => 'number_integer',
    'type_name' => 'migrate_example_wine',
    'widget_type' => 'number',
    'change' => 'Change basic information',
    'weight' => '31',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_migrate_example_top_vintag][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' => NULL,
    'group' => false,
    'required' => 0,
    'multiple' => '1',
    'min' => '1472',
    'max' => '2010',
    'prefix' => '',
    'suffix' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'number',
    'widget_module' => 'number',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'int',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  );
  content_field_instance_create($field);

  /*  if (!field_info_field('field_migrate_example_image')) {
    $field = array(
      'field_name' => 'field_migrate_example_image',
      'type' => 'image',
      'cardinality' => 1,
      'translatable' => TRUE,
      'indexes' => array('fid' => array('fid')),
      'settings' => array(
        'uri_scheme' => 'public',
        'default_image' => FALSE,
      ),
    );
    field_create_field($field);
  }

  if (!field_info_instance('node', 'field_migrate_example_image', 'migrate_example_wine')) {
    $instance = array(
      'field_name' => 'field_migrate_example_image',
      'entity_type' => 'node',
      'label' => 'Image',
      'bundle' => 'migrate_example_wine',
      'description' => 'Upload an image to go with this wine.',
      'settings' => array(
        'file_directory' => 'field/migrate_example/image',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'alt_field' => TRUE,
        'title_field' => '',
      ),

      'widget' => array(
        'type' => 'image_image',
        'settings' => array(
          'progress_indicator' => 'throbber',
          'preview_image_style' => 'thumbnail',
        ),
        'weight' => -1,
      ),

      'display' => array(
        'full' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'type' => 'image_link_content__medium',
          'settings' => array(),
          'weight' => -1,
        ),
        'rss' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_index' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
        'search_results' => array(
          'label' => 'hidden',
          'type' => 'image__large',
          'settings' => array(),
          'weight' => -1,
        ),
      ),
    );
    field_create_instance($instance);
  }*/
}

function migrate_example_wine_content_type_delete() {
  node_type_delete('migrate_example_wine');
  node_type_delete('migrate_example_producer');
}

function migrate_example_wine_data_wine() {
  $data = array(
    array(1, 'Montes Classic Cabernet Sauvignon', 'Intense ruby-red color', 'Great!', 9, NULL,
      strtotime('2010-01-02 03:04:05'), strtotime('2010-03-04 05:06:07'), 25, 17, 95,
      '3/4/10 05:06:07 PM'),
    array(2, 'Archeo Ruggero di Tasso Nero d\'Avola', 'Lots of berry character', 'Pair with red sauced dishes', 3, NULL,
      strtotime('2010-09-03 18:23:58'), strtotime('2010-09-03 18:23:58'), 26, 2, 85),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine}
              (wineid, name, body, excerpt, accountid, image, posted, last_changed,
                variety, region, rating, last_reviewed)
              VALUES(%d, '%s', '%s', '%s', %d, '%s',
                     '%s', '%s', %d, %d, %d, '%s')",
             $row);
  }
}

function migrate_example_wine_data_updates() {
  $data = array(
    array(1, 93),
    array(2, NULL),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_updates}
              (wineid, rating)
              VALUES(%d, %d)",
             $row);
  }
}

function migrate_example_wine_data_producer() {
  $data = array(
    array(1, 'Montes', 'Fine Chilean winery', 'Great!', 9),
    array(2, 'Archeo', 'Sicilia!', NULL, 3),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_producer}
              (producerid, name, body, excerpt, accountid)
              VALUES(%d, '%s', '%s', '%s', %d)",
             $row);
  }
}

function migrate_example_wine_data_account() {
  $data = array(
    array(1, 1, '2010-03-30 10:31:05', '2010-04-30 18:25:24', '2010-04-30 14:01:02',
      'darren', 'M', 'dpass', 'ddarren@example.com', 'darren@example.com',
      'All about the Australians'),
    array(3, 0, '2007-03-15 10:31:05', '2007-06-10 04:11:38', '2007-06-10 04:11:38',
      'emily', 'F', 'insecure', 'emily@example.com', 'emily@example.com',
      'Sommelier to the stars'),
    array(9, 1, '2004-02-29 10:31:05', '2004-02-29 10:31:05', '2004-02-29 10:31:05',
      'fonzie', NULL, 'bike', 'thefonz@example.com', 'arthur@example.com',
      'Aaay!'),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_account}
              (accountid, status, posted, last_access, last_login, name, sex,
                password, mail, original_mail, sig)
              VALUES(%d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
             $row);
  }
}

function migrate_example_wine_data_account_updates() {
  $data = array(
    array(1, NULL),
    array(3, 'M'),
    array(9, 'F'),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_account_updates}
              (accountid, sex)
              VALUES(%d, '%s')",
             $row);
  }
}

function migrate_example_wine_data_comment() {
  $data = array(
    array(1, 1, NULL, 'im first', 'Tasty', 'grace', 'grace@example.com', 0,
      '123.456.78.9', 'http:://grace.example.com/',
      strtotime('2010-01-02 03:04:05'), strtotime('2010-04-05 06:07:08')),
    array(2, 1, NULL, 'im second', 'Delicious', 'horace', 'horace@example.com', 0,
      'example.com', NULL,
      strtotime('2010-02-02 03:04:05'), strtotime('2010-05-05 06:07:08')),
    array(3, 1, NULL, 'im parent', 'Don\'t care for it', 'irene', 'irene@example.com', 0,
      '254.0.2.5', 'http:://www.example.com/irene',
      strtotime('2010-03-02 03:04:05'), strtotime('2010-03-02 03:04:05')),
    array(4, 1, 3, 'im child', 'But it\'s so good!', 'emily', NULL, 3,
      '58.29.126.1', 'http:://www.wine.com/',
      strtotime('2010-01-02 03:04:05'), strtotime('2010-01-02 03:04:05')),
    array(5, 1, 4, 'im grandchild', 'Right on, Emily!', 'thefonz@example.com', NULL, 9,
      '123.456.78.9', NULL,
      strtotime('2010-06-02 03:04:05'), strtotime('2010-06-02 03:04:05')),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_comment}
              (commentid, wineid, comment_parent, subject, body, name, mail,
               accountid, commenthost, userpage, posted, lastchanged)
              VALUES(%d, %d, %d, '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s')",
             $row);
  }
}

function migrate_example_wine_data_comment_updates() {
  $data = array(
    array(1, 'I am first'),
    array(2, 'I am second'),
    array(3, 'I am parent'),
    array(4, ''),
    array(5, 'I am Spartacus'),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_comment_updates}
              (commentid, subject)
              VALUES(%d, '%s')",
             $row);
  }
}

function migrate_example_wine_data_categories() {
  $data = array(
    array(1, 'variety', 'White wine', NULL, 'White wines are generally simpler and sweeter than red', 3),
    array(3, 'variety', 'Red wine', NULL, 'Red wines are generally more complex and "dry" than white', 1),
    array(8, 'variety', 'Riesling', 1, 'Associated with Germany', 2),
    array(9, 'variety', 'Chardonnay', 1, 'One of the most popular whites', 1),
    array(13, 'variety', 'Merlot', 3, 'Very drinkable', 4),
    array(14, 'variety', 'Syrah', 3, 'A.k.a. shiraz', -3),
    array(25, 'variety', 'Cabernet Sauvignon', 3, 'A basic', -5),
    array(26, 'variety', "Nero d'Avola", 3, 'Sicilian specialty', 2),
    array(2, 'region', 'Italy', NULL, 'Largest producer of wine', 5),
    array(11, 'region', 'Tuscany', 2, NULL, 2),
    array(18, 'region', 'Chianti', 11, NULL, -1),
    array(19, 'region', 'Elba', 11, NULL, 5),
    array(4, 'region', 'France', NULL, 'C\'est bon', 6),
    array(5, 'region', 'Bordeaux', 4, NULL, 1),
    array(6, 'region', 'Barsac', 5, NULL, 3),
    array(7, 'region', 'Pomerol', 5, NULL, 2),
    array(16, 'region', 'Chile', NULL, NULL, 3),
    array(17, 'region', 'Colchagua Valley', 16, NULL, 1),
    array(20, 'region', 'California', NULL, NULL, 5),
    array(21, 'region', 'Redwood Valley', 20, NULL, 1),
    array(10, 'best_with', 'Beef', NULL, NULL, 5),
    array(12, 'best_with', 'Pork', NULL, NULL, -3),
    array(15, 'best_with', 'Chicken', NULL, NULL, -5),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_categories}
              (categoryid, type, name, category_parent, details, ordering)
              VALUES(%d, '%s', '%s', %d, '%s', %d)",
             $row);
  }
}

function migrate_example_wine_data_vintages() {
  $data = array(
    array(1, 2006),
    array(1, 2007),
    array(2, 2001),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_vintages}
              (wineid, vintage)
              VALUES(%d, %d)",
             $row);
  }
}

function migrate_example_wine_data_variety_updates() {
  $data = array(
    array(1, 'White wines are simpler and sweeter than red'),
    array(3, 'Red wines are generally more complex and dry than white'),
    array(8, 'Usually associated with Germany'),
    array(9, NULL),
    array(13, 'Common, very drinakable'),
    array(14, 'AKA Shiraz'),
    array(25, 'Basic'),
    array(26, 'A specialty of Sicily'),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_variety_updates}
              (categoryid, details)
              VALUES(%d, '%s')",
             $row);
  }
}

function migrate_example_wine_data_category_wine() {
  $fields = array('wineid', 'categoryid');
  $data = array(
    array(1, 12),
    array(1, 15),
    array(2, 10),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_category_wine}
              (wineid, categoryid)
              VALUES(%d, %d)",
             $row);
  }
}

function migrate_example_wine_data_category_producer() {
  $data = array(
    array(1, 17),
  );
  foreach ($data as $row) {
    db_query("INSERT INTO {migrate_example_wine_category_producer}
              (producerid, categoryid)
              VALUES(%d, %d)",
             $row);
  }
}
