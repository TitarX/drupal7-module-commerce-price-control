<?php

/* Возвращает taxonomy id терминов не имеющих потомков : */
function get_all_last_child_tids($tid, $name, &$child_terms_tids) {
  $tid_child_tids = taxonomy_get_children($tid);
  if (isset($tid_child_tids) && (count($tid_child_tids) > 0)) {
    foreach ($tid_child_tids as $tid_child_tid) {
      get_all_last_child_tids(
        $tid_child_tid->tid,
        $tid_child_tid->name,
        $child_terms_tids
      );
    }
  }
  else {
    $child_terms_tids[$tid] = $name;
  }
}

function get_all_last_child_tids_by_taxonomy_vocabulary_id($vocabulary_id) {
  $voc = taxonomy_vocabulary_load($vocabulary_id);
  $voc_terms = taxonomy_get_tree($voc->vid);
  $child_terms_tids = [];
  foreach ($voc_terms as $voc_term) {
    get_all_last_child_tids($voc_term->tid, $voc_term->name, $child_terms_tids);
  }

  return $child_terms_tids;
}

function get_all_last_child_tids_by_taxonomy_vocabulary_machine_name(
  $vocabulary_machine_name
) {
  $voc = taxonomy_vocabulary_machine_name_load($vocabulary_machine_name);
  $voc_terms = taxonomy_get_tree($voc->vid);
  $child_terms_tids = [];
  foreach ($voc_terms as $voc_term) {
    get_all_last_child_tids($voc_term->tid, $voc_term->name, $child_terms_tids);
  }

  return $child_terms_tids;
}
