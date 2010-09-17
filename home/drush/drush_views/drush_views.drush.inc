<?php
// $Id: drush_views.drush.inc,v 1.2.2.1 2010/03/14 09:45:59 pescetti Exp $

/**
 * @file
 *
 */

define('DRUSH_VIEWS_DEFAULT_PATH', "export/views");

/**
 * Implementation of hook_drush_help().
 */
function drush_views_drush_help($section) {
  switch ($section) {
      case 'drush:views-export':
        return dt("Usage: drush [options] views-export <views>\n\n"
                 ."Export the specified views to single .view files.\n"
                 ."<views> should be a comma-separated list. For example: view1,view2,view3.\n"
                 ."If <views> is omitted, all views are exported.\n\n"
                 ."Available options:\n"
                 ."--target-path=TARGET_PATH\n"
                 ."  Store exported views in the TARGET_PATH directory.\n"
                 ."  If omitted, the default path @default_path is used.\n",
                 array('@default_path' => DRUSH_VIEWS_DEFAULT_PATH));
      case 'drush:views-import':
        return dt("Usage: drush [options] views-import <views>\n\n"
                 ."Import the specified views.\n"
                 ."Existing views will be deleted, or overridden if they are default views.\n"
                 ."<views> should be a comma-separated list of files, with an optional\n"
                 .".view extension. For example: file1,file2,file3.\n"
                 ."If <views> is a directory, all *.view files it contains will be imported.\n"
                 ."If omitted, the default path @default_path is used.\n",
                 array('@default_path' => DRUSH_VIEWS_DEFAULT_PATH));
     case 'drush:views-list':
       return dt("Usage: drush [options] views-list\n\n"
                 ."List all available views.");
     case 'drush:views-delete':
       return dt("Usage: drush [options] views-delete <views>\n\n"
                 ."Delete the specified views. Use 'ALL' to delete all normal/overridden views.");
     /*
     case 'drush:views-tag':
       return dt("Usage: drush [options] views-tag [--names] \n"
                ."Usage: drush [options] views-tag list <tags>\n"
                ."Usage: drush [options] views-tag export [--module=<name>] <tags>");
     */
  }
}

/**
 * Implementation of hook_drush_command().
 */
function drush_views_drush_command() {
  $items = array();
  $items['views-export'] = array(
    'callback' => 'drush_views_export',
    'description' => 'Export one or more views.',
    'drupal dependencies' => array('views'),
    'aliases' => array('vwex'),
  );
  $items['views-import'] = array(
    'callback' => 'drush_views_import',
    'description' => 'Import one or more non-default views.',
    'drupal dependencies' => array('views'),
    'aliases' => array('vwim'),
  );
  $items['views-list'] = array(
    'callback' => 'drush_views_list',
    'description' => 'List available views.',
    'drupal dependencies' => array('views'),
    'aliases' => array('vwls'),
  );
  $items['views-delete'] = array(
    'callback' => 'drush_views_delete',
    'description' => 'Delete or revert a view.',
    'drupal dependencies' => array('views'),
    'aliases' => array('vwrm'),
  );
  /*
  $items['views tag'] = array(
    'callback' => 'drush_views_tag',
    'description' => 'List and export views by tag.',
    'drupal dependencies' => array('views'),
    'aliases' => array('vt'),
    'examples' => array(
      'views tag' => 'List all tags',
      'views tag list default' => 'List views for tag "default".',
      'views tag export your_module > your_module/includes/your_module.views_default.inc' => 'Export all views of tag "your_module" and pipe it to the modules default views file. This is similar to doing a bulk export of all views from a tag.',
    ),
  );
  */
  return $items;
}

/**
 * Command callback: views-export.
 */
