<?php

/**
 * Implements hook_commerce_currency_info_alter
 */
function commerce_price_control_commerce_currency_info_alter(
  &$currencies,
  $langcode
) {
  if (db_table_exists('commerce_price_control_decimal')) {
    $query = db_select('commerce_price_control_decimal');
    $query->fields(
      'commerce_price_control_decimal',
      ['currency_symbol', 'currency_decimal']
    );
    $commerce_price_control_decimal_results = $query->execute()->fetchAll();

    if ((isset($commerce_price_control_decimal_results)) && (is_array(
        $commerce_price_control_decimal_results
      )) && (count($commerce_price_control_decimal_results) > 0)) {
      foreach ($commerce_price_control_decimal_results as $commerce_price_control_decimal_result) {
        $currency_symbol = trim(
          $commerce_price_control_decimal_result->currency_symbol
        );
        $currency_decimal = ((int) trim(
          $commerce_price_control_decimal_results[0]->currency_decimal
        ));

        if (is_numeric(
            $currency_decimal
          ) && ($currency_decimal >= 0) && ($currency_decimal <= 4)) {
          $currencies[$currency_symbol]['decimals'] = $currency_decimal;
        }
      }
    }
  }
}

function commerce_price_control_decimal_form($form, &$form_state) {
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
    ) . '/js/commerce_price_control_decimal.js'
  );
  drupal_add_css(
    drupal_get_path(
      'module',
      'commerce_price_control'
    ) . '/css/jquery.confirm/jquery.confirm.css'
  );

  $current_commerce_currencies = commerce_currencies(TRUE, FALSE);
  $current_commerce_currencies_keys = array_keys($current_commerce_currencies);
  $currencies_select_potions = array_combine(
    $current_commerce_currencies_keys,
    $current_commerce_currencies_keys
  );
  array_unshift($currencies_select_potions, 'Нет');

  $currency_default_value = '';
  if (isset($form_state['values']['currencies']) && !empty($form_state['values']['currencies'])) {
    $currency_default_value = trim($form_state['values']['currencies']);
  }

  $decimal_default_value = '';
  if (!empty($currency_default_value)) {
    $query = db_select('commerce_price_control_decimal');
    $query->fields('commerce_price_control_decimal', ['currency_decimal']);
    $query->condition('currency_symbol', $currency_default_value);
    $commerce_price_control_decimal_results = $query->execute()->fetchAll();

    if ((isset($commerce_price_control_decimal_results)) && (is_array(
        $commerce_price_control_decimal_results
      )) && (count($commerce_price_control_decimal_results) > 0)) {
      $decimal_default_value = ((int) trim(
        $commerce_price_control_decimal_results[0]->currency_decimal
      ));
    }
    elseif (isset($current_commerce_currencies[$currency_default_value]['decimals']) && !empty($current_commerce_currencies[$currency_default_value]['decimals'])) {
      $decimal_default_value = $current_commerce_currencies[$currency_default_value]['decimals'];
    }
  }

  $form = [
    '#method' => 'post',
  ];

  $form['currencies-wrapper'] = [
    '#type' => 'fieldset',
  ];
  $form['currencies-wrapper']['currencies'] = [
    '#title' => t('Включённые валюты'),
    '#description' => t('Выбор валюты'),
    '#type' => 'select',
    '#options' => $currencies_select_potions,
    '#default_value' => $currency_default_value,
    '#required' => TRUE,
    '#ajax' => [
      'callback' => 'get_decimal_value_current_currency',
      'wrapper' => 'decimal-value-wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ],
  ];

  $form['decimal-value-wrapper'] = [
    '#type' => 'fieldset',
    '#id' => 'decimal-value-wrapper',
  ];
  $form['decimal-value-wrapper']['decimal-value'] = [
    '#title' => t('Количество знаков после запятой: '),
    '#description' => t(''),
    '#type' => 'textfield',
    '#default_value' => $decimal_default_value,
    '#attributes' => ['min' => '0', 'max' => '4'],
    '#size' => 3,
    '#maxlength' => 1,
    '#required' => TRUE,
    '#id' => 'edit-decimal-value',
    '#name' => 'decimal-value',
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Сохранить'),
  ];

  if (isset($form_state['result'])) {
    if ($form_state['result'] == 'parameters_error') {
      drupal_set_message(t('Не все параметры были получены'), 'error', FALSE);
    }
    elseif ($form_state['result'] == 'currency_symbol_not_valid') {
      drupal_set_message(t('Валюта не найдена'), 'error', FALSE);
    }
    elseif ($form_state['result'] == 'decimal_value_not_valid') {
      drupal_set_message(
        t(
          'Поле "Количества знаков после запятой" может содержать только цифру, включительно от 0 до 4'
        ),
        'error',
        FALSE
      );
    }
    elseif ($form_state['result'] == 'successfully') {
      drupal_set_message(
        t(
          'Количество знаков после запятой успешно изменено.'
        ) . '&nbsp;<a href="/admin/config/development/performance">' . t(
          'Очистите кеш.'
        ) . '</a>',
        'status',
        FALSE
      );
    }
    else {
      drupal_set_message(t('Неизвестный результат'), 'warning', FALSE);
    }
  }

  $form_state['source'] = 'form';
  return $form;
}

