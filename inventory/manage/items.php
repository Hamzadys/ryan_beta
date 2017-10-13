<?php

$page_security = 'SA_ITEM';
$path_to_root = "../..";
include($path_to_root . "/includes/session.inc");

$js = "";
if ($SysPrefs->use_popup_windows)
	$js .= get_js_open_window(900, 500);
if (user_use_date_picker())
	$js .= get_js_date_picker();

if (isset($_GET['FixedAsset'])) {
  $page_security = 'SA_ASSET';
  $_SESSION['page_title'] = _($help_context = "Fixed Assets");
  $_POST['mb_flag'] = 'F';
  $_POST['fixed_asset']  = 1;
}
else {
  $_SESSION['page_title'] = _($help_context = "Items");
	if (!get_post('fixed_asset'))
		$_POST['fixed_asset']  = 0;
}


page($_SESSION['page_title'], false, false, "", $js);

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/data_checks.inc");
include_once($path_to_root . "/inventory/includes/inventory_db.inc");

include_once($path_to_root . "/fixed_assets/includes/fixed_assets_db.inc");

$user_comp = user_company();
$new_item = get_post('stock_id')=='' || get_post('cancel') || get_post('clone'); 
//------------------------------------------------------------------------------------

if (isset($_GET['stock_id']))
{
	$_POST['stock_id'] = $_GET['stock_id'];
}
$stock_id = get_post('stock_id');
if (list_updated('stock_id')) {
	$_POST['NewStockID'] = $stock_id = get_post('stock_id');
    clear_data();
	$Ajax->activate('details');
	$Ajax->activate('controls');
}

if (get_post('cancel')) {
	$_POST['NewStockID'] = $stock_id = $_POST['stock_id'] = '';
    clear_data();
	set_focus('stock_id');
	$Ajax->activate('_page_body');
}
if (list_updated('category_id') || list_updated('mb_flag') || list_updated('fa_class_id') || list_updated('depreciation_method')) {
	$Ajax->activate('details');
}
$upload_file = "";
if (isset($_FILES['pic']) && $_FILES['pic']['name'] != '') 
{
	$stock_id = $_POST['NewStockID'];
	$result = $_FILES['pic']['error'];
 	$upload_file = 'Yes'; //Assume all is well to start off with
	$filename = company_path().'/images';
	if (!file_exists($filename))
	{
		mkdir($filename);
	}	
	$filename .= "/".item_img_name($stock_id).".jpg";

  if ($_FILES['pic']['error'] == UPLOAD_ERR_INI_SIZE) {
    display_error(_('The file size is over the maximum allowed.'));
		$upload_file ='No';
  }
  elseif ($_FILES['pic']['error'] > 0) {
		display_error(_('Error uploading file.'));
		$upload_file ='No';
  }
	
	//But check for the worst 
	if ((list($width, $height, $type, $attr) = getimagesize($_FILES['pic']['tmp_name'])) !== false)
		$imagetype = $type;
	else
		$imagetype = false;

	if ($imagetype != IMAGETYPE_GIF && $imagetype != IMAGETYPE_JPEG && $imagetype != IMAGETYPE_PNG)
	{	//File type Check
		display_warning( _('Only graphics files can be uploaded'));
		$upload_file ='No';
	}
	elseif (!in_array(strtoupper(substr(trim($_FILES['pic']['name']), strlen($_FILES['pic']['name']) - 3)), array('JPG','PNG','GIF')))
	{
		display_warning(_('Only graphics files are supported - a file extension of .jpg, .png or .gif is expected'));
		$upload_file ='No';
	} 
	elseif ( $_FILES['pic']['size'] > ($SysPrefs->max_image_size * 1024)) 
	{ //File Size Check
		display_warning(_('The file size is over the maximum allowed. The maximum size allowed in KB is') . ' ' . $SysPrefs->max_image_size);
		$upload_file ='No';
	} 
	elseif ( $_FILES['pic']['type'] == "text/plain" ) 
	{  //File type Check
		display_warning( _('Only graphics files can be uploaded'));
        $upload_file ='No';
	} 
	elseif (file_exists($filename))
	{
		$result = unlink($filename);
		if (!$result) 
		{
			display_error(_('The existing image could not be removed'));
			$upload_file ='No';
		}
	}
	
	if ($upload_file == 'Yes')
	{
		$result  =  move_uploaded_file($_FILES['pic']['tmp_name'], $filename);
	}
	$Ajax->activate('details');
 /* EOF Add Image upload for New Item  - by Ori */
}