function drush_views_export($views = NULL) {
  $target_path = drush_get_option('target-path');
  if ($target_path=="")
    $target_path = DRUSH_VIEWS_DEFAULT_PATH;
  if (substr($target_path, -1) != "/")
    $target_path .= "/";
  $exports = array();
  if (is_null($views)) {
    $exports = views_get_all_views();
  }
  else {
    $views = explode(',', $views);
    foreach ($views as $view) {
      $retrieved_view = views_get_view($view);
      if (!is_object($retrieved_view))
        return drush_set_error('DRUSH_FRAMEWORK_ERROR',dt("View '@view' not found.", array('@view' => $view)));
      else
        $exports[] = $retrieved_view;
    }
  }
  drush_log(dt("Exporting !count views to directory @target_path:",
              array('!count' => count($exports), '@target_path' => $target_path)),
            'ok');
  @mkdir($target_path, 0777, TRUE);
  foreach ($exports as $export) {
    $filename=$target_path ."$export->name.view";
    drush_log(dt("- Exporting view @name to file @filename.",
                array('@name' => $export->name, '@filename' => $filename)),
              'ok');
    if (!$fh = fopen($filename, 'w'))
      return drush_set_error('DRUSH_FRAMEWORK_ERROR',dt("Cannot open '@filename' for writing.",
                  array('@filename' => $filename)));
    $code = $export->export();
    fwrite($fh, $code);
    fclose($fh);
  }
  drush_log(dt("Done."),'ok');
  return $result;
}

/**
 * Command callback: views-import.
 */
function drush_views_import($views = NULL) {
  if (!$views)
    $imports = array(DRUSH_VIEWS_DEFAULT_PATH);
  else
    $imports = explode(',', $views);
  // If the user specified a directory name, import all views from there.
  if (count($imports)==1) {
    $source_path = $imports[0];
    if (substr($source_path, -1) != "/")
      $source_path .= "/";
    if (is_dir($source_path)) {
      drush_log(dt("Importing all .view files from @source_path.",
                  array('@source_path' => $source_path)),
                'ok');
      $imports = glob($source_path ."*.view");
    }
  }
  foreach ($imports as $key => $filename) {
    if (!file_exists($filename)) {
      $new_filename = "$filename.view";
      if (file_exists($new_filename))
        $imports[$key] = $new_filename;
      else
        return drush_set_error('DRUSH_FRAMEWORK_ERROR',
                               dt("Cannot open file @filename or @new_filename.",
                                 array('@filename' => $filename,
                                 '@new_filename' => $new_filename)));
    }
  }
  // Initialize Views.
  views_include('view');
  foreach ($imports as $import) {
    drush_log(dt("- Importing views from file @filename.",
                array('@filename' => $import)),
              'ok');
    $code=file_get_contents("$import");
    eval($code);
    drush_log(dt("  Importing view: @view",
                array('@view' => $view->name)),
              'ok');
    $skip = FALSE;
    if (is_object($old_view = views_get_view($view->name))) {
      if ($old_view->type == dt('Default'))
        drush_log(dt("    View @view is a default view, overriding.",
                    array('@view' => $old_view->name)),
                  'ok');
      else {
        drush_log(dt("    View @view already exists, deleting.",
                    array('@view' => $old_view->name)),
                  'ok');
        $old_view->delete();
        views_object_cache_clear('view', $view->name);
      }
    }
    drush_log(dt("    Saving new view @view.",
                array('@view' => $view->name)),
              'ok');
    $view->save();
    menu_rebuild();
    cache_clear_all('*', 'cache_views');
    cache_clear_all();
    views_object_cache_clear('view', $view->name);
  }
  drush_log(dt("Done."),'ok');
  return;
}

/**
 * Helper function to sort views by name.
 */
function _drush_views_name_sort($a, $b) {
  return ($a->name < $b->name ? -1 : 1);
}

/**
 * Command callback: views-list.
 */
