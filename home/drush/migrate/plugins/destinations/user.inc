<?php
// $Id: user.inc,v 1.1.4.9 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * Support for user destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys
// Speed up password generation a ton: $conf['password_count_log2'] = 1;

/**
 * Destination class implementing migration into users.
 */
class MigrateDestinationUser extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'uid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'ID of destination user',
      ),
    );
  }

  /**
   * Basic initialization
   */
  public function __construct($language = NULL, $text_format = NULL) {
    parent::__construct('user', 'user', $language, $text_format);
  }

  /**
   * Returns a list of fields available to be mapped for users
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    // First the core (users table) properties
    $fields['uid'] = t('User: Existing user ID');
    $fields['name'] = t('User: Username');
    $fields['pass'] = t('User: Password (plain text)');
    $fields['mail'] = t('User: Email address');
    $fields['roles'] = t('User: Roles');
    $fields['theme'] = t('User: Default theme');
    $fields['signature'] = t('User: Signature');
    $fields['signature_format'] = t('User: Signature format');
    $fields['created'] = t('User: Registered timestamp');
    $fields['access'] = t('User: Last access timestamp');
    $fields['login'] = t('User: Last login timestamp');
    $fields['status'] = t('User: Status');
    $fields['timezone'] = t('User: Timezone');
    $fields['language'] = t('User: Language');
    $fields['picture'] = t('User: Picture');
    $fields['init'] = t('User: Init');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('User', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Delete a batch of users at once.
   *
   * @param $uids
   *  Array of user IDs to be deleted.
   */
  public function bulkRollback(array $uids) {
    migrate_instrument_start('user_delete_multiple');
    user_delete_multiple($uids);
    migrate_instrument_stop('user_delete_multiple');
  }

  /**
   * Import a single user.
   *
   * @param $account
   *  Account object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (uid only in this case) of the user that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $account, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      $account->uid = $row->migrate_map_destid1;
      if (isset($account->uid)) {
        if ($account->uid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming uid !uid and map destination uid !destid1 don't match",
            array('!uid' => $account->uid, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $account->uid = $row->migrate_map_destid1;
      }
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($account->uid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination uid provided'));
      }
    }

    $this->prepare($account, $row);
    migrate_instrument_start('user_save');
    $newaccount = user_save($account, (array)$account);
    migrate_instrument_stop('user_save');
    if ($newaccount) {
      $this->complete($newaccount, $row);
      $return = isset($newaccount->uid) ? array($newaccount->uid) : FALSE;
    }
    else {
      $return = FALSE;
    }
    return $return;
  }
}