if (get_post('fixed_asset')) {
	check_db_has_fixed_asset_categories(_("There are no fixed asset categories defined in the system. At least one fixed asset category is required to add a fixed asset."));
	check_db_has_fixed_asset_classes(_("There are no fixed asset classes defined in the system. At least one fixed asset class is required to add a fixed asset."));
} else
	check_db_has_stock_categories(_("There are no item categories defined in the system. At least one item category is required to add a item."));

check_db_has_item_tax_types(_("There are no item tax types defined in the system. At least one item tax type is required to add a item."));

function clear_data()
{
	unset($_POST['long_description']);
	unset($_POST['description']);
	unset($_POST['carton']);
	unset($_POST['category_id']);
	unset($_POST['tax_type_id']);
	unset($_POST['units']);
	unset($_POST['mb_flag']);
	unset($_POST['NewStockID']);
	unset($_POST['dimension_id']);
	unset($_POST['dimension2_id']);
	unset($_POST['no_sale']);
	unset($_POST['no_purchase']);
	unset($_POST['depreciation_method']);
	unset($_POST['depreciation_rate']);
	unset($_POST['depreciation_factor']);
	unset($_POST['depreciation_start']);
	unset($_POST['salesman']);
	
	unset($_POST['amount1']);
	unset($_POST['amount2']);
	unset($_POST['amount3']);
	unset($_POST['amount4']);
	unset($_POST['amount5']);
	unset($_POST['amount6']);

	unset($_POST['text1']);
	unset($_POST['text2']);
	unset($_POST['text3']);
	unset($_POST['text4']);
	unset($_POST['text5']);
	unset($_POST['text6']);
	
	
	unset($_POST['combo1']);
	unset($_POST['combo2']);
	unset($_POST['combo3']);
	unset($_POST['combo4']);
	unset($_POST['combo5']);
	unset($_POST['combo6']);


	
	}

//------------------------------------------------------------------------------------

