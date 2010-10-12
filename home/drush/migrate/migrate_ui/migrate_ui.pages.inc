<?php
// $Id: migrate_ui.pages.inc,v 1.1.4.8 2010/07/14 19:57:38 weitzman Exp $

/**
 * @file
 */

/**
 * Menu callback for dashboard page
 */
function migrate_ui_dashboard() {
  return drupal_get_form('_migrate_ui_dashboard_form');
}

/**
 * Form definition for dashboard page
 */
function _migrate_ui_dashboard_form() {
  drupal_add_css(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.css');
  $build = array();

  $build['overview'] = array(
    '#prefix' => '<div>',
    '#markup' => migrate_overview(),
    '#suffix' => '</div>',
  );

  $header = array(
    array('data' => t('Status')),
    array('data' => t('Migration')),
    array('data' => t('Total rows')),
    array('data' => t('Imported')),
    array('data' => t('Unimported')),
    array('data' => t('Messages')),
    array('data' => t('Throughput')),
    array('data' => t('Last imported')),
    );

  $migrations = migrate_migrations();

  $rows = array();
  foreach ($migrations as $migration) {
    $row = array();
    $has_counts = TRUE;
    if (method_exists($migration, 'sourceCount')) {
      $total = $migration->sourceCount();
    }
    else {
      $has_counts = FALSE;
      $total = t('N/A');
    }
    if (method_exists($migration, 'importedCount')) {
      $imported = $migration->importedCount();
    }
    else {
      $has_counts = FALSE;
      $imported = t('N/A');
    }
    if ($has_counts) {
      $unimported = $total - $imported;
    }
    else {
      $unimported = t('N/A');
    }
    $status = $migration->getStatus();
    switch ($status) {
      case MigrationBase::STATUS_IDLE:
        $status = t('Idle');
        break;
      case MigrationBase::STATUS_IMPORTING:
        $status = t('Importing');
        break;
      case MigrationBase::STATUS_ROLLING_BACK:
        $status = t('Rolling back');
        break;
      case MigrationBase::STATUS_STOPPING:
        $status = t('Stopping');
        break;
      default:
        $status = t('Unknown');
        break;
    }

    $row['status'] = $status;
    $machine_name = $migration->getMachineName();
    $row['machinename'] =
      l($machine_name, 'admin/migrate/' . $machine_name);
    $row['importrows'] = $total;
    $row['imported'] = $imported;
    $row['unimported'] = $unimported;

    if (is_subclass_of($migration, 'Migration')) {
      $num_messages = $migration->messageCount();
      $row['messages'] = $num_messages ? l($num_messages, 'admin/migrate/messages/' . $machine_name) : 0;
    }
    else {
      $row['messages'] = t('N/A');
    }
    if (method_exists($migration, 'getLastThroughput')) {
      $rate = $migration->getLastThroughput();
      if ($rate == '') {
        $row['lastthroughput'] = t('Unknown');
      }
      elseif ($status == MigrationBase::STATUS_IDLE) {
        $row['lastthroughput'] = t('!rate/min', array('!rate' => $rate));
      }
      else {
        if ($rate > 0) {
          $row['lastthroughput'] = t('!rate/min, !time remaining', array('!rate' => $rate, '!time' => format_interval((60*$unimported) / $rate)));
        }
        else {
          $row['lastthroughput'] = t('!rate/min, unknown time remaining', array('!rate' => $rate));
        }
      }
    }
    else {
      $row['lastthroughput'] = t('N/A');
    }
    $row['lastimported'] = $migration->getLastImported();
    $rows[] = $row;
  }

  $build['dashboard'] = array(
    '#prefix' => '<div>',
    '#value' => theme_table($header, $rows),
    '#suffix' => '</div>',
  );
  return $build;
}

/**
 * Menu callback for messages page
 */
function migrate_ui_messages($migration) {
  drupal_add_css(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.css');
  $build = $rows = array();

  $header = array(t('Source ID'), t('Level'), t('Message'));

  if (is_string($migration)) {
    $migration = migration_load($migration);
  }

  // TODO: need a general MigrateMap API
  $messages = db_select($migration->getMap()->getMessageTable(), 'msg')
              ->extend('PagerDefault')
              ->limit(500)
              ->fields('msg')
              ->execute();

  foreach ($messages as $message) {
    $class = $message->level <= MigrationBase::MESSAGE_WARNING ? 'migrate-error' : '';
    $rows[] = array(
      array('data' => $message->sourceid1, 'class' => $class), // TODO: deal with compound keys
      array('data' => $migration->getMessageLevelName($message->level), 'class' => $class),
      array('data' => $message->message, 'class' => $class),
    );
    unset($classes);
  }

  $build['messages'] = array(
    '#prefix' => '<div>',
    '#value' => theme_table($header, $rows),
    '#suffix' => '</div>',
  );

  $build['migrate_ui_pager'] = array('#theme' => 'pager');
  return drupal_render($build);
}

/**
 * Menu callback function for migration view page.
 */
function migrate_migration_info($form_state, $migration) {
  drupal_add_css(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.css');

  if (is_string($migration)) {
    $migration = migration_load($migration);
  }

  $has_mappings = method_exists($migration, 'getFieldMappings');

  $form = array();
  $form['detail'] = array(
    '#type' => 'fieldset',
  );
  
  // Add vertical tabs display if available.
  $form['detail']['#pre_render'][] = 'vertical_tabs_form_pre_render';
  // Despite docs (http://drupal.org/handbook/modules/vertical-tabs), line below isn't working.
  // $form['detail']['#attached']['js']['vertical-tabs'] = drupal_get_path('module', 'migrate_ui') . '/migrate_ui.js';
  // Here is easy workarund.
  drupal_add_js(drupal_get_path('module', 'migrate_ui') . '/migrate_ui.js');

  if ($has_mappings) {
    $field_mappings = $migration->getFieldMappings();
    // Identify what destination and source fields are mapped
    foreach ($field_mappings as $mapping) {
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      $sourceFields[$source_field] = $source_field;
      $destinationFields[$destination_field] = $destination_field;
    }
  }

  $form['detail']['overview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overview'),
    '#group' => 'detail',
  );

  $team = array();
  foreach ($migration->getTeam() as $member) {
    $email_address = $member->getEmailAddress();
    $team[$member->getGroup()][] =
      $member->getName() . ' <' . l($email_address, 'mailto:' .$email_address) . '>';
  }

  foreach ($team as $group => $list) {
    $form['detail']['overview'][$group] = array(
      '#prefix' => '<div>',
      '#value' => theme_item_list($list, $group, 'ul', array('id' => 'team')),
      '#suffix' => '</div>',
    );
  }

  $dependencies = $migration->getHardDependencies();
  if (count($dependencies) > 0) {
    $form['detail']['overview']['dependencies'] = array(
      '#prefix' => '<div>',
      '#value' => '<strong>' . t('Dependencies: ') . '</strong>' .
         implode(', ', $dependencies),
      '#suffix' => '</div>',
    );
  }
  $soft_dependencies = $migration->getSoftDependencies();
  if (count($soft_dependencies) > 0) {
    $form['detail']['overview']['soft_dependencies'] = array(
      '#prefix' => '<div>',
      '#value' => '<strong>' . t('Soft Dependencies: ') . '</strong>' .
         implode(', ', $soft_dependencies),
      '#suffix' => '</div>',
    );
  }

  if ($has_mappings) {
    switch ($migration->getSystemOfRecord()) {
      case Migration::SOURCE:
        $system_of_record = t('Source data');
        break;
      case Migration::DESTINATION:
        $system_of_record = t('Destination data');
        break;
      default:
        $system_of_record = t('Unknown');
        break;
    }
    $form['detail']['overview']['system_of_record'] = array(
      '#prefix' => '<div>',
      '#value' => '<strong>' . t('System of record: ') . '</strong>' . $system_of_record,
      '#suffix' => '</div>',
    );
  }

  $form['detail']['overview']['description'] = array(
    '#prefix' => '<div>',
    '#value' => $migration->getDescription(),
    '#suffix' => '</div>',
  );

  if ($has_mappings) {
    // Destination field information
    $form['detail']['destination'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Destination'),
      '#group' => 'detail',
      '#description' =>
        t('<p>These are the fields available in the destination of this
           migration. The machine names listed here are those available to be used
           as the first parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $destination = $migration->getDestination();
    $form['detail']['destination']['type'] = array(
      '#type' => 'item',
      '#title' => t('Type'),
      '#value' => (string)$destination,
    );
    $destKey = $destination->getKeySchema();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($destination->fields() as $machine_name => $description) {
      $class = '';
      if (isset($destKey[$machine_name])) {
        // Identify primary key
        $machine_name .= t(' (PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        if (!isset($destinationFields[$machine_name])) {
          $class = 'migrate-error';
        }
      }
      $rows[] = array(array('data' => $machine_name, 'class' => $class), array('data' => $description, 'class' => $class));
    }

    $form['detail']['destination']['fields'] = array(
      '#prefix' => '<div>',
      '#value' => theme_table($header, $rows),
      '#suffix' => '</div>',
    );

    // TODO: Get source_fields from arguments
    $form['detail']['source'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Source'),
      '#group' => 'detail',
      '#description' =>
        t('<p>These are the fields available from the source of this
           migration. The machine names listed here are those available to be used
           as the second parameter to $this->addFieldMapping() in your Migration
           class constructor. <span class="error">Unmapped fields are red</span>.</p>'),
    );
    $source = $migration->getSource();
    $form['detail']['source']['query'] = array(
      '#type' => 'item',
      '#title' => t('Query'),
      '#value' => '<pre>' . $source . '</pre>',
    );
    $sourceKey = $migration->getMap()->getSourceKey();
    $header = array(t('Machine name'), t('Description'));
    $rows = array();
    foreach ($source->fields() as $machine_name => $description) {
      $class = '';
      if (isset($sourceKey[$machine_name])) {
        // Identify primary key
        $machine_name .= t(' (PK)');
      }
      else {
        // Add class for mapped/unmapped. Used in summary.
        if (!isset($sourceFields[$machine_name])) {
          $class =  'migrate-error';
        }
      }
      $rows[] = array(array('data' => $machine_name, 'class' => $class), array('data' => $description, 'class' => $class));
    }

    $form['detail']['source']['fields'] = array(
      '#prefix' => '<div>',
      '#value' => theme_table($header, $rows),
      '#suffix' => '</div>',
    );

    $header = array(t('Destination'), t('Source'), t('Default'), t('Description'), t('Priority'));

    // First group the mappings
    $descriptions = array();
    $source_fields = $source->fields();
    $destination_fields = $destination->fields();

    foreach ($field_mappings as $mapping) {
      // Validate source and destination fields actually exist
      $source_field = $mapping->getSourceField();
      $destination_field = $mapping->getDestinationField();
      if (!is_null($source_field) && !isset($source_fields[$source_field])) {
        drupal_set_message(t('!source used as source field in mapping but not in
          list of source fields', array('!source' => $source_field)),
        'warning');
      }
      if (!is_null($destination_field) && !isset($destination_fields[$destination_field])) {
        drupal_set_message(t('!destination used as destination field in mapping but not in
          list of destination fields', array('!destination' => $destination_field)),
        'warning');
      }
      $descriptions[$mapping->getIssueGroup()][] = $mapping;
    }

    // Put out each group header
    $rows = array();
    $count = 0;
    foreach ($descriptions as $group => $mappings) {
      $form['detail'][$group] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => t('Mapping: ') . $group,
        '#group' => 'detail',
        '#attributes' => array('class' => 'migrate-mapping'),
      );
      $rows = array();
      foreach ($mappings as $mapping) {
        $default = $mapping->getDefaultValue();
        if (is_array($default)) {
          $default = implode(',', $default);
        }
        $issue_priority = $mapping->getIssuePriority();
        if (!is_null($issue_priority)) {
          $class = 'migrate-priority-' . $issue_priority;
          $priority = MigrateFieldMapping::$priorities[$issue_priority];
          $issue_pattern = $migration->getIssuePattern();
          $issue_number = $mapping->getIssueNumber();
          if (!is_null($issue_pattern) && !is_null($issue_number)) {
            $priority .= ' (' . l('#' . $issue_number, str_replace(':id', $issue_number,
              $issue_pattern)) . ')';
          }
          if ($issue_priority != MigrateFieldMapping::ISSUE_PRIORITY_OK) {
            $class .= ' migrate-error';
          }
        }
        else {
          $priority = t('OK');
          $class .= ' migrate-priority-' . 1;
        }
        $row = array(
          array('data' => $mapping->getDestinationField(), 'class' => $class),
          array('data' => $mapping->getSourceField(), 'class' => $class),
          array('data' => $default, 'class' => $class),
          array('data' => $mapping->getDescription(),  'class' => $class),
          array('data' => $priority, 'class' => $class),
        );
        $rows[] = $row;
        $class = '';
      }
      $form['detail'][$group]['table'] = array(
        '#prefix' => '<div>',
        '#value' => theme_table($header, $rows),
        '#suffix' => '</div>',
      );
    }
  }

  return $form;
}
