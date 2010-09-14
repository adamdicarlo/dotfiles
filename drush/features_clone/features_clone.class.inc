<?php 

// $Id: features_clone.class.inc,v 1.1.2.3 2010/01/06 00:44:03 inadarei Exp $

/**
* @File Features Clone Drush Plugin implementation class
*/

class features_cloner {
  
  var $src_feature;
  var $dst_feature;
  
  var $src_path;
  var $dst_path;
  
  function execute($args) {
    
    $num_of_arguments_supplied = sizeof($args);
    $this->src_feature = empty($args[0]) ? "" : $args[0];
    $this->dst_feature = empty($args[1]) ? "" : $args[1];    
    
    $ret = $this->check_arguments($args);
    
    //if validation did not pass
    if ($ret === FALSE) { 
      return $ret;
    }
    
    /** For debugging only:
      $ret = _drush_sql_get_credentials();
      print_r($ret);
      echo("\n");
    */
        
    print "Cloning $this->src_path into: $this->dst_path \n";
  
    $this->recursive_copy_and_filter($this->src_path, $this->dst_path);
  
    print "Feature Clone Complete\n";
    
    cache_clear_all();

  }
  
  function check_arguments($args) {
  
    $please_refer = ' ' . dt('Please refer to: drush help features clone');
    
    if (empty($this->src_feature)) {
      drush_set_error('FEATURES_CLONE_INVALID_FEATURENAME_SRC', 
                       dt('You need to provide the name of the feature to clone.' . $please_refer));
      return FALSE;
    }
  
    if (empty($this->dst_feature)) {
      drush_set_error('FEATURES_CLONE_INVALID_FEATURENAME_DST', 
                      dt('You need to provide the name of the feature to create after cloning.' . $please_refer));
      return FALSE;
    }
  
    if ( sizeof($args) > 2) {
      drush_set_error('FEATURES_CLONE_EXTRA_ARGUMENTS', dt('You provided extra arguments.' . $please_refer));
      return FALSE;    
    }
    
    if (!preg_match('!^[a-z0-9_]+$!', $this->dst_feature)) {
      drush_set_error('FEATURES_CLONE_INVALID_FEATURENAME_DST', 
                      dt('Invalid name for the destination feature. The machine-readable name must contain only lowercase letters, numbers or underscores.'));
      return FALSE;
    }
  
      
    $this->dst_path = "sites/default/modules/custom_features/" . $this->dst_feature;
        
    //Did we get a machine-readable module name? 
    //Run a query, it's safer than drupal_get_path since it only checks enabled modules.
    $sql = "select filename from {system} where name='%s' and status=1";
    $modulefile = db_result(db_query_range($sql, $this->src_feature, 0, 1)); 
    $this->src_path = dirname($modulefile);
    
    if (empty($this->src_path)) {
      drush_set_error('FEATURES_CLONE_INVALID_FEATURENAME_SRC', 
                      dt('Invalid source feature name. No module found with such name.' . $please_refer));
      return FALSE;
    }  
    
    if (file_exists($this->dst_path)) {
      drush_print(dt('A module appears to already exist in !dir', array('!dir' => $this->dst_path)));
      if (!drush_confirm(dt('Should we remove it and replace with the cloned feature?'))) {
        return FALSE;
      }
      $this->recursive_remove_dir($this->dst_path);
    }    
    
    return TRUE; 
  }
  
  
  /**
  * Copy files recursively and filter content to change module/function names for cloning.
  */
  function recursive_copy_and_filter($src, $dst, $mode = 0775) {
  
    if (substr($src, -1) != '/') {
      $src .= '/';
    }
  
    $files = glob($src .'*', GLOB_MARK);
  
    if (empty($files)) {
      drush_die("Source folder $src seems to be empty!");
    }
  
    mkdir($dst, $mode, $recursive = TRUE);
  
    foreach ($files as $file) {
  
      $file_name = basename($file);
  
      if (is_dir($file)) {
        $this->recursive_copy_and_filter($file, $dst .'/'. $file_name);
      } else {
        $file_name = trim($file_name);
        $extension = array_pop(explode('.', $file_name));
        $file_types_to_alter = array('inc', 'module', 'install', 'info');
        if (in_array($extension, $file_types_to_alter)) {
          $mod_filename = str_replace($this->src_feature, $this->dst_feature, $file_name);  
          $mod_path = $dst .'/'. $mod_filename;       
            //-- print-out human-friendly partial pathes
            $partial_path = str_replace($this->dst_path . '/', '', $mod_path);          
          print (dt(" .. Modified & Copied: !partial_path ", array('!partial_path' => $partial_path)) . "\n");           
          $this->modify_and_copy($file, $mod_path);
        }
        else {
          $path = $dst .'/'. $file_name;
            //-- print-out human-friendly partial pathes
            $partial_path = str_replace($this->dst_path . '/', '', $path);
          print (dt(" .. Copied: !partial_path ", array('!partial_path' => $partial_path)) . "\n"); 
          copy($file, $path);
        }
  
        
      }
  
    }
  
  }
  
  
  /**
  * Copies source file to the destination, making appropriate changes to its source along the way
  */
  function modify_and_copy($src, $dst) {
    $text = file_get_contents($src);
    $text = str_replace($this->src_feature, $this->dst_feature, $text);
    
    //-- Let's fix description and name attributes in the .info file, to be extra nice:
    $file_name = basename($dst);
    if ($file_name == $this->dst_feature . '.info') {
      
      $nice_name = $this->create_nice_name($this->dst_feature);
      
      $text = preg_replace('/name\s*=\s*.+/i', "name = \"$nice_name\"", $text);
      
      $nice_name = $nice_name . " " . dt("Feature");
      $text = preg_replace('/description\s*=\s*.+/i', "description = \"$nice_name\"", $text);      
    }

    file_put_contents($dst, $text);  
  }
  