if (isset($_POST['addupdate'])) 
{

	$input_error = 0;
	if ($upload_file == 'No')
		$input_error = 1;
	if (strlen($_POST['description']) == 0) 
	{
		$input_error = 1;
		display_error( _('The item name must be entered.'));
		set_focus('description');
	} 
	elseif (strlen($_POST['NewStockID']) == 0) 
	{
		$input_error = 1;
		display_error( _('The item code cannot be empty'));
		set_focus('NewStockID');
	}
	elseif (strstr($_POST['NewStockID'], " ") || strstr($_POST['NewStockID'],"'") || 
		strstr($_POST['NewStockID'], "+") || strstr($_POST['NewStockID'], "\"") || 
		strstr($_POST['NewStockID'], "&") || strstr($_POST['NewStockID'], "\t")) 
	{
		$input_error = 1;
		display_error( _('The item code cannot contain any of the following characters -  & + OR a space OR quotes'));
		set_focus('NewStockID');

	}
	elseif ($new_item && db_num_rows(get_item_kit($_POST['NewStockID'])))
	{
		  	$input_error = 1;
      		display_error( _("This item code is already assigned to stock item or sale kit."));
			set_focus('NewStockID');
	}
	
  if (get_post('fixed_asset')) {
    if ($_POST['depreciation_rate'] > 100) {
      $_POST['depreciation_rate'] = 100;
    }
    elseif ($_POST['depreciation_rate'] < 0) {
      $_POST['depreciation_rate'] = 0;
    }
    $move_row = get_fixed_asset_move($_POST['NewStockID'], ST_SUPPRECEIVE);
    if (isset($_POST['depreciation_start']) && strtotime($_POST['depreciation_start']) < strtotime($move_row['tran_date'])) {
      display_warning(_('The depracation cannot start before the fixed asset purchase date'));
    }
  }
	
	if ($input_error != 1)
	{
		if (check_value('del_image'))
		{
			$filename = company_path().'/images/'.item_img_name($_POST['NewStockID']).".jpg";
			if (file_exists($filename))
				unlink($filename);
		}
		
		if (!$new_item) 
		{ /*so its an existing one */
			update_item($_POST['NewStockID'], $_POST['description'],
				$_POST['long_description'], $_POST['carton'], $_POST['category_id'], 
				$_POST['tax_type_id'], get_post('units'),
				get_post('fixed_asset') ? 'F' : get_post('mb_flag'), $_POST['sales_account'],
				$_POST['inventory_account'], $_POST['cogs_account'],
				$_POST['adjustment_account'], $_POST['wip_account'], 
				$_POST['dimension_id'], $_POST['dimension2_id'],
				check_value('no_sale'), check_value('editable'), check_value('no_purchase'),
				get_post('depreciation_method'), input_num('depreciation_rate'), input_num('depreciation_factor'), get_post('depreciation_start'),
				get_post('fa_class_id'),$_POST['salesman'],$_POST['batch_status'],
				get_post('amount1'),get_post('amount2')
				,get_post('amount3'),get_post('amount4'),get_post('amount5'),get_post('amount6'),get_post('text1'),get_post('text2'),get_post('text3'),get_post('combo1')
				,get_post('combo2'),get_post('combo3'),get_post('combo4'),get_post('combo5'),get_post('combo6'),$_POST['alt_units'], $_POST['con_factor'], $_POST['con_type']);


			update_record_status($_POST['NewStockID'], $_POST['inactive'],
				'stock_master', 'stock_id');
			update_record_status($_POST['NewStockID'], $_POST['inactive'],
				'item_codes', 'item_code');
			set_focus('stock_id');
			$Ajax->activate('stock_id'); // in case of status change
			display_notification(_("Item has been updated."));
		} 
		else 
		{ //it is a NEW part

			add_item2($_POST['NewStockID'], $_POST['description'],
				$_POST['long_description'], $_POST['carton'], $_POST['category_id'], $_POST['tax_type_id'],
				$_POST['units'], get_post('fixed_asset') ? 'F' : get_post('mb_flag'), $_POST['sales_account'],
				$_POST['inventory_account'], $_POST['cogs_account'],
				$_POST['adjustment_account'], $_POST['wip_account'], 
				$_POST['dimension_id'], $_POST['dimension2_id'],
				check_value('no_sale'), check_value('editable'), check_value('no_purchase'),
				get_post('depreciation_method'), input_num('depreciation_rate'), input_num('depreciation_factor'), get_post('depreciation_start'),
				get_post('fa_class_id'),get_post('salesman'),$_POST['batch_status'],get_post('amount1'),get_post('amount2')
				,get_post('amount3'),get_post('amount4'),get_post('amount5'),get_post('amount6'),get_post('text1'),get_post('text2'),get_post('text3'),get_post('combo1')
				,get_post('combo2'),get_post('combo3'),get_post('combo4'),get_post('combo5'),get_post('combo6'), $_POST['alt_units'], $_POST['con_factor'], $_POST['con_type']);


			display_notification(_("A new item has been added."));
			$_POST['stock_id'] = $_POST['NewStockID'] = 
			$_POST['description'] = $_POST['long_description']  =  $_POST['carton'] = '';
			$_POST['no_sale'] = $_POST['editable'] = $_POST['no_purchase'] =0;
			set_focus('NewStockID');
		}
		$Ajax->activate('_page_body');
	}
}

if (get_post('clone')) {
	unset($_POST['stock_id']);
	$stock_id = '';
	unset($_POST['inactive']);
	set_focus('NewStockID');
	$Ajax->activate('_page_body');
}

//------------------------------------------------------------------------------------

