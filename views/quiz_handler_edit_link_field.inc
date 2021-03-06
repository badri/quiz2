<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class quiz_handler_edit_link_field extends quiz_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy quiz to check access against
    $dummy_quiz = (object) array('type' => $type);
    if (!quiz_access('edit', $dummy_quiz)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $quiz_id = $values->{$this->aliases['quiz_id']};

    return l($text, 'admin/content/quizzes/quiz/' . $quiz_id . '/edit');
  }
}