function drush_views_list() {
  $views = views_get_all_views();
  uasort($views, '_drush_views_name_sort');
  drush_log(dt("Available views:"),'ok');
  foreach ($views as $view) {
    if ($view->type == dt('Default'))
      drush_log(dt("  @view (DEFAULT)",
                  array('@view' => "$view->name")),
                'ok');
    else
      drush_log(dt("  @view",
                    array('@view' => "$view->name")),
                'ok');
  }
  drush_log(dt("@views available.",
              array('@views' => format_plural(count($views), '1 view', '@count views'))),
            'ok');
  return;
}

/**
 * Command callback: views-delete.
 */
function drush_views_delete() {
  $args = func_get_args();
  if (!count($args)) {
    return drush_set_error('DRUSH_FRAMEWORK_ERROR',
                           dt("Provide the name of the view you would like to delete."));
  }
  else {
    $views = views_get_all_views();
    // Delete all views
    if (count($args) == 1 && $args[0] == 'ALL' && empty($views['ALL'])) {
      if (drush_confirm(dt("Do you really want to delete ALL normal/overridden views?"))) {
        foreach ($views as $view_name => $view) {
          if (in_array($view->type, array('Normal', 'Overridden'))) {
            $view->delete();
            views_object_cache_clear('view', $view_name);
          }
        }
        drush_log(dt("All normal/overridden views have been deleted."),'ok');
      }
      else {
        drush_log(dt("Aborted."),'ok');
	return;
      }
    }
    else {
      foreach ($args as $view_name) {
        if (!empty($views[$view_name])) {
          if (in_array($views[$view_name]->type, array('Normal', 'Overridden'))) {          
            $views[$view_name]->delete();
            views_object_cache_clear('view', $view_name);
            drush_log(dt("The view @view_name has been deleted.",
                        array('@view_name' => $view_name)),
                      'ok');
          }
          else
            drush_log(dt("The view @view_name is neither normal nor overridden: skipping.",
                        array('@view_name' => $view_name)),
                      'ok');
        }
        else
          drush_log(dt("Unknown view @view_name.",
                      array('@view_name' => $view_name)),
                    'notice');
      }
    }
  }
}

/**
 * Command callbacks: views tag, views tag list and views tag export.
 */
function drush_views_tag() {
  $args = func_get_args();
  $output = '';
  // Default to 'list' if no arguments are given. If arguments are given, the
  // first is a command (export or list).
  $command = count($args) ? array_shift($args) : 'list';

  if (!in_array($command, array('list', 'export'))) {
    drush_set_error(dt("Command '@command' not available. See 'drush help views tag'.", array('@command' => $command)));
    return;
  }

  // List view tags.
  if ($command == 'list') {
    $tags = array();
    foreach (views_get_all_views() as $name => $view) {
      // Create an array with tags and their views, and filter by tags if provided.
      if (!empty($view->tag)  && (count($args) < 1 || in_array($view->tag, $args))) {
        $tags[$view->tag][] = $name;
      }
    }
    // Display a tag's views if --names is set or if any tags were specified. 
    if (drush_get_option(array('n', 'names')) || count($args)) {
      foreach ($tags as $tag => $names) {
        $output .= "$tag:\t" . implode($names, ", ") . "\n";
      }
    }
    else {
      $output .= implode(array_keys($tags), "\n");
      $output .= "\n";
    }
  }

  // Export views by tag.
  elseif ($command == 'export') {
    if (!count($args)) {
      drush_set_error(dt('No tags specified.'));
      return;
    }

    $views = array();
    foreach (views_get_all_views() as $name => $view) {
      if (!empty($view->tag) && in_array($view->tag, $args)) {
        $views[$name] = $name;
      }
    }

    if (!count($views)) {
      drush_set_error(dt('No views found.'));
      return;
    }
    
    $m = drush_get_option(array('module'));
    // Use first tag as module name if not explicitly specified.
    $module = $m ? $m : $args[0];
    // Use the same output that is used for Views bulk export.
    $output .= "<?php\n";
    $output .= module_invoke('views', 'views_exportables', 'export', $views, $module);
  }
  // Print to STDOUT.
  print $output;
}
