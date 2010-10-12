<?php
// $Id$

/**
 * @file
 *   Simpletest module drush integration.
 */

/**
 * Implementation of hook_drush_command().
 */
function test_drush_command() {
  $items = array();

  $items['test'] = array(
    'callback' => 'simpletest_drush_test',
    'description' => "List all the available tests for the site.",
    'drupal dependencies' => array('simpletest'),
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function test_drush_help($section) {
  switch ($section) {
    case 'drush:test':
      return dt("List all the available tests for the site.");
  }
}

/**
 * Test command callback.
 */
function simpletest_drush_test($param = NULL) {
  // Check runtime requirements.
  if (!drush_get_option('uri')) {
    return drush_log(dt("You must specify this site's URL using the --uri parameter."), 'error');
  }

  cache_clear_all('simpletest', 'cache');

  // Retrieve all tests and groups.
  if (function_exists('simpletest_get_all_tests')) {
    $all_tests = simpletest_get_all_tests();
    $groups = simpletest_drush_test_groups($all_tests);
  }
  else {
    $groups = simpletest_test_get_all();
    $all_tests = array();
    foreach ($groups as $group) {
      $all_tests = array_merge($all_tests, array_keys($group));
    }
  }

  // Specific test class specified.
  if (isset($param) && in_array($param, $all_tests, TRUE)) {
    simpletest_drush_run_test($param);
    simpletest_clean_environment();
    return;
  }
  // Specific group specified.
  else if (isset($param) && isset($groups[$param])) {
    foreach (array_keys($groups[$param]) as $class) {
      drush_backend_invoke('test '. $class);
    }
    return;
  }
  // Run all tests.
  else if (isset($param) && $param === 'all') {
    foreach (array_keys($groups) as $group) {
      foreach (array_keys($groups[$group]) as $class) {
        drush_backend_invoke('test '. $class);
      }
    }
    return;
  }
  // Clean test environment.
  else if (isset($param) && $param === 'clean') {
    simpletest_clean_environment();
    return;
  }
  // No args, list tests.
  if (!isset($param)) {
    $rows = array();
    $rows[] = array(dt('Command'), dt('Description'));
    $rows[] = array('-------', '-----------');
    foreach ($groups as $group_name => $group_tests) {
      foreach ($group_tests as $test_class => $test_info) {
        if (!isset($rows[$test_info['group']])) {
          $rows[$test_info['group']] = array($group_name, $test_info['group']);
        }
        $rows[] = array("  {$test_class}", "  {$test_info['name']}");
      }
    }
    drush_print_table($rows, TRUE);
    return;
  }
}

/**
 * Run a single test and display any failure messages.
 */
function simpletest_drush_run_test($class) {
  db_query('INSERT INTO {simpletest_test_id} (test_id) VALUES (default)');
  $test_id = db_last_insert_id('simpletest_test_id', 'test_id');

  $test = new $class($test_id);
  $test->run();

  $info = $test->getInfo();
  $status = ((isset($test->results['#fail']) && $test->results['#fail'] > 0)
          || (isset($test->results['#exception']) && $test->results['#exception'] > 0) ? 'error' : 'ok');
  drush_log($info['name'] . ' ' . _simpletest_format_summary_line($test->results), $status);

  // If there were some failed tests show them.
  if ($status === 'error') {
    $result = db_query("SELECT * FROM {simpletest} WHERE test_id = %d AND status = 'fail' ORDER BY test_class, message_id", $test_id);
    while ($row = db_fetch_object($result)) {
      drush_set_error('DRUSH_TEST_FAIL', dt("Test @function failed: @message", array('@function' => $row->function, '@message' => $row->message)));
    }
  }
}

/**
 * Retrieve all test groups and sanitize their names to make them command-line
 * friendly.
 */
function simpletest_drush_test_groups($tests) {
  $groups = array();
  foreach (simpletest_categorize_tests($tests) as $name => $group) {
    $sanitized = strtr($name, array(' ' => ''));
    $groups[$sanitized] = $group;
  }
  return $groups;
}
