<?php

/**
 * @file
 * Abstract and interface plugin implementation.
 */

/**
 * Provide a separate Exception so it can be caught separately.
 */
class QuestionAttemptException extends Exception { }

/**
 * Basic interface for Quiz QuestionAttempt.
 *
 * EntityBundlePluginProvideFieldsInterface also mandates a fields() method.
 */
interface QuestionAttemptInterface extends EntityBundlePluginProvideFieldsInterface {

  /**
   * Returns whether a question response is correct or not
   *
   * @return
   *   TRUE if given question evaluates to true, FALSE otherwise.
   */
  public function evaluate();

  //TODO: write scoring function or any other common question type interfaces here.
}

/**
 * Quiz QuestionAttempt base class.
 *
 * All questions types should inherit this class
 */
abstract class QuestionAttemptBase extends Entity implements QuestionAttemptInterface, EntityBundlePluginValidableInterface {

  /**
   * The question attempt id.
   *
   * @var integer
   */
  public $question_attempt_id;

  /**
   * The question attempt type (bundle).
   *
   * @var string
   */
  public $type;

  /**
   * The uid of the question attempter.
   *
   * @var integer
   */
  public $uid;

  /**
   * The question_id of the question being attempted.
   *
   * @var integer
   */
  public $question;

  /**
   * The date (unix timestamp) when the question was attempted.
   *
   * @var integer
   */
  public $attempt_time;

  /**
   * Question Attempt metadata wrapper.
   *
   * @var EntityDrupalWrapper
   */
  public $wrapper;


  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'question_attempt');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    // solution explanation text should be here.
    //TODO: remove this
  }

  /**
   * Implements QuestionAttemptInterface::evaluate().
   */
  public function evaluate() {
    return FALSE;
  }


  /**
   * Overrides Entity::save().
   */
  public function save() {
    //TODO: any overriding of question save logic goes here.
    parent::save();
  }

  /**
   * Implements EntityBundlePluginValidableInterface::isValid().
   */
  public static function isValid() {
    return TRUE;
  }
}

/**
 * Broken implementation of a type plugin.
 * Used as a fallback when the actual type plugin can't be loaded.
 */
class QuestionAttemptBroken extends QuestionAttemptBase {

  /**
   * Throw an exception.
   */
  public function __construct($values = array(), $entityType = NULL) {
    throw new QuestionAttemptException('Attempted to instantiate a broken question type plugin');
  }
}
