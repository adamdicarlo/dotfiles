<?php
// $Id: view.inc,v 1.1.4.4 2010/10/09 17:34:52 mikeryan Exp $

/**
 * @file
 * Define a MigrateSource for importing from views
 */

/**
 * Implementation of MigrateSource, to handle imports from views. All that
 * should be necessary (once Views is ported to D7) is to pull the SQL query
 * from the view, the MigrateSourceSQL class can then just handle it like
 * hand-constructed queries.
 */
class MigrateSourceView extends MigrateSourceSQL {
  protected $viewName;

  public function __construct($view_name) {
    $this->viewName = $view_name;
    // TODO: Derive the query from the view
    parent::__construct($query);
  }
}
