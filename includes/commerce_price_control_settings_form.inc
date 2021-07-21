<?php

function commerce_price_control_settings_form($form, &$form_state) {
  $query = db_select('commerce_price_control');
  $query->fields(
    'commerce_price_control',
    [
      'vocabulary_id',
      'vocabulary_name',
      'brand_field_label',
      'brand_field_machine_name',
    ]
  );
  $commerce_price_control_results = $query->execute()->fetchAll();

  $vocabularies = taxonomy_get_vocabularies();
  $vocabularies_vid_and_name = [0 => t('Выберите Cловарь')];
  foreach ($vocabularies as $vocabulary) {
    $vocabulary_vid = $vocabulary->vid;
    $vocabulary_name = $vocabulary->name;
    $vocabularies_vid_and_name[$vocabulary_vid] = $vocabulary->name;
  }

  $form = [
    '#method' => 'post',
  ];

  $form['row-first'] = [
    '#type' => 'fieldset',
  ];
  $form['row-first']['vocabulary'] = [
    '#title' => t('Словарь'),
    '#description' => t('Выбор Словаря Каталога'),
    '#type' => 'select',
    '#options' => $vocabularies_vid_and_name,
    '#default_value' => (isset($commerce_price_control_results) && (is_array(
      $commerce_price_control_results
    )) && (count(
        $commerce_price_control_results
      ) > 0) && (isset($commerce_price_control_results[0])) ? trim(
      $commerce_price_control_results[0]->vocabulary_id
    ) : ''),
    '#required' => TRUE,
  ];

  $product_display_fields = field_info_instances("node", "product_display");
  $brands_field_machine_name_and_label = [0 => t('Выберите Поле Бренда')];
  foreach ($product_display_fields as $product_display_field_key => $product_display_field_value) {
    $brands_field_machine_name_and_label[$product_display_field_key] = $product_display_field_value['label'];
  }

  $form['row-second'] = [
    '#type' => 'fieldset',
  ];
  $form['row-second']['brand_filed'] = [
    '#title' => t('Поле Бренда'),
    '#description' => t('Выбор Поля Бренда'),
    '#type' => 'select',
    '#options' => $brands_field_machine_name_and_label,
    '#default_value' => (isset($commerce_price_control_results) && (is_array(
      $commerce_price_control_results
    )) && (count(
        $commerce_price_control_results
      ) > 0) && (isset($commerce_price_control_results[0])) ? trim(
      $commerce_price_control_results[0]->brand_field_machine_name
    ) : ''),
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Сохранить'),
  ];

  if (isset($form_state['result'])) {
    if ($form_state['result'] == 'parameters_error') {
      drupal_set_message(t('Не все параметры были получены'), 'error', FALSE);
    }
    elseif ($form_state['result'] == 'successfully') {
      drupal_set_message(t('Изменения успешно внесены'), 'status', FALSE);
    }
    else {
      drupal_set_message(t('Неизвестный результат'), 'warning', FALSE);
    }
  }

  return $form;
}

function commerce_price_control_settings_form_submit($form, &$form_state) {
  if (isset($form_state['values']['vocabulary']) && !empty($form_state['values']['vocabulary'])
    && isset($form_state['values']['brand_filed']) && !empty($form_state['values']['brand_filed'])) {
    $vocabulary_id = trim($form_state['values']['vocabulary']);

    $vocabulary = taxonomy_vocabulary_load($vocabulary_id);
    $vocabulary_name = trim($vocabulary->name);

    $brand_field_machine_name = trim($form_state['values']['brand_filed']);

    $field_info = field_info_instance(
      'node',
      $brand_field_machine_name,
      'product_display'
    );
    $brand_field_label = trim($field_info['label']);

    $query = db_truncate('commerce_price_control');
    $query->execute();

    $query = db_insert('commerce_price_control');
    $query->fields(
      [
        'vocabulary_id' => $vocabulary_id,
        'vocabulary_name' => $vocabulary_name,
        'brand_field_label' => $brand_field_label,
        'brand_field_machine_name' => $brand_field_machine_name,
      ]
    );
    $query->execute();

    $form_state['rebuild'] = TRUE;
    $form_state['result'] = 'successfully';
  }
  else {
    $form_state['rebuild'] = TRUE;
    $form_state['result'] = 'parameters_error';
  }
}
