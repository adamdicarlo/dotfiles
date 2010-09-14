<?php
// $Id: features_clone.drush.inc,v 1.1.2.2 2010/09/11 22:15:20 inadarei Exp $

/**
* @File Features Clone Drush Plugin
*/

require_once (dirname(__FILE__) . '/features_clone.class.inc');

/**
 * Implementation of hook_drush_command().
 */
function features_clone_drush_command() {
  $items['features-clone'] = array(
    'description' => dt('Clones a Feature to sites/defaul under a different name.'),
    'callback' => 'features_clone_execute',
    'arguments' => array(
      'src_featurename' => dt('Required. Modulename (machine name) of the Feature to Clone.'),
      'dst_featurename' => dt('Required. Machine name of the new Feature created after cloning.'),      
    ),    
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function features_clone_drush_help($section) {
  switch ($section) {
    case 'drush:features clone':
      return dt("drush features clone <src_featurename> <dst_featurname>");
  }
}

/**
 * Drush callback; make based on the makefile.
 */
function features_clone_execute($src_featurename, $dst_featurename) {

  $cloner = new features_cloner();
  
  $ret = $cloner->execute(func_get_args());
  
  if ($ret === FALSE) {
    print("Aborting.\n");
    return;
  }  

}