function check_usage($stock_id, $dispmsg=true)
{
	$msg = item_in_foreign_codes($stock_id);

	if ($msg != '')	{
		if($dispmsg) display_error($msg);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------

if (isset($_POST['delete']) && strlen($_POST['delete']) > 1) 
{

	if (check_usage($_POST['NewStockID'])) {

		$stock_id = $_POST['NewStockID'];
		delete_item($stock_id);
		$filename = company_path().'/images/'.item_img_name($stock_id).".jpg";
		if (file_exists($filename))
			unlink($filename);
		display_notification(_("Selected item has been deleted."));
		$_POST['stock_id'] = '';
		clear_data();
		set_focus('stock_id');
		$new_item = true;
		$Ajax->activate('_page_body');
	}
}

function item_settings(&$stock_id, $new_item) 
{
	global $SysPrefs, $path_to_root, $page_nested, $depreciation_methods;

	start_outer_table(TABLESTYLE2);

	table_section(1);

	table_section_title(_("General Settings"));

	//------------------------------------------------------------------------------------
	if ($new_item) 
	{
		text_row(_("Item Code:"), 'NewStockID', null, 21, 20);

		$_POST['inactive'] = 0;
	} 
	else 
	{ // Must be modifying an existing item
		if (get_post('NewStockID') != get_post('stock_id') || get_post('addupdate')) { // first item display

			$_POST['NewStockID'] = $_POST['stock_id'];

			$myrow = get_item($_POST['NewStockID']);

			$_POST['long_description'] = $myrow["long_description"];
			$_POST['description'] = $myrow["description"];
			$_POST['carton'] = $myrow["carton"];
			$_POST['category_id']  = $myrow["category_id"];
			$_POST['tax_type_id']  = $myrow["tax_type_id"];
			$_POST['units']  = $myrow["units"];
			$_POST['alt_units']  = $myrow["alt_units"];
			$_POST['con_factor']  = $myrow["con_factor"];
			$_POST['con_type']  = $myrow["con_type"];
			$_POST['mb_flag']  = $myrow["mb_flag"];

			$_POST['depreciation_method'] = $myrow['depreciation_method'];
			$_POST['depreciation_rate'] = number_format2($myrow['depreciation_rate'], 1);
			$_POST['depreciation_factor'] = number_format2($myrow['depreciation_factor'], 1);
			$_POST['depreciation_start'] = sql2date($myrow['depreciation_start']);
			$_POST['depreciation_date'] = sql2date($myrow['depreciation_date']);
			$_POST['fa_class_id'] = $myrow['fa_class_id'];
			$_POST['material_cost'] = $myrow['material_cost'];
			$_POST['purchase_cost'] = $myrow['purchase_cost'];
			
			$_POST['sales_account'] =  $myrow['sales_account'];
			$_POST['inventory_account'] = $myrow['inventory_account'];
			$_POST['cogs_account'] = $myrow['cogs_account'];
			$_POST['adjustment_account']	= $myrow['adjustment_account'];
			$_POST['wip_account']	= $myrow['wip_account'];
			$_POST['dimension_id']	= $myrow['dimension_id'];
			$_POST['dimension2_id']	= $myrow['dimension2_id'];
			$_POST['no_sale']	= $myrow['no_sale'];
			$_POST['batch_status']	= $myrow['batch_status'];
			$_POST['no_purchase']	= $myrow['no_purchase'];
			$_POST['del_image'] = 0;
			$_POST['inactive'] = $myrow["inactive"];
			$_POST['editable'] = $myrow["editable"];
			$_POST['salesman'] = $myrow["salesman"];
			
			$_POST['amount1'] = $myrow["amount1"];
			$_POST['amount2'] = $myrow["amount2"];
			$_POST['amount3'] = $myrow["amount3"];
			$_POST['amount4'] = $myrow["amount4"];
			$_POST['amount5'] = $myrow["amount5"];
			$_POST['amount6'] = $myrow["amount6"];
			
			$_POST['text1'] = $myrow["text1"];
			$_POST['text2'] = $myrow["text2"];
			$_POST['text3'] = $myrow["text3"];
			$_POST['text4'] = $myrow["text4"];
			$_POST['text5'] = $myrow["text5"];
			$_POST['text6'] = $myrow["text6"];
			
			
			$_POST['combo1'] = $myrow["combo1"];
			$_POST['combo2'] = $myrow["combo2"];
			$_POST['combo3'] = $myrow["combo3"];
			$_POST['combo4'] = $myrow["combo4"];
			$_POST['combo5'] = $myrow["combo5"];
			$_POST['combo6'] = $myrow["combo6"];
			
		}
		label_row(_("Item Code:"),$_POST['NewStockID']);
		hidden('NewStockID', $_POST['NewStockID']);
		set_focus('description');
	}
	$fixed_asset = get_post('fixed_asset');

	text_row(_("Name:"), 'description', null, 52, 200);

	textarea_row(_('Description:'), 'long_description', null, 42, 3);

	stock_categories_list_row(_("Category:"), 'category_id', null, false, $new_item, $fixed_asset);

	if ($new_item && (list_updated('category_id') || !isset($_POST['units']))) {

		$category_record = get_item_category($_POST['category_id']);

		$_POST['tax_type_id'] = $category_record["dflt_tax_type"];
		$_POST['units'] = $category_record["dflt_units"];
		$_POST['mb_flag'] = $category_record["dflt_mb_flag"];
		$_POST['inventory_account'] = $category_record["dflt_inventory_act"];
		$_POST['cogs_account'] = $category_record["dflt_cogs_act"];
		$_POST['sales_account'] = $category_record["dflt_sales_act"];
		$_POST['adjustment_account'] = $category_record["dflt_adjustment_act"];
		$_POST['wip_account'] = $category_record["dflt_wip_act"];
		$_POST['dimension_id'] = $category_record["dflt_dim1"];
		$_POST['dimension2_id'] = $category_record["dflt_dim2"];
		$_POST['no_sale'] = $category_record["dflt_no_sale"];
		$_POST['no_purchase'] = $category_record["dflt_no_purchase"];
		$_POST['editable'] = 0;

	}
	$fresh_item = !isset($_POST['NewStockID']) || $new_item 
		|| check_usage($_POST['stock_id'],false);

	item_tax_types_list_row(_("Item Tax Type:"), 'tax_type_id', null);

	if (!get_post('fixed_asset'))
		stock_item_types_list_row(_("Item Type:"), 'mb_flag', null, $fresh_item);

	table_section_title(_("Unit of Measurement"));
    text_row(_("Packing:"), 'carton', null, 5, 5);

	stock_units_list_row(_('Units of Measure:'), 'units', null, $fresh_item);

    stock_units_list_row(_('Alt. Units of Measure:'), 'alt_units', null); //dz

	amount_row('Conversion Factor', 'con_factor'); //dz


    yesno_list_row(_('Conversion Type'), 'con_type',null, "Main UoM/Alt. UoM", "Alt. UoM/Main UoM"); //dz
    
	check_row(_("Editable description:"), 'editable');

	if (get_post('fixed_asset'))
		hidden('no_sale', 0);
	else
		check_row(_("Exclude from sales:"), 'no_sale');

	check_row(_("Exclude from purchases:"), 'no_purchase');

	check_row(_("Allow null batch:"), 'batch_status');
	
	table_section(2);
	table_section_title(_("Custom Fields"));
	
	sales_persons_list_row( _("Sales Person:"), 'salesman', null);
    
    $myrow1 = get_company_item_pref('amount1');
//	display_error($myrow1['item_enable']."hareem");
	if($myrow1['item_enable'] == 0){}
	else {
		amount_row($myrow1['label_value'].'', 'amount1');
	}


	$myrow2 = get_company_item_pref('amount2');
	if($myrow2['item_enable'] == 0){}
	else {
		amount_row($myrow2['label_value'].'', 'amount2');
	}


	$myrow3 = get_company_item_pref('amount3');
	if($myrow3['item_enable'] == 0){}
	else {
		amount_row($myrow3['label_value'].'', 'amount3');
	}


	$myrow4 = get_company_item_pref('amount4');
	if($myrow4['item_enable'] == 0){}
	else {
		amount_row($myrow4['label_value'].'', 'amount4');
	}

	$myrow5 = get_company_item_pref('amount5');
	if($myrow5['item_enable'] == 0){}
	else {
		amount_row($myrow5['label_value'].'', 'amount5');
	}

	$myrow6 = get_company_item_pref('amount6');
	if($myrow6['item_enable'] == 0){}
	else {
		amount_row($myrow6['label_value'].'', 'amount6');
	}

	$myrow7 = get_company_item_pref('text1');
	if($myrow7['item_enable'] == 0){}
	else {
		text_row(_($myrow7['label_value'].""), 'text1', null, 50, 50);
	}
	$myrow8 = get_company_item_pref('text2');
	if($myrow8['item_enable'] == 0){}
	else {
		text_row(_($myrow8['label_value'].""), 'text2', null, 21, 20);
	}
	$myrow9 = get_company_item_pref('text3');
	if($myrow9['item_enable'] == 0){}
	else {
		text_row(_($myrow9['label_value'].""), 'text3', null, 21, 20);
	}
	$myrow10 = get_company_item_pref('text4');
	if($myrow10['item_enable'] == 0){}
	else {
		text_row(_($myrow10['label_value'].""), 'text4', null, 21, 20);
	}
	$myrow11 = get_company_item_pref('text5');
	if($myrow11['item_enable'] == 0){}
	else {
		text_row(_($myrow11['label_value'].""), 'text5', null, 21, 20);
	}
	$myrow12 = get_company_item_pref('text6');
	if($myrow12['item_enable'] == 0){}
	else {
		text_row(_($myrow12['label_value'].""), 'text6', null, 21, 20);
	}
	
	
	$myrow33 = get_company_item_pref('combo1');
	if($myrow33['item_enable'] == 0){}
	else {
		combo_item_1_list_row($myrow33['label_value']."", 'combo1', null, true, false);
	}

	$myrow44 = get_company_item_pref('combo2');
	if($myrow44['item_enable'] == 0){}
	else {
		combo_item_2_list_row($myrow44['label_value']."", 'combo2', null, true, false);
	}

	$myrow55 = get_company_item_pref('combo3');

	if($myrow55['item_enable'] == 0){}
	else {
		combo_item_3_list_row($myrow55['label_value']."", 'combo3', null, true, false);
	}

	$myrow13 = get_company_item_pref('combo4');
	if($myrow13['item_enable'] == 0){}
	else {
		combo_item_4_list_row($myrow13['label_value']."", 'combo4', null, true, false);
	}

	$myrow35 = get_company_item_pref('combo5');
	if($myrow35['item_enable'] == 0){}
	else {
		combo_item_5_list_row($myrow35['label_value']."", 'combo5', null, true, false);
	}


	$myrow36 = get_company_item_pref('combo6');
	if($myrow36['item_enable'] == 0){}
	else {
		combo_item_6_list_row($myrow36['label_value']."", 'combo6', null, true, false);
	}

	$myrow14 = get_company_item_pref('date1');
	if($myrow14['item_enable'] == 0){}
	else {
		date_row($myrow14['label_value']."", 'date1', null);
	}
	$myrow15 = get_company_item_pref('date2');
	if($myrow15['item_enable'] == 0){}
	else {
		date_row($myrow15['label_value']."", 'date2', null);
	}
	$myrow16 = get_company_item_pref('date3');
	if($myrow16['item_enable'] == 0){}
	else {
		date_row($myrow16['label_value']."", 'date3', null);
	}
	////26-09-17


	if (get_post('fixed_asset')) {
		table_section_title(_("Depreciation"));

		fixed_asset_classes_list_row(_("Fixed Asset Class").':', 'fa_class_id', null, false, true);

		array_selector_row(_("Depreciation Method").":", "depreciation_method", null, $depreciation_methods, array('select_submit'=> true));

		if (!isset($_POST['depreciation_rate']) || (list_updated('fa_class_id') || list_updated('depreciation_method'))) {
			$class_row = get_fixed_asset_class($_POST['fa_class_id']);
			$_POST['depreciation_rate'] = get_post('depreciation_method') == 'N' ? ceil(100/$class_row['depreciation_rate'])
				: $class_row['depreciation_rate'];
		}

		if ($_POST['depreciation_method'] == 'O')
		{
			hidden('depreciation_rate', 100);
			label_row(_("Depreciation Rate").':', "100 %");
		}
		elseif ($_POST['depreciation_method'] == 'N')
		{
			small_amount_row(_("Depreciation Years").':', 'depreciation_rate', null, null, _('years'), 0);
		}
		elseif ($_POST['depreciation_method'] == 'D')
			small_amount_row(_("Base Rate").':', 'depreciation_rate', null, null, '%', user_percent_dec());
		else
			small_amount_row(_("Depreciation Rate").':', 'depreciation_rate', null, null, '%', user_percent_dec());

		if ($_POST['depreciation_method'] == 'D')
			small_amount_row(_("Rate multiplier").':', 'depreciation_factor', null, null, '', 2);

		// do not allow to change the depreciation start after this item has been depreciated
		if ($new_item || $_POST['depreciation_start'] == $_POST['depreciation_date'])
			date_row(_("Depreciation Start").':', 'depreciation_start', null, null, 1 - date('j'));
		else {
			hidden('depreciation_start');
			label_row(_("Depreciation Start").':', $_POST['depreciation_start']);
			label_row(_("Last Depreciation").':', $_POST['depreciation_date']==$_POST['depreciation_start'] ? _("None") :  $_POST['depreciation_date']);
		}
		hidden('depreciation_date');
	}
	table_section(2);

	$dim = get_company_pref('use_dimension');
	if ($dim >= 1)
	{
		table_section_title(_("Dimensions"));

		dimensions_list_row(_("Dimension")." 1", 'dimension_id', null, true, " ", false, 1);
		if ($dim > 1)
			dimensions_list_row(_("Dimension")." 2", 'dimension2_id', null, true, " ", false, 2);
	}
	if ($dim < 1)
		hidden('dimension_id', 0);
	if ($dim < 2)
		hidden('dimension2_id', 0);

	table_section_title(_("GL Accounts"));

	gl_all_accounts_list_row(_("Sales Account:"), 'sales_account', $_POST['sales_account']);

	if (get_post('fixed_asset')) {
		gl_all_accounts_list_row(_("Asset account:"), 'inventory_account', $_POST['inventory_account']);
		gl_all_accounts_list_row(_("Depreciation cost account:"), 'cogs_account', $_POST['cogs_account']);
		gl_all_accounts_list_row(_("Depreciation/Disposal account:"), 'adjustment_account', $_POST['adjustment_account']);
	}
	elseif (!is_service($_POST['mb_flag'])) 
	{
		gl_all_accounts_list_row(_("Inventory Account:"), 'inventory_account', $_POST['inventory_account']);
		gl_all_accounts_list_row(_("C.O.G.S. Account:"), 'cogs_account', $_POST['cogs_account']);
		gl_all_accounts_list_row(_("Inventory Adjustments Account:"), 'adjustment_account', $_POST['adjustment_account']);
	}
	else 
	{
		gl_all_accounts_list_row(_("C.O.G.S. Account:"), 'cogs_account', $_POST['cogs_account']);
		hidden('inventory_account', $_POST['inventory_account']);
		hidden('adjustment_account', $_POST['adjustment_account']);
	}


	if (is_manufactured($_POST['mb_flag']))
		gl_all_accounts_list_row(_("WIP Account:"), 'wip_account', $_POST['wip_account']);
	else
		hidden('wip_account', $_POST['wip_account']);

	table_section_title(_("Other"));

	// Add image upload for New Item  - by Joe
	file_row(_("Image File (.jpg)") . ":", 'pic', 'pic');
	// Add Image upload for New Item  - by Joe
	$stock_img_link = "";
	$check_remove_image = false;
	if (isset($_POST['NewStockID']) && file_exists(company_path().'/images/'
		.item_img_name($_POST['NewStockID']).".jpg")) 
	{
	 // 31/08/08 - rand() call is necessary here to avoid caching problems.
		$stock_img_link .= "<img id='item_img' alt = '[".$_POST['NewStockID'].".jpg".
			"]' src='".company_path().'/images/'.item_img_name($_POST['NewStockID']).
			".jpg?nocache=".rand()."'"." height='".$SysPrefs->pic_height."' border='0'>";
		$check_remove_image = true;
	} 
	else 
	{
		$stock_img_link .= _("No image");
	}

	label_row("&nbsp;", $stock_img_link);
	if ($check_remove_image)
		check_row(_("Delete Image:"), 'del_image');

	record_status_list_row(_("Item status:"), 'inactive');

	if (get_post('fixed_asset')) {
		table_section_title(_("Values"));
		if (!$new_item) {
			hidden('material_cost');
			hidden('purchase_cost');
			label_row(_("Initial Value").":", price_format($_POST['purchase_cost']), "", "align='right'");
			label_row(_("Depreciations").":", price_format($_POST['purchase_cost'] - $_POST['material_cost']), "", "align='right'");
			label_row(_("Current Value").':', price_format($_POST['material_cost']), "", "align='right'");
		}
	}
	end_outer_table(1);

	div_start('controls');
	if (!isset($_POST['NewStockID']) || $new_item) 
	{
		submit_center('addupdate', _("Insert New Item"), true, '', 'default');
	} 
	else 
	{
		submit_center_first('addupdate', _("Update Item"), '', 
			$page_nested ? true : 'default');
		submit_return('select', get_post('stock_id'), 
			_("Select this items and return to document entry."), 'default');
		submit('clone', _("Clone This Item"), true, '', true);
		submit('delete', _("Delete This Item"), true, '', true);
		submit_center_last('cancel', _("Cancel"), _("Cancel Edition"), 'cancel');
	}

	div_end();
}

//-------------------------------------------------------------------------------------------- 

start_form(true);

if (db_has_stock_items()) 
{
	start_table(TABLESTYLE_NOBORDER);
	start_row();
    stock_items_list_cells(_("Select an item:"), 'stock_id', null,
	  _('New item'), true, check_value('show_inactive'), false, array('fixed_asset' => get_post('fixed_asset')));
	$new_item = get_post('stock_id')=='';
	check_cells(_("Show inactive:"), 'show_inactive', null, true);
	end_row();
	end_table();

	if (get_post('_show_inactive_update')) {
		$Ajax->activate('stock_id');
		set_focus('stock_id');
	}
}
else
{
	hidden('stock_id', get_post('stock_id'));
}

div_start('details');

$stock_id = get_post('stock_id');
if (!$stock_id)
	unset($_POST['_tabs_sel']); // force settings tab for new customer

$tabs = (get_post('fixed_asset'))
	? array(
		'settings' => array(_('&General settings'), $stock_id),
		'movement' => array(_('&Transactions'), $stock_id) )
	: array(
		'settings' => array(_('&General settings'), $stock_id),
		'sales_pricing' => array(_('S&ales Pricing'), $stock_id),
		'purchase_pricing' => array(_('&Purchasing Pricing'), $stock_id),
		'standard_cost' => array(_('Standard &Costs'), $stock_id),
		'reorder_level' => array(_('&Reorder Levels'), (is_inventory_item($stock_id) ? $stock_id : null)),
		'movement' => array(_('&Transactions'), $stock_id),
		'status' => array(_('&Status'), (is_inventory_item($stock_id) ? $stock_id : null)),
	);

tabbed_content_start('tabs', $tabs);

	switch (get_post('_tabs_sel')) {
		default:
		case 'settings':
			item_settings($stock_id, $new_item); 
			break;
		case 'sales_pricing':
			$_GET['stock_id'] = $stock_id;
			$_GET['page_level'] = 1;
			include_once($path_to_root."/inventory/prices.php");
			break;
		case 'purchase_pricing':
			$_GET['stock_id'] = $stock_id;
			$_GET['page_level'] = 1;
			include_once($path_to_root."/inventory/purchasing_data.php");
			break;
		case 'standard_cost':
			$_GET['stock_id'] = $stock_id;
			$_GET['page_level'] = 1;
			include_once($path_to_root."/inventory/cost_update.php");
			break;
		case 'reorder_level':
			if (!is_inventory_item($stock_id))
			{
				break;
			}	
			$_GET['page_level'] = 1;
			$_GET['stock_id'] = $stock_id;
			include_once($path_to_root."/inventory/reorder_level.php");
			break;
		case 'movement':
			$_GET['stock_id'] = $stock_id;
			include_once($path_to_root."/inventory/inquiry/stock_movements.php");
			break;
		case 'status':
			$_GET['stock_id'] = $stock_id;
			include_once($path_to_root."/inventory/inquiry/stock_status.php");
			break;
	};

br();
tabbed_content_end();

div_end();

hidden('fixed_asset', get_post('fixed_asset'));

if (get_post('fixed_asset'))
	hidden('mb_flag', 'F');

end_form();

//------------------------------------------------------------------------------------

end_page();
