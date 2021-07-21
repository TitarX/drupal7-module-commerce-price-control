<?php

function commerce_price_control_form($form, &$form_state) {
  drupal_add_js(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/js/jquery.confirm/jquery.confirm.min.js'
  );
  drupal_add_js(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/js/jquery.confirm/bootstrap.min.js'
  );
  drupal_add_js(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/js/commerce_price_control.js'
  );
  drupal_add_css(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/css/jquery.confirm/jquery.confirm.css'
  );
  drupal_add_css(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/css/commerce_price_control.css'
  );

  $form = [
    '#method' => 'post',
    '#attributes' => ['class' => ['price-changing-form']],
  ];

  $query = db_select('commerce_price_control');
  $query->fields(
    'commerce_price_control',
    ['vocabulary_id', 'brand_field_machine_name']
  );
  $commerce_price_control_results = $query->execute()->fetchAll();

  if ((isset($commerce_price_control_results)) && (is_array(
      $commerce_price_control_results
    )) && (count($commerce_price_control_results) > 0)) {
    $vocabulary_id = trim($commerce_price_control_results[0]->vocabulary_id);
    $brand_field_machine_name = $commerce_price_control_results[0]->brand_field_machine_name;

    $termins_array = get_all_last_child_tids_by_taxonomy_vocabulary_id(
      $vocabulary_id
    );
    $termins_array = array_reverse($termins_array, TRUE);
    $termins_array['all'] = t('Все');
    $termins_array = array_reverse($termins_array, TRUE);

    $field_info = field_info_field($brand_field_machine_name);
    $brands_array = $field_info['settings']['allowed_values'];
    $brands_array = array_reverse($brands_array, TRUE);
    $brands_array['all'] = t('Все');
    $brands_array = array_reverse($brands_array, TRUE);

    $form['row-first'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-row']],
    ];
    $form['row-first']['row-first-cell-first'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-first']['row-first-cell-first']['brand'] = [
      '#title' => t('Бренд: '),
      '#description' => t(''),
      '#type' => 'select',
      '#options' => $brands_array,
      '#default_value' => (isset($form_state['values']['brand']) && (!empty($form_state['values']['brand'])) ? trim(
        $form_state['values']['brand']
      ) : 'all'),
    ];
    $form['row-first']['row-first-cell-second'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-first']['row-first-cell-second']['catalog'] = [
      '#title' => t('Каталог: '),
      '#description' => t(''),
      '#type' => 'select',
      '#options' => $termins_array,
      '#default_value' => (isset($form_state['values']['catalog']) && (!empty($form_state['values']['catalog'])) ? trim(
        $form_state['values']['catalog']
      ) : 'all'),
    ];

    $form['row-second'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-row']],
    ];
    $form['row-second']['row-second-cell-first'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-second']['row-second-cell-first']['operand_type'] = [
      '#title' => t('Тип операнда: '),
      '#description' => t(''),
      '#type' => 'select',
      '#options' => [
        'value' => 'Значение',
        'percent' => 'Процент',
      ],
      '#default_value' => (isset($form_state['values']['operand_type']) && (!empty($form_state['values']['operand_type'])) ? trim(
        $form_state['values']['operand_type']
      ) : 'value'),
    ];
    $form['row-second']['row-second-cell-second'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-second']['row-second-cell-second']['operand'] = [
      '#title' => t('Операнд: '),
      '#description' => t(''),
      '#type' => 'textfield',
      '#default_value' => (isset($form_state['values']['operand']) && !empty($form_state['values']['operand']) ? trim(
        $form_state['values']['operand']
      ) : ''),
      '#attributes' => ['min' => '1', 'max' => '9999999'],
      '#size' => 10,
      '#maxlength' => 7,
    ];

    $form['row-third'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-row']],
    ];
    $form['row-third']['row-third-cell-first'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-third']['row-third-cell-first']['operation'] = [
      '#title' => t('Операция: '),
      '#description' => t(''),
      '#type' => 'select',
      '#options' => [
        'addition' => 'Сложение',
        'subtraction' => 'Вычитание',
        'multiplication' => 'Умножение',
        'division' => 'Деление',
        'replacement' => 'Замена',
      ],
      '#default_value' => (isset($form_state['values']['operation']) && (!empty($form_state['values']['operation'])) ? trim(
        $form_state['values']['operation']
      ) : 'addition'),
    ];
    $form['row-third']['row-third-cell-second'] = [
      '#type' => 'fieldset',
      '#attributes' => ['class' => ['price-changing-form-cell']],
    ];
    $form['row-third']['row-third-cell-second']['submit'] = [
      '#type' => 'submit',
      '#value' => t('Изменить цены'),
      '#attributes' => ['class' => ['ts-button', 'price-change-submit-button']],
    ];
  }
  else {
    drupal_set_message(
      t(
        'Укажите Словарь Каталога и Поле Бренда, в разделе: Магазин -> Конфигурация -> Управление ценами -> Настройка массового изменения цен'
      ),
      'warning',
      FALSE
    );
  }

  if (isset($form_state['result'])) {
    if ($form_state['result'] == 'parameters_error') {
      drupal_set_message(t('Не все параметры были получены'), 'error', FALSE);
    }
    elseif ($form_state['result'] == 'numeric_error') {
      drupal_set_message(
        t('Поле "Операнд" может содержать только цифры'),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'numeric_range_error') {
      drupal_set_message(
        t(
          'Поле "Операнд" должно содержать только число в диапазоне от 1 до 9999999'
        ),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'products_not_found') {
      drupal_set_message(t('Не найдено ни одного продукта'), 'warning', FALSE);
    }
    elseif ($form_state['result'] == 'field_is_not_correct') {
      drupal_set_message(
        t('Поле "Тип операнда" имеет некорректное значение'),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'division_by_zero') {
      drupal_set_message(t('Ошибка: Деление на ноль'), 'error', FALSE);
    }
    elseif ($form_state['result'] == 'product_change_not_attempt') {
      drupal_set_message(
        t('Не удалось попытаться изменить цену на на один продукт'),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'decimal_value_not_valid') {
      drupal_set_message(
        t(
          'Параметр количества знаков после запятой имеет некорректное значение'
        ),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'current_currency_error') {
      drupal_set_message(
        t('Не определена валюта по умолчанию'),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'successfully') {
      ctools_include('object-cache');

      $successfully_message = ctools_object_cache_get(
        'commerce_price_control',
        'successfully_message'
      );
      $incorrect_price_value_product_count_message = ctools_object_cache_get(
        'commerce_price_control',
        'incorrect_price_value_product_count_message'
      );

      if (isset($successfully_message)) {
        drupal_set_message(t($successfully_message), 'status', FALSE);
        ctools_object_cache_clear(
          'commerce_price_control',
          'successfully_message'
        );
      }
      if (isset($incorrect_price_value_product_count_message)) {
        drupal_set_message(
          t($incorrect_price_value_product_count_message),
          'warning',
          FALSE
        );
        ctools_object_cache_clear(
          'commerce_price_control',
          'incorrect_price_value_product_count_message'
        );
      }
    }
    else {
      drupal_set_message(t('Неизвестный результат'), 'warning', FALSE);
    }
  }

  return $form;
}

function commerce_price_control_form_submit($form, &$form_state) {
  if (isset($form_state['values']['operand']) && !empty($form_state['values']['operand'])
    && isset($form_state['values']['brand']) && !empty($form_state['values']['brand'])
    && isset($form_state['values']['catalog']) && !empty($form_state['values']['catalog'])
    && isset($form_state['values']['operand_type']) && !empty($form_state['values']['operand_type'])
    && isset($form_state['values']['operation']) && !empty($form_state['values']['operation'])) {
    $operand_par = trim($form_state['values']['operand']);

    if (is_numeric($operand_par)) {
      $operand = intval($operand_par);
      if (($operand > 0) && ($operand < 9999999)) {
        $price_changing_select_brand = trim($form_state['values']['brand']);
        $price_changing_select_catalog = trim($form_state['values']['catalog']);
        $price_changing_select_operand_type = trim(
          $form_state['values']['operand_type']
        );
        $price_changing_select_operation = trim(
          $form_state['values']['operation']
        );

        $view_arguments = [];
        array_push($view_arguments, $price_changing_select_brand);
        array_push($view_arguments, $price_changing_select_catalog);

        $view = views_get_view('price_changing');
        $view->set_display('block');
        $view->set_arguments($view_arguments);
        $view->execute();
        $response = $view->result;

        //Массив id товаров, под изменение цены
        $product_id_array = [];
        foreach ($response as $response_item) {
          $response_item_array = get_object_vars($response_item);

          foreach ($response_item_array as $response_item_item) {
            $response_item_item = trim($response_item_item);
            if ($response_item_item != '') {
              array_push($product_id_array, $response_item_item);
            }
          }
        }

        if (count($product_id_array) > 0) {
          $product_count = 0;
          $incorrect_price_value_product_count = 0;

          //Вычисление количества знаков после запятой
          $current_commerce_currencies = commerce_currencies(TRUE, FALSE);
          $commerce_default_currency = trim(commerce_default_currency());
          $decimal_default_value = '';
          if (isset($commerce_default_currency) && !empty($commerce_default_currency)) {
            $decimal_default_value = trim(
              $current_commerce_currencies[$commerce_default_currency]['decimals']
            );
            if ((!empty($decimal_default_value) || $decimal_default_value == 0) && is_numeric(
                $decimal_default_value
              ) && ($decimal_default_value >= 0) && ($decimal_default_value <= 4)) {
              $decimal_default_value_int = ((int) $decimal_default_value);
              $decimal_value = 1;
              for ($i = 0; $i < $decimal_default_value_int; $i++) {
                $decimal_value *= 10;
              }

              foreach ($product_id_array as $product_id) {
                $commerce_product_unit = commerce_product_load($product_id);
                $commerce_product_unit_amount = (($commerce_product_unit->commerce_price["und"][0]["amount"]));

                //Вычисление запятой
                $commerce_product_unit_amount_before = ((int) ($commerce_product_unit_amount / $decimal_value));
                $commerce_product_unit_amount_after = $commerce_product_unit_amount % $decimal_value;
                $commerce_product_unit_amount_after_fractional = ((double) $commerce_product_unit_amount_after / $decimal_value);
                $commerce_product_unit_amount_fractional = ((double) $commerce_product_unit_amount_before + $commerce_product_unit_amount_after_fractional);

                if ($price_changing_select_operand_type == 'value') {
                  $operand_value = $operand_par;
                }
                elseif ($price_changing_select_operand_type == 'percent') {
                  $operand_value = $commerce_product_unit_amount_fractional / 100 * $operand_par;
                }
                else {
                  $form_state['result'] = 'field_is_not_correct';
                  break;
                }

                $is_price_changing_select_operation_valid = TRUE;
                $is_division_by_zero = FALSE;
                switch ($price_changing_select_operation) {
                  case 'addition':
                    $commerce_product_unit_amount_fractional += $operand_value;
                    break;
                  case 'subtraction':
                    $commerce_product_unit_amount_fractional -= $operand_value;
                    break;
                  case 'multiplication':
                    $commerce_product_unit_amount_fractional *= $operand_value;
                    break;
                  case 'division':
                    if ($operand_value == 0) {
                      $is_division_by_zero = TRUE;
                      $form_state['result'] = 'division_by_zero';
                      break;
                    }

                    $commerce_product_unit_amount_fractional /= $operand_value;
                    break;
                  case 'replacement':
                    $commerce_product_unit_amount_fractional = $operand_value;
                    break;
                  default:
                    $is_price_changing_select_operation_valid = FALSE;
                }

                if ($is_price_changing_select_operation_valid && !$is_division_by_zero) {
                  $commerce_product_unit_amount_result = ((int) ($commerce_product_unit_amount_fractional * $decimal_value));

                  if (($commerce_product_unit_amount_result <= 2147483647) && ($commerce_product_unit_amount_result > -1)) {
                    $commerce_product_unit->commerce_price["und"][0]["amount"] = $commerce_product_unit_amount_result;
                    commerce_product_save($commerce_product_unit);

                    $product_count++;
                  }
                  else {
                    $incorrect_price_value_product_count++;
                  }
                }
                elseif (!$is_price_changing_select_operation_valid) {
                  $form_state['result'] = 'field_is_not_correct';
                }
              }

              if (($product_count > 0) || ($incorrect_price_value_product_count > 0)) {
                $form_state['result'] = 'successfully';
                ctools_include('object-cache');
                if ($product_count > 0) {
                  ctools_object_cache_set(
                    'commerce_price_control',
                    'successfully_message',
                    ('Цена успешно изменена на ' . $product_count . (($product_count == 1) ? ' товар' : (($product_count > 4) ? ' товаров' : ' товара')))
                  );
                }
                if ($incorrect_price_value_product_count > 0) {
                  ctools_object_cache_set(
                    'commerce_price_control',
                    'incorrect_price_value_product_count_message',
                    ('Результат вычисления цены имеет недопустимо значение. Не удалось изменить цену на ' . $incorrect_price_value_product_count . (($incorrect_price_value_product_count == 1) ? ' товар.' : (($incorrect_price_value_product_count > 4) ? ' товаров.' : ' товара.')))
                  );
                }
              }
              else {
                $form_state['result'] = 'product_change_not_attempt';
              }
            }
            else {
              $form_state['result'] = 'decimal_value_not_valid';
            }
          }
          else {
            $form_state['result'] = 'current_currency_error';
          }
        }
        else {
          $form_state['result'] = 'products_not_found';
        }
      }
      else {
        $form_state['result'] = 'numeric_range_error';
      }
    }
    else {
      $form_state['result'] = 'numeric_error';
    }
  }
  else {
    $form_state['result'] = 'parameters_error';
  }

  $form_state['rebuild'] = TRUE;
}
