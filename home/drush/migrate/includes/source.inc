<?php
// $Id: source.inc,v 1.1.4.5 2010/10/09 17:34:51 mikeryan Exp $

/**
 * @file
 * Define base for migration sources.
 */

/**
 * Abstract base class for source handling.
 *
 * Derived classes are expected to define __toString(), returning a string
 * describing the source and significant options. See
 * MigrateSourceSQL for an example.
 */
abstract class MigrateSource implements Iterator {
  /**
   * The current row from the quey
   *
   * @var stdClass
   */
  protected $currentRow;

  /**
   * Number of eligible rows processed so far (used for itemlimit checking)
   *
   * @var int
   */
  protected $numProcessed = 0;

  /**
   * The primary key of the current row
   *
   * @var array
   */
  protected $currentKey;
  public function getCurrentKey() {
    return $this->currentKey;
  }

  /**
   * Derived classes must implement fields(), returning a list of available
   * source fields.
   *
   * @return array
   *  Keys: machine names of the fields (to be passed to addFieldMapping)
   *  Values: Human-friendly descriptions of the fields.
   */
  abstract public function fields();

  /**
   * Derived classes must implement count(), returning a count of all available
   * source records. If the count is cached, it must be refreshed when TRUE is passed.
   */
  abstract public function count($refresh = FALSE);

  public function __construct() {}

  /**
   * Default implementations of Iterator methods - many derivations will find
   * these adequate and will only need to implement rewind() and next()
   */

  /**
   * Implementation of Iterator::current() - called when entering a loop
   * iteration, returning the current row
   */
  public function current() {
    return $this->currentRow;
  }

  /**
   * Implementation of Iterator::key - called when entering a loop iteration, returning
   * the key of the current row. It must be a scalar - we will serialize
   * to fulfill the requirement, but using getCurrentKey() is preferable.
   */
  public function key() {
    return serialize($this->currentKey);
  }

  /**
   * Implementation of Iterator::valid() - called at the top of the loop, returning
   * TRUE to process the loop and FALSE to terminate it
   */
  public function valid() {
    return !is_null($this->currentRow);
  }
}