  /**
  * Auto-create a "nice" name out of machine name
  */
  function create_nice_name($name) {
    $nice_name = $name;
    $nice_name = str_replace ("_", " ", $nice_name);    
    $nice_name = ucwords($nice_name);
    return $nice_name;
  }

  
  /**
  * @source: http://lixlpixel.org/recursive_function/php/recursive_directory_delete/
  */
  function recursive_remove_dir($directory, $empty=FALSE)
  {
      // if the path has a slash at the end we remove it here
      if (substr($directory,-1) == '/') {
          $directory = substr($directory,0,-1);
      }
   
      // if the path is not valid or is not a directory ...
      if (!file_exists($directory) || !is_dir($directory)) {
          // ... we return false and exit the function
          return FALSE;
   
      // ... if the path is not readable
      }
      elseif (!is_readable($directory)) {
          // ... we return false and exit the function
          return FALSE;
   
      // ... else if the path is readable
      } 
      else {
   
          // we open the directory
          $handle = opendir($directory);
   
          // and scan through the items inside
          while (FALSE !== ($item = readdir($handle))) {
              // if the filepointer is not the current directory
              // or the parent directory
              if ($item != '.' && $item != '..') {
                  // we build the new path to delete
                  $path = $directory.'/'.$item;
   
                  // if the new path is a directory
                  if(is_dir($path)) {
                      // we call this function with the new path
                      $this->recursive_remove_dir($path);
   
                  // if the new path is a file
                  } 
                  else {
                      // we remove the file
                      unlink($path);
                  }
              }
          }
          // close the directory
          closedir($handle);
   
          // if the option to empty is not set to true
          if($empty == FALSE) {
              // try to delete the now empty directory
              if(!rmdir($directory)) {
                  // return false if not possible
                  return FALSE;
              }
          }
          // return success
          return TRUE;
      }
  }  

  
}
