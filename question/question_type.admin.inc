<?php

/**
 * @file
 * Question type editing UI.
 */

/**
 * UI controller.
 */
class QuestionTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage question entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the question type editing form.
 */
function question_type_form($form, &$form_state, $question_type, $op = 'edit') {

  if ($op == 'clone') {
    $question_type->label .= ' (cloned)';
    $question_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $question_type->label,
    '#description' => t('The human-readable name of this question type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($question_type->type) ? $question_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $question_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'question_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this question type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save question type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function question_type_form_submit(&$form, &$form_state) {
  $question_type = entity_ui_form_submit_build_entity($form, $form_state);
  $question_type->save();
  $form_state['redirect'] = 'admin/structure/question_types';
}

/**
 * Form API submit callback for the delete button.
 */
function question_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/question_types/manage/' . $form_state['question_type']->type . '/delete';
}
