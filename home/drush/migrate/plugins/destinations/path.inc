<?php
// $Id: path.inc,v 1.1.4.5 2010/07/03 17:07:04 mikeryan Exp $

/**
 * @file
 * Support for paths in core Drupal objects
 */

class MigratePathNodeHandler extends MigrateDestinationHandler {
  public function __construct() {
    $this->registerTypes(array('node'));
  }

  public function fields() {
    return array('path' => t('Node: Path alias'));
  }
}
