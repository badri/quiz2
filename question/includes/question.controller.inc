<?php

/**
 * @file
 * Controller for the quiz question entity type.
 */

class QuestionEntityController extends EntityBundlePluginEntityController {

  /**
   * Overrides EntityBundlePluginEntityController::delete().
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $entities = $ids ? $this->load($ids) : FALSE;
    if (!$entities) {
      // Do nothing, in case invalid or no ids have been passed.
      return;
    }

    try {
      parent::delete($ids, $transaction);

      //TODO: if the question is used elsewhere, don't delete.
    }
    catch (Exception $e) {
      if (isset($transaction)) {
        $transaction->rollback();
      }
      throw $e;
    }
  }
}
