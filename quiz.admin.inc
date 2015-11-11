<?php

/**
 * @file
 * Quiz editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class QuizUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Quizzes',
      'description' => 'Add edit and update quizzes.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of quizzes.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a quiz',
      'description' => 'Add a new quiz',
      'page callback'  => 'quiz_add_page',
      'access callback'  => 'quiz_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'quiz.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (quiz_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'quiz_form_wrapper',
        'page arguments' => array(quiz_create(array('type' => $type->type))),
        'access callback' => 'quiz_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'quiz.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing quiz entities
    $items[$this->path . '/quiz/' . $wildcard] = array(
      'page callback' => 'quiz_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'quiz_access',
      'access arguments' => array('edit', $id_count + 1),
      'weight' => 0,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'quiz.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    $items[$this->path . '/quiz/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $items[$this->path . '/quiz/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'quiz_delete_form_wrapper',
      'page arguments' => array($id_count + 1),
      'access callback' => 'quiz_access',
      'access arguments' => array('edit', $id_count + 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_INLINE,
      'weight' => 10,
      'file' => 'quiz.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Menu item for viewing quizzes
    $items['quiz/' . $wildcard] = array(
      //'title' => 'Title',
      'title callback' => 'quiz_page_title',
      'title arguments' => array(1),
      'page callback' => 'quiz_page_view',
      'page arguments' => array(1),
      'access callback' => 'quiz_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );
    return $items;
  }


  /**
   * Create the markup for the add Quiz Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('quiz_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: create or edit a quiz.
 *
 * @param $quiz
 *   The quiz object being edited by this form.
 *
 * @see quiz_edit_form()
 */
function quiz_form_wrapper($quiz) {
  // Add the breadcrumb for the form's location.
  quiz_set_breadcrumb();
  return drupal_get_form('quiz_edit_form', $quiz);
}


/**
 * Form callback wrapper: delete a quiz.
 *
 * @param $quiz
 *   The quiz object being edited by this form.
 *
 * @see quiz_edit_form()
 */
function quiz_delete_form_wrapper($quiz) {
  // Add the breadcrumb for the form's location.
  //quiz_set_breadcrumb();
  return drupal_get_form('quiz_delete_form', $quiz);
}


/**
 * Form callback: create or edit a quiz.
 *
 * @param $quiz
 *   The quiz object to edit or for a create form an empty quiz object
 *     with only a quiz type defined.
 */
function quiz_edit_form($form, &$form_state, $quiz) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Quiz Name'),
    '#default_value' => isset($quiz->name) ? $quiz->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );

  // Add the field related form elements.
  $form_state['quiz'] = $quiz;
  field_attach_form('quiz', $quiz, $form, $form_state);

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
    '#value' => t('Save quiz'),
    '#submit' => $submit + array('quiz_edit_form_submit'),
  );

  if (!empty($quiz->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete quiz'),
      '#suffix' => l(t('Cancel'), 'admin/content/quizzes'),
      '#submit' => $submit + array('quiz_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'quiz_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the quiz form
 */
function quiz_edit_form_validate(&$form, &$form_state) {
  $quiz = $form_state['quiz'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('quiz', $quiz, $form, $form_state);
}


/**
 * Form API submit callback for the quiz form.
 *
 * @todo remove hard-coded link
 */
function quiz_edit_form_submit(&$form, &$form_state) {

  $quiz = entity_ui_controller('quiz')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the quiz and go back to the list of quizzes

  // Add in created and changed times.
  if ($quiz->is_new = isset($quiz->is_new) ? $quiz->is_new : 0){
    $quiz->created = time();
  }

  $quiz->changed = time();

  $quiz->save();
  $form_state['redirect'] = 'admin/content/quizzes';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function quiz_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/quizzes/quiz/' . $form_state['quiz']->quiz_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a quiz.
 *
 * @param $quiz
 *   The quiz to delete
 *
 * @see confirm_form()
 */
function quiz_delete_form($form, &$form_state, $quiz) {
  $form_state['quiz'] = $quiz;

  $form['#submit'][] = 'quiz_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete quiz %name?', array('%name' => $quiz->name)),
    'admin/content/quizzes/quiz',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for quiz_delete_form
 */
function quiz_delete_form_submit($form, &$form_state) {
  $quiz = $form_state['quiz'];

  quiz_delete($quiz);

  drupal_set_message(t('The quiz %name has been deleted.', array('%name' => $quiz->name)));
  watchdog('quiz', 'Deleted quiz %name.', array('%name' => $quiz->name));

  $form_state['redirect'] = 'admin/content/quizzes';
}



/**
 * Page to add Quiz Entities.
 *
 * @todo Pass this through a proper theme function
 */
function quiz_add_page() {
  $controller = entity_ui_controller('quiz');
  return $controller->addPage();
}


/**
 * Displays the list of available quiz types for quiz creation.
 *
 * @ingroup themeable
 */
function theme_quiz_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="quiz-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer quiz types')) {
      $output = '<p>' . t('Quiz Entities cannot be added because you have not created any quiz types yet. Go to the <a href="@create-quiz-type">quiz type creation page</a> to add a new quiz type.', array('@create-quiz-type' => url('admin/structure/quiz_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No quiz types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative quiz pages.
 */
function quiz_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Content'), 'admin/content'),
    l(t('Quizzes'), 'admin/content/quizzes'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
