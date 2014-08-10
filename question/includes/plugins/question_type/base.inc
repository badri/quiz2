<?php

/**
 * @file
 * Abstract and interface plugin implementation.
 */

/**
 * Provide a separate Exception so it can be caught separately.
 */
class QuestionException extends Exception { }

/**
 * Basic interface for Quiz Question.
 *
 * EntityBundlePluginProvideFieldsInterface also mandates a fields() method.
 */
interface QuestionInterface extends EntityBundlePluginProvideFieldsInterface {

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
 * Quiz Question base class.
 *
 * All questions types should inherit this class
 */
abstract class QuestionBase extends Entity implements QuestionInterface, EntityBundlePluginValidableInterface {

  /**
   * The question id.
   *
   * @var integer
   */
  public $question_id;

  /**
   * The question type (bundle).
   *
   * @var string
   */
  public $type;

  /**
   * The uid of the question creator.
   *
   * @var integer
   */
  public $uid;

  /**
   * The date (unix timestamp) when the question was created.
   *
   * @var integer
   */
  public $created;

  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'question');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    // solution explanation text should be here.
  }

  /**
   * Implements QuestionInterface::evaluate().
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
}

/**
 * Broken implementation of a type plugin.
 * Used as a fallback when the actual type plugin can't be loaded.
 */
class QuestionBroken extends QuestionBase {

  /**
   * Throw an exception.
   */
  public function __construct($values = array(), $entityType = NULL) {
    throw new QuestionException('Attempted to instantiate a broken question type plugin');
  }

  /**
   * Implements EntityBundlePluginValidableInterface::isValid().
   */
  public static function isValid() {
    return FALSE;
  }
}