function get_decimal_value_current_currency($form, &$form_state) {
  $current_commerce_currencies = commerce_currencies(TRUE, FALSE);
  $currencies_current_key = trim($form_state['values']['currencies']);

  $decimal_default_value = '';
  if (isset($currencies_current_key) && !empty($currencies_current_key)) {
    $query = db_select('commerce_price_control_decimal');
    $query->fields('commerce_price_control_decimal', ['currency_decimal']);
    $query->condition('currency_symbol', $currencies_current_key);
    $commerce_price_control_decimal_results = $query->execute()->fetchAll();

    if ((isset($commerce_price_control_decimal_results)) && (is_array(
        $commerce_price_control_decimal_results
      )) && (count($commerce_price_control_decimal_results) > 0)) {
      $decimal_default_value = ((int) trim(
        $commerce_price_control_decimal_results[0]->currency_decimal
      ));
    }
    elseif (isset($current_commerce_currencies[$currencies_current_key]['decimals']) && !empty($current_commerce_currencies[$currencies_current_key]['decimals'])) {
      $decimal_default_value = $current_commerce_currencies[$currencies_current_key]['decimals'];
    }
  }

  $form['decimal-value-wrapper'] = [
    '#type' => 'fieldset',
    '#id' => 'decimal-value-wrapper',
  ];
  $form['decimal-value-wrapper']['decimal-value'] = [
    '#title' => t('Количество знаков после запятой: '),
    '#description' => t(''),
    '#type' => 'textfield',
    '#value' => $decimal_default_value,
    '#default_value' => $decimal_default_value,
    '#attributes' => ['min' => '0', 'max' => '4'],
    '#size' => 3,
    '#maxlength' => 1,
    '#required' => TRUE,
    '#id' => 'edit-decimal-value',
    '#name' => 'decimal-value',
  ];

  $form_state['source'] = 'ajax';
  return $form['decimal-value-wrapper'];
}

function commerce_price_control_decimal_form_submit($form, &$form_state) {
  if ($form_state['source'] == 'form') {
    if (isset($form_state['values']['currencies']) && !empty($form_state['values']['currencies'])
      && isset($form_state['values']['decimal-value']) && (!empty($form_state['values']['decimal-value']) || $form_state['values']['decimal-value'] == 0)) {
      $decimal_value = trim($form_state['values']['decimal-value']);
      $currency_symbol_value = trim($form_state['values']['currencies']);

      $current_commerce_currencies = commerce_currencies(TRUE, FALSE);
      if (array_key_exists(
        $currency_symbol_value,
        $current_commerce_currencies
      )) {
        if (is_numeric(
            $decimal_value
          ) && ($decimal_value >= 0) && ($decimal_value <= 4)) {
          $query = db_select('commerce_price_control_decimal');
          $query->fields(
            'commerce_price_control_decimal',
            ['currency_decimal']
          );
          $query->condition('currency_symbol', $currency_symbol_value);
          $commerce_price_control_decimal_results = $query->execute()->fetchAll(
          );

          if ((isset($commerce_price_control_decimal_results)) && (is_array(
              $commerce_price_control_decimal_results
            )) && (count($commerce_price_control_decimal_results) > 0)) {
            $query = db_update('commerce_price_control_decimal');
            $query->fields(['currency_decimal' => $decimal_value]);
            $query->condition('currency_symbol', $currency_symbol_value);
            $query->execute();
          }
          else {
            $query = db_insert('commerce_price_control_decimal');
            $query->fields(
              [
                'currency_symbol' => $currency_symbol_value,
                'currency_decimal' => $decimal_value,
              ]
            );
            $query->execute();
          }

          $form_state['result'] = 'successfully';
        }
        else {
          $form_state['result'] = 'decimal_value_not_valid';
        }
      }
      else {
        $form_state['result'] = 'currency_symbol_not_valid';
      }
    }
    else {
      $form_state['result'] = 'parameters_error';
    }

    $form_state['source'] = 'submit';
    $form_state['rebuild'] = TRUE;
  }
}
