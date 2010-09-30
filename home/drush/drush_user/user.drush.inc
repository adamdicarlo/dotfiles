<?php
// $Id: 

/**
 * @file Drush User Management commands
 */

/**
 * Implementation of hook_drush_help().
 */
function user_drush_help($section) {
  switch ($section) {
    case 'drush:user-info':
      return dt("Display information about a user identified by username, uid or email address.");
    case 'drush:user-block':
      return dt("Block the specified user(s).");
    case 'drush:user-unblock':
      return dt("Unblock the specified user(s).");
    case 'drush:user-add-role':
      return dt("Add a role to the specified user accounts.");
    case 'drush:user-remove-role':
      return dt("Remove a role from the specified user accounts.");
    case 'drush:user-create':
      return dt("Create a user account.");
    case 'drush:user-cancel':
      return dt("Cancel a user account.");
    case 'drush:user-password':
      return dt("(Re)Set the password for the given user account.");
  }
}

/**
 * Implementation of hook_drush_command().
 */
function user_drush_command() {
  $items['user-info'] = array(
    'callback' => 'drush_user_info',
    'description' => 'Print information about the specified user(s).',
    'examples' => array(
      'drush user-info 2,3,someguy,somegal,billgates@microsoft.com' => 
        'Display information about any users with uids, names, or mail addresses matching the strings between commas.',
    ),
    'arguments' => array(
      'users' => 'A comma delimited list of uids, user names, or email addresses.',
    ),
    'options' => array(
      '--detail' => 'show basic, extended, or all information about the user',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-block'] = array(
    'callback' => 'drush_user_block',
    'description' => 'Block the specified user(s).',
    'examples' => array(
      'drush user-block --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' => 
        'Block the users with uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com',
    ),
    'options' => array(
      '--uid' => 'A comma delimited list of uids to block',
      '--name' => 'A comma delimited list of user names to block',
      '--mail' => 'A comma delimited list of user mail addresses to block',
      '--users' => 'A comma delimited list of user uids, names and mail addresses to block (you will be prompted to resolve any ambiguities between uids and names)',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-unblock'] = array(
    'callback' => 'drush_user_unblock',
    'description' => 'Unblock the specified user(s).',
    'arguments' => array(
      'role' => 'The name of the role to add'
    ),
    'examples' => array(
      'drush user-unblock --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' => 
        'Unblock the users with uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com',
    ),
    'options' => array(
      '--uid' => 'A comma delimited list of uids to unblock',
      '--name' => 'A comma delimited list of user names to unblock',
      '--mail' => 'A comma delimited list of user mail addresses to unblock',
      '--users' => 'A comma delimited list of user uids, names and mail addresses to block (you will be prompted to resolve any ambiguities between uids and names)',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-add-role'] = array(
    'callback' => 'drush_user_add_role',
    'description' => 'Add a role to the specified user accounts.',
    'examples' => array(
      'drush user-add-role "power user" --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' => 
        'Add the "power user" role to the accounts with uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com',
    ),
    'options' => array(
      '--uid' => 'A comma delimited list of uids',
      '--name' => 'A comma delimited list of user names',
      '--mail' => 'A comma delimited list of user mail addresses',
      '--users' => 'A comma delimited list of user uids, names and mail addresses to block (you will be prompted to resolve any ambiguities between uids and names)',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-remove-role'] = array(
    'callback' => 'drush_user_remove_role',
    'description' => 'Remove a role from the specified user accounts.',
    'examples' => array(
      'drush user-remove-role "power user" --uid=2,3 --name=someguy,somegal --mail=billgates@microsoft.com' => 
        'Remove the "power user" role from the accounts with uids 2 and 3, names someguy and somegal, and email address of billgates@microsoft.com',
    ),
    'options' => array(
      '--uid' => 'A comma delimited list of uids',
      '--name' => 'A comma delimited list of user names',
      '--mail' => 'A comma delimited list of user mail addresses',
      '--users' => 'A comma delimited list of user uids, names and mail addresses to block (you will be prompted to resolve any ambiguities between uids and names)',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-create'] = array(
    'callback' => 'drush_user_create',
    'description' => 'Create a user account with the specified name.',
    'arguments' => array(
      'name' => 'The name of the account to add'
    ),
    'examples' => array(
      'drush user-create newuser --mail="person@example.com" --password="letmein"' => 
        'Create a new user account with the name newuser, the email address person@example.com, and the password letmein',
    ),
    'options' => array(
      '--password' => 'The password for the new account',
      '--mail' => 'The email address for the new account',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-cancel'] = array(
    'callback' => 'drush_user_cancel',
    'description' => 'Cancel a user account with the specified name.',
    'arguments' => array(
      'name' => 'The name of the account to cancel',
    ),
    'options' => array(
      'delete-content' => 'Delete all content created by the user (d7 only)',
    ),
    'examples' => array(
      'drush user-cancel username' => 
        'Cancel the user account with the name username and anonymize all content created by that user.',
      'drush user-cancel --delete-content=true username' => 
        'Cancel the user account with the name username and delete all content created by that user.',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  $items['user-password'] = array(
    'callback' => 'drush_user_password',
    'description' => '(Re)Set the password for the user account with the specified name.',
    'arguments' => array(
      'name' => 'The name of the account to modify'
    ),
    'options' => array(
      '--password' => '(required) The new password for the account',
    ),
    'examples' => array(
      'drush user-password someuser --password="gr3@tP@$s"' => 
        'Set the password for the username someuser to gr3@tP@$s.',
    ),
    'drupal dependencies' => array('user'),
    'core' => array('5','6','7'),
  );
  return $items;
}

/**
 * Prints info about the specified user(s).
 */
function drush_user_info($users) {
  $users = explode(',', $users);
  $detail = drush_get_option('detail', 'basic');
  foreach($users as $user) {
    $uid = _drush_user_get_uid($user);  
    if ($uid !== FALSE) {
        _drush_user_print_info($uid, $detail);
    }
  }
}

/**
 * Block the specified user(s).
 */
function drush_user_block() {
  if ($uids = _drush_user_parse_user_args()) {
    user_user_operations_block($uids);
  }
  else {
    drush_set_error("Could not find any valid users to block!");
  }
}

/**
 * Unblock the specified user(s).
 */
function drush_user_unblock() {
  if ($uids = _drush_user_parse_user_args()) {
    user_user_operations_unblock($uids);
  }
  else {
    drush_set_error("Could not find any valid users to unblock!");
  }
}

/**
 * Add a role to the specified user accounts.
 */
function drush_user_add_role($role) {
  if ($uids = _drush_user_parse_user_args()) {
    if (drush_drupal_major_version() >= 7) {
      $rid_query = db_query("SELECT rid FROM {role} WHERE name = :role", array(':role' => $role));
    }
    else {
      $rid_query = db_query("SELECT rid FROM {role} WHERE name = '%s'", $role);
    }
    if ($rid = drush_db_result($rid_query)) {
      user_multiple_role_edit($uids, 'add_role', $rid);
      foreach($uids as $uid) {
        drush_log(dt("Added the %role role to uid %uid", array('%role' => $role, '%uid' => $uid)), 'notice');
      }
    }
    else {
      drush_set_error("There is no role named: \"$role\"!");
    }
  }
  else {
    drush_set_error("Could not find any valid users to which to add the \"$role\" role!");
  }
}

/**
 * Remove a role from the specified user accounts.
 */
function drush_user_remove_role($role) {
  if ($uids = _drush_user_parse_user_args()) {
    if (drush_drupal_major_version() >= 7) {
      $rid_query = db_query("SELECT rid FROM {role} WHERE name = :role", array(':role' => $role));
    }
    else {
      $rid_query = db_query("SELECT rid FROM {role} WHERE name = '%s'", $role);
    }
    if ($rid = drush_db_result($rid_query)) {
      user_multiple_role_edit($uids, 'remove_role', $rid);
      foreach($uids as $uid) {
        drush_log(dt("Removed the %role role from uid %uid", array('%role' => $role, '%uid' => $uid)), 'notice');
      }
    }
    else {
      drush_set_error("There is no role named: \"$role\"!");
    }
  }
  else {
    drush_set_error("Could not find any valid users from which to remove the \"$role\" role!");
  }
}

/**
 * Creates a new user account.
 */
function drush_user_create($name) {
  $mail = drush_get_option('mail');
  $pass = drush_get_option('password');
  $new_user = array(
    'name' => $name,
    'pass' => $pass,
    'mail' => $mail,
    'access' => '0',
    'status' => 1,
  );
  if (drush_drupal_major_version() >= 7) {
    $result = db_query("SELECT uid FROM {users} WHERE name = :name OR mail = :mail", array(':name' => $name, ':mail' => $new_user['mail']));
  }
  else {
    $result = db_query("SELECT uid FROM {users} WHERE name = '%s' OR mail = '%s'", $name, $new_user['mail']);
  }
  if (drush_db_result($result) === FALSE) {
    $new_user_object = user_save(NULL, $new_user, NULL);
    if ($new_user_object !== FALSE) {
      _drush_user_print_info($new_user_object->uid);
    }
    else {
      drush_set_error("Could not create a new user account with the name " . $name . "!");
    }
  }
  else {
    drush_set_error("There is already a user account with the name " . $name . " or email address " . $new_user['mail'] . "!");
  }
}

/**
 * Cancels a user account.
 */
function drush_user_cancel($name) {
  if (drush_drupal_major_version() >= 7) {
    $result = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $name));
  }
  else {
    $result = db_query("SELECT uid FROM {users} WHERE name = '%s'", $name);
  }
  $uid = drush_db_result($result);
  if ($uid !== FALSE) {
    drush_print("Cancelling the user account with the following information:");
    _drush_user_print_info($uid);
    if (strtolower(drush_get_option('delete-content')) == 'true' && drush_drupal_major_version() >= 7) {
      drush_print("All content created by this user will be deleted!");
    }
    if (drush_confirm('Cancel user account?: ')) {
      if (drush_drupal_major_version() >= 7) {
        if (strtolower(drush_get_option('delete-content')) == 'true') {
          user_cancel(array(), $uid, 'user_cancel_delete');
        }
        else {
          user_cancel(array(), $uid, 'user_cancel_reassign');
        }
        // I got the following technique here: http://drupal.org/node/638712
        $batch =& batch_get();
        $batch['progressive'] = FALSE;
        batch_process();
      }
      else {
        user_delete(array(), $uid);
      }
    }
  }
  else {
    drush_set_error("Could not find a user account with the name " . $name . "!");
  }
}

/**
 * Sets the password for the account with the given username
 */
function drush_user_password($name) {
  $pass = drush_get_option('password');
  if (empty($pass)) {
    drush_die("You must specify a password!");
  }
  if (drush_drupal_major_version() >= 7) {
    $result = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $name));
    $user = user_load(drush_db_result($result));
  }
  else {
    $user = user_load(array('name' => $name));
  }
  if ($user !== FALSE) {
    $user_object = user_save($user, array('pass' => $pass));
    if ($user_object === FALSE) {
      drush_set_error("Could not change the password for the user account with the name " . $name . "!");
    }
  }
  else {
    drush_set_error("The user account with the name " . $name . " could not be loaded!");
  }
}

/**
 * Print information about a given uid
 */
function _drush_user_print_info($uid, $detail = 'basic') {
  $userinfo = user_load(array('uid' => $uid));
  switch ($detail) {
    case 'all':
      drush_print_r($userinfo);
      break;
    case 'extended':
      $userinfo = (array)$userinfo;
      unset($userinfo['data']);
      unset($userinfo['block']);
      unset($userinfo['form_build_id']);
      foreach($userinfo as $key => $val) {
        if (is_array($val)) {
          drush_print($key . ': ');
          drush_print_r($val);
        }
        else {
          if ($key === 'created' OR $key === 'access' OR $key === 'login') {
            drush_print($key . ': ' . format_date($val));
          }
          else {
            drush_print($key . ': ' . $val);
          }
        }
      }
      break;
    case 'basic':
    default:
      drush_print("Information for '$userinfo->name'");
      drush_print("User ID: $userinfo->uid");
      drush_print("User name: $userinfo->name");
      drush_print("User mail: $userinfo->mail");
      $userinfo->status ? drush_print("User is active.") : drush_print("User is blocked.");
      drush_print("User roles: " . implode(', ', $userinfo->roles));
      drush_print("");
      drush_print_pipe("$userinfo->name, $userinfo->uid, $userinfo->mail, $userinfo->status, \"" . implode(', ', $userinfo->roles) . "\"\n");
      break;
  }
}

/**
 * Get uid(s) from a uid, user name, or email address.
 * Returns a uid, or FALSE if none found.
 */
function _drush_user_get_uid($search) {
  // We use a DB query while looking for the uid to keep things speedy.
  $uids = array();
  if (is_numeric($search)) {
    if (drush_drupal_major_version() >= 7) {
      $uid_query = db_query("SELECT uid, name FROM {users} WHERE uid = :uid OR name = :name", array(':uid' => $search, ':name' => $search));
    }
    else {
      $uid_query = db_query("SELECT uid, name FROM {users} WHERE uid = %d OR name = '%d'", $search, $search);
    }
  }
  else {
    if (drush_drupal_major_version() >= 7) {
      $uid_query = db_query("SELECT uid, name FROM {users} WHERE mail = :mail OR name = :name", array(':mail' => $search, ':name' => $search));
    }
    else {
      $uid_query = db_query("SELECT uid, name FROM {users} WHERE mail = '%s' OR name = '%s'", $search, $search);
    }
  }
  while ($uid = drush_db_fetch_object($uid_query)) {
    $uids[$uid->uid] = $uid->name;
  }
  switch (count($uids)) {
    case 0:
      return FALSE;
      break;
    case 1:
      return array_pop($uids);
      break;
    default:
      drush_print('More than one user account was found for the search string "' . $search . '".');
      return(drush_choice($uids, 'Please choose a name:', '!value (uid=!key)'));
  }
}

/**
 * Parse user identification arguments.
  */
function _drush_user_parse_user_args() {
  $users = array();
  foreach (array('uid', 'name', 'mail', 'users') as $user_attr) {
    if ($arg = drush_get_option($user_attr)) {
      foreach(explode(',', $arg) as $search) {
        $uid_query = FALSE;
        switch ($user_attr) {
          case 'uid':
            if (drush_drupal_major_version() >= 7) {
              $uid_query = db_query("SELECT uid FROM {users} WHERE uid = :uid", array(':uid' => $search));
            }
            else {
              $uid_query = db_query("SELECT uid FROM {users} WHERE uid = %d", $search);
            }
            break;
          case 'name':
            if (drush_drupal_major_version() >= 7) {
              $uid_query = db_query("SELECT uid FROM {users} WHERE name = :name", array(':name' => $search));
            }
            else {
              $uid_query = db_query("SELECT uid FROM {users} WHERE name = '%s'", $search);
            }
            break;
          case 'mail':
            if (drush_drupal_major_version() >= 7) {
              $uid_query = db_query("SELECT uid FROM {users} WHERE mail = :mail", array(':mail' => $search));
            }
            else {
              $uid_query = db_query("SELECT uid FROM {users} WHERE mail = '%s'", $search);
            }
            break;
          case 'users':
            $users[] = _drush_user_get_uid($search);  
            break;
        }
        if ($uid_query !== FALSE) {
          if ($uid = drush_db_result($uid_query)) {
            $users[] = $uid;
          }
          else {
            drush_set_error("Could not find a uid for $user_attr = $search");
          }
        }
      }
    }
  }
  return empty($users) ? FALSE : $users;
}
