<?php

/**
 * @file
 * Quiz type editing UI.
 */

/**
 * UI controller.
 */
class QuizTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage quiz entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the quiz type editing form.
 */
function quiz_type_form($form, &$form_state, $quiz_type, $op = 'edit') {

  if ($op == 'clone') {
    $quiz_type->label .= ' (cloned)';
    $quiz_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $quiz_type->label,
    '#description' => t('The human-readable name of this quiz type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($quiz_type->type) ? $quiz_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $quiz_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'quiz_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this quiz type. It must only contain lowercase letters, numbers, and underscores.'),
  );


  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save quiz type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function quiz_type_form_submit(&$form, &$form_state) {
  $quiz_type = entity_ui_form_submit_build_entity($form, $form_state);
  $quiz_type->save();
  $form_state['redirect'] = 'admin/structure/quiz_types';
}

/**
 * Form API submit callback for the delete button.
 */
function quiz_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/quiz_types/manage/' . $form_state['quiz_type']->type . '/delete';
}
