<?php

/**
 * @file
 * Example tpl file for theming a single question-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 *
 * Helper variables:
 * - $question: The Question object this status is derived from
 */
?>

<div class="question-status">
  <?php print '<strong>Question Sample Data:</strong> ' . $question_sample_data = ($question_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>
