<?php
// $Id: term.inc,v 1.1.4.12 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * Support for taxonomy term destinations.
 */

// TODO:
// Make sure this works with updates, explicit destination keys
// taxonomy_term_save() is doing a cache_clear_all and an automatic insertion for parent.

/**
 * Destination class implementing migration into terms.
 */
class MigrateDestinationTerm extends MigrateDestinationEntity {
  static public function getKeySchema() {
    return array(
      'tid' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => TRUE,
        'description' => 'ID of destination term',
      ),
    );
  }

  /**
   * Basic initialization
   */
  public function __construct($bundle, $language = NULL, $text_format = NULL) {
    parent::__construct('taxonomy_term', $bundle, $language, $text_format);
  }

  /**
   * Returns a list of fields available to be mapped for this vocabulary (bundle)
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  public function fields() {
    $fields = array();
    // First the core (taxonomy_term_data table) properties
    $fields['tid'] = t('Term: Existing term ID');
    $fields['name'] = t('Term: Name');
    $fields['description'] = t('Term: Description');
    $fields['format'] = t('Term: Format');
    $fields['weight'] = t('Term: Weight');
    // TODO: Remove parent_name, implement via arguments
    $fields['parent_name'] = t('Term: Parent (by name)');
    $fields['parent'] = t('Term: Parent (by Drupal term ID)');

    // Then add in anything provided by handlers
    $fields += migrate_handler_invoke_all('Term', 'fields', $this->entityType, $this->bundle);

    return $fields;
  }

  /**
   * Delete a migrated term
   *
   * @param $ids
   *  Array of fields representing the key (in this case, just tid).
   */
  public function rollback(array $key) {
    $tid = reset($key);

    migrate_instrument_start('taxonomy_term_delete');
    $result = (bool) taxonomy_del_term($tid);
    migrate_instrument_stop('taxonomy_term_delete');
    return $result == SAVED_DELETED;
  }

  /**
   * Import a single term.
   *
   * @param $term
   *  Term object to build. Prefilled with any fields mapped in the Migration.
   * @param $row
   *  Raw source data object - passed through to prepare/complete handlers.
   * @return array
   *  Array of key fields (tid only in this case) of the term that was saved if
   *  successful. FALSE on failure.
   */
  public function import(stdClass $term, stdClass $row) {
    $migration = Migration::currentMigration();
    // Updating previously-migrated content?
    if (isset($row->migrate_map_destid1)) {
      $term->tid = $row->migrate_map_destid1;
      if (isset($term->tid)) {
        if ($term->tid != $row->migrate_map_destid1) {
          throw new MigrateException(t("Incoming tid !tid and map destination nid !destid1 don't match",
            array('!tid' => $term->tid, '!destid1' => $row->migrate_map_destid1)));
        }
      }
      else {
        $term->tid = $row->migrate_map_destid1;
      }
    }
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      if (!isset($term->tid)) {
        throw new MigrateException(t('System-of-record is DESTINATION, but no destination tid provided'));
      }
      $rawterm = $term;
      $this->prepare($term, $row);
      $old_term = taxonomy_term_load($term->tid);
      foreach ($rawterm as $field => $value) {
        $old_term->$field = $term->$field;
      }
      $parents = taxonomy_get_parents($term->tid);
      if ($parents) {
        $parent = array_pop($parents);
        $old_term->parent = $parent->tid;
      }
      else {
        $old_term->parent = 0;
      }
      $term = $old_term;
    }
    else {
      // Default to bundle if no vocabulary machine name provided
      if (!isset($term->vocabulary_machine_name)) {
        $term->vocabulary_machine_name = $this->bundle;
      }
      // vid is required
      if (empty($term->vid)) {
        static $vocab_map = array();
        if (!isset($vocab_map[$term->vocabulary_machine_name])) {
          $vid = db_select('vocabulary', 'v')
                 ->fields('v', array('vid'))
                 ->condition('name', $term->vocabulary_machine_name)
                 ->execute()
                 ->fetchField();
          if ($vid) {
            $vocab_map[$term->vocabulary_machine_name] = $vid;
          }
          else {
            $migration->saveMessage(t('No vocabulary found with machine_name !name',
              array('!name' => $term->vocabulary_machine_name)));
            return FALSE;
          }
        }
        $term->vid = $vocab_map[$term->vocabulary_machine_name];
      }
      // Look up parent name if provided
      if (isset($term->parent_name)) {
        // Look for the name in the same vocabulary.
        // Note that hierarchies may have multiples of the same name...
        $terms = taxonomy_get_term_by_name(trim($term->parent_name));
        foreach ($terms as $candidate) {
          if ($candidate->vid == $term->vid) {
            $term->parent = $candidate->tid;
            break;
          }
        }
        unset($term->parent_name);
      }
      if (!isset($term->parent)) {
        $term->parent = 0;
      }
      if (!isset($term->format)) {
        $term->format = $this->textFormat;
      }

      $this->prepare($term, $row);
    }

    // Trying to update an existing term
    if ($migration->getSystemOfRecord() == Migration::DESTINATION) {
      $existing_term = taxonomy_term_load($term->tid);
      if ($existing_term) {
        // Incoming data overrides existing data, so only copy non-existent fields
        foreach ($existing_term as $field => $value) {
          if (!isset($term->$field)) {
            $term->$field = $existing_term->$field;
          }
        }
      }
    }

    migrate_instrument_start('taxonomy_term_save');
    $term = (array)$term;
    $status = taxonomy_save_term($term);
    $term = (object)$term;
    migrate_instrument_stop('taxonomy_term_save');
    $this->complete($term, $row);
    $return = isset($term->tid) ? array($term->tid) : FALSE;
    return $return;
  }
}

/**
 * Handler to process term assignments for nodes
 */
class MigrateTermNodeHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('node'));
  }

  public function prepare(stdClass $node, stdClass $row) {
    // Identify vocabularies for this node type
    $vocabs = taxonomy_get_vocabularies($node->type);
    foreach ($vocabs as $vid => $vocab) {
      $vocab_name = $vocab->name;
      if (isset($node->$vocab_name)) {
        $vocab_values = $node->$vocab_name;
        if (!is_array($vocab_values)) {
          $vocab_values = array($vocab_values);
        }
        $by_tid = FALSE;
        if (isset($vocab_values['arguments']['source_type'])) {
          if ($vocab_values['arguments']['source_type'] == 'tid') {
            $by_tid = TRUE;
          }
        }
        unset($vocab_values['arguments']);
        foreach ($vocab_values as $term_value) {
          if ($by_tid) {
            $node->taxonomy[$term_value] = $term_value;
          }
          else {
            foreach (taxonomy_get_term_by_name($term_value) as $term) {
              if ($term->vid == $vid) {
                $node->taxonomy[$term->tid] = $term->tid;
              }
            }
          }
        }
      }
    }
  }
}
