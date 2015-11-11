<?php

/**
 * @file
 * Question editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class QuestionUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Questions',
      'description' => 'Add edit and update questions.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of questions.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a question',
      'description' => 'Add a new question',
      'page callback'  => 'question_add_page',
      'access callback'  => 'question_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'question.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (question_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'question_form_wrapper',
        'page arguments' => array(question_create(array('type' => $type->type))),
        'access callback' => 'question_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'question.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing question entities
    $items[$this->path . '/question/' . $wildcard] = array(
      'page callback' => 'question_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'question_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'question.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/question/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/question/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'question_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'question_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'question.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing questions
    $items['question/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'question_page_title',
      'title arguments' => array(1),
      'page callback' => 'question_page_view',
      'page arguments' => array(1),
      'access callback' => 'question_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Question Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('question_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: create or edit a question.
 *
 * @param $question
 *   The question object being edited by this form.
 *
 * @see question_edit_form()
 */
function question_form_wrapper($question) {
  // Add the breadcrumb for the form's location.
  question_set_breadcrumb();
  return drupal_get_form('question_edit_form', $question);
}


/**
 * Form callback wrapper: delete a question.
 *
 * @param $question
 *   The question object being edited by this form.
 *
 * @see question_edit_form()
 */
function question_delete_form_wrapper($question) {
  // Add the breadcrumb for the form's location.
  //question_set_breadcrumb();
  return drupal_get_form('question_delete_form', $question);
}


/**
 * Form callback: create or edit a question.
 *
 * @param $question
 *   The question object to edit or for a create form an empty question object
 *     with only a question type defined.
 */
function question_edit_form($form, &$form_state, $question) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Question Name'),
    '#default_value' => isset($question->name) ? $question->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Add the field related form elements.
  $form_state['question'] = $question;
  field_attach_form('question', $question, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save question'),
    '#submit' => $submit + array('question_edit_form_submit'),
  );

  if (!empty($question->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete question'),
      '#suffix' => l(t('Cancel'), 'admin/content/questions'),
      '#submit' => $submit + array('question_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'question_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the question form
 */
function question_edit_form_validate(&$form, &$form_state) {
  $question = $form_state['question'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('question', $question, $form, $form_state);
}


/**
 * Form API submit callback for the question form.
 *
 * @todo remove hard-coded link
 */
function question_edit_form_submit(&$form, &$form_state) {

  $question = entity_ui_controller('question')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the question and go back to the list of questions

  // Add in created and changed times.
  if ($question->is_new = isset($question->is_new) ? $question->is_new : 0){
    $question->created = time();
  }

  $question->changed = time();

  $question->save();
  $form_state['redirect'] = 'admin/content/questions';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function question_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/questions/question/' . $form_state['question']->question_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a question.
 *
 * @param $question
 *   The question to delete
 *
 * @see confirm_form()
 */
function question_delete_form($form, &$form_state, $question) {
  $form_state['question'] = $question;

  $form['#submit'][] = 'question_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete question %name?', array('%name' => $question->name)),
    'admin/content/questions/question',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for question_delete_form
 */
function question_delete_form_submit($form, &$form_state) {
  $question = $form_state['question'];

  question_delete($question);

  drupal_set_message(t('The question %name has been deleted.', array('%name' => $question->name)));
  watchdog('question', 'Deleted question %name.', array('%name' => $question->name));

  $form_state['redirect'] = 'admin/content/questions';
}



/**
 * Page to add Question Entities.
 *
 * @todo Pass this through a proper theme function
 */
function question_add_page() {
  $controller = entity_ui_controller('question');
  return $controller->addPage();
}


/**
 * Displays the list of available question types for question creation.
 *
 * @ingroup themeable
 */
function theme_question_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="question-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer question types')) {
      $output = '<p>' . t('Question Entities cannot be added because you have not created any question types yet. Go to the <a href="@create-question-type">question type creation page</a> to add a new question type.', array('@create-question-type' => url('admin/structure/question_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No question types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative question pages.
 */
function question_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Questions'), 'admin/content/questions'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
