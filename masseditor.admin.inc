<?php

function _masseditor_default_settings() {
  return array(
    'model' => TRUE,
    'title' => TRUE,
    'list_price' => FALSE,
    'cost' => FALSE,
    'sell_price' => TRUE,
    'weight' => FALSE,
    'weight_units' => FALSE,
    'length' => FALSE,
    'width' => FALSE,
    'height' => FALSE,
    'length_units' => FALSE,
    'pkg_qty' => FALSE,
    'default_qty' => FALSE,
    'crs_sell_price' => FALSE,
    'crs_sell_price_cur' => FALSE,
  );
}

function _masseditor_default_options() {
  return array(
    'per_page' => 50,
  );
}

function masseditor_settings_form(&$form_state = NULL) {
  drupal_add_css(drupal_get_path('module', 'masseditor') . '/masseditor.css');

  $form['#tree'] = TRUE;
  $form['#theme'] = 'masseditor_settings_form';
  $form['fs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $def_settings = _masseditor_default_settings();
  $settings = variable_get('masseditor_settings', array());
  foreach ($def_settings as $s_name => $s_value) {
    if (!isset($settings[$s_name])) {
      $settings[$s_name] = $s_value;
    }
  }

  if (db_table_exists('content_node_field_instance')) {
    $cck_settings = array();
    $products_types = uc_product_types();
    foreach($products_types as $products_type) {
      $fields_query = db_query("select field_name from {content_node_field_instance} where type_name = '%s' and widget_type in ('text_textarea', 'text_textfield', 'optionwidgets_select')", $products_type);
      while($data = db_fetch_object($fields_query)) {
        $cck_settings[$data->field_name] = FALSE;
      }
    }
  }
  $settings += $cck_settings;

  $weight = 0;
  foreach ($settings as $setting_name => $setting_status) {
    $form['fs']['cb'][$setting_name] = array(
      '#type' => 'checkbox',
      '#title' => $setting_name,
      '#default_value' => $setting_status,
    );
    $form['fs']['we'][$setting_name] = array(
      '#type' => 'textfield',
      '#default_value' => $weight,
      '#size' => 2,
    );
    $weight++;
  }

  $masseditor_options = variable_get('masseditor_options', _masseditor_default_options());

  $form['fs']['opt']['per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Items per page'),
    '#default_value' => $masseditor_options['per_page'],
    '#size' => 7,
    '#maxlength' => 2,
  );


  $form['fs']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function masseditor_settings_form_submit($form, &$form_state) {
  $new_settings = array();
  $cb_list = $form_state['clicked_button']['#post']['fs']['we'];
  foreach ($cb_list as $cb_name => $cb_old_order) {
    $new_settings[$cb_name] = (boolean)$form_state['values']['fs']['cb'][$cb_name];
  }
  $masseditor_options['per_page'] = $form_state['values']['fs']['opt']['per_page'];
  variable_set('masseditor_settings', $new_settings);
  variable_set('masseditor_options', $masseditor_options);
}
