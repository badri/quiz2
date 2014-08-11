<?php

/**
 * @file
 * Providing extra functionality for the Quiz UI via views.
 */


/**
 * Implements hook_views_data()
 */
function quiz_views_data_alter(&$data) {
  $data['quiz']['link_quiz'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the quiz.'),
      'handler' => 'quiz_handler_link_field',
    ),
  );
  $data['quiz']['edit_quiz'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the quiz.'),
      'handler' => 'quiz_handler_edit_link_field',
    ),
  );
  $data['quiz']['delete_quiz'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the quiz.'),
      'handler' => 'quiz_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows quizzes/quiz/%quiz_id/op
  $data['quiz']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this quiz.'),
      'handler' => 'quiz_handler_quiz_operations_field',
    ),
  );
}


/**
 * Implements hook_views_default_views().
 */
function quiz_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'quizzes';
  $view->description = 'A list of all quizzes';
  $view->tag = 'quizzes';
  $view->base_table = 'quiz';
  $view->human_name = 'Quizzes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Quizzes';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create any quiz type';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'quiz_id' => 'quiz_id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'quiz_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty ';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No quizzes have been created yet';
  /* Field: Quiz: Quiz ID */
  $handler->display->display_options['fields']['quiz_id']['id'] = 'quiz_id';
  $handler->display->display_options['fields']['quiz_id']['table'] = 'quiz';
  $handler->display->display_options['fields']['quiz_id']['field'] = 'quiz_id';
  $handler->display->display_options['fields']['quiz_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['quiz_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['quiz_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['quiz_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['quiz_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['quiz_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['quiz_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['quiz_id']['empty_zero'] = 0;
  /* Field: Quiz: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'quiz';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  /* Field: Quiz: Link */
  $handler->display->display_options['fields']['link_quiz']['id'] = 'link_quiz';
  $handler->display->display_options['fields']['link_quiz']['table'] = 'quiz';
  $handler->display->display_options['fields']['link_quiz']['field'] = 'link_quiz';
  $handler->display->display_options['fields']['link_quiz']['label'] = 'View';
  $handler->display->display_options['fields']['link_quiz']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link_quiz']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link_quiz']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link_quiz']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link_quiz']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link_quiz']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link_quiz']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link_quiz']['empty_zero'] = 0;
  /* Field: Quiz: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'quiz';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'quizzes_admin_page');
  $handler->display->display_options['path'] = 'admin/content/quizzes/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'tab';
  $handler->display->display_options['tab_options']['title'] = 'Quizzes';
  $handler->display->display_options['tab_options']['description'] = 'Manage quizzes';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['quizzes'] = array(
    t('Master'),
    t('Quizzes'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Empty '),
    t('No quizzes have been created yet'),
    t('Quiz ID'),
    t('.'),
    t(','),
    t('Name'),
    t('View'),
    t('Operations links'),
    t('Page'),
  );
  $views[] = $view;
  return $views;

}
