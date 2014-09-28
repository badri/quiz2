<?php

/**
 * @file
 * Provides a question type plugin.
 */

$plugin = array(
  'title' => t('Multichoice Question'),
  'class' => 'MultichoiceQuestion',
  'weight' => 1,
);

/**
 * Multichoice Question type.
 */
class MultichoiceQuestion extends QuestionBase  {

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    $fields = parent::fields();

    // A real project could use commerce_single_address and just call
    // commerce_single_address_active_profile_load() to get the current
    // billing profile and all the information within, including the customer
    // name.
    $fields['cle_name']['field'] = array(
      'type' => 'text',
      'cardinality' => 1,
    );
    $fields['cle_name']['instance'] = array(
      'label' => t('Choice'),
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
      ),
      'widget' => array(
        'module' => 'text',
        'type' => 'text_textfield',
        'settings' => array(
          'size' => 20,
        ),
      ),
    );

    return $fields;
  }


  /**
   * Implements QuestionBase::render().
   */
  public function render() {
    print("multichoice rendered");
    return TRUE;
  }
}
