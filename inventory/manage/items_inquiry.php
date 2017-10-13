<?php

$page_security = 'SA_SALESTRANSVIEW';
$path_to_root = "../../";
include_once($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/sales/includes/sales_ui.inc");
include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/inventory/includes/db/items_db.inc");
include_once($path_to_root . "/reporting/includes/reporting.inc");

$js = "";
if ($SysPrefs->use_popup_windows)
	$js .= get_js_open_window(900, 500);
if (user_use_date_picker())
	$js .= get_js_date_picker();
page(_($help_context = "Items Inquiry"), isset($_GET['customer_id']), false, "", $js);

if (isset($_GET['customer_id']))
{
	$_POST['customer_id'] = $_GET['customer_id'];
}

//------------------------------------------------------------------------------------------------

start_form();

start_table(TABLESTYLE_NOBORDER);
start_row();

ref_cells(_("Item Code"), 'stock_id', '',null, '', true);
ref_cells(_("Name"), 'description', '',null, '', true);
ref_cells(_("Packing"), 'carton', '',null, '', true);
ref_cells(_("Description"), 'long_description', '',null, '', true);
end_row();
end_table();

start_table(TABLESTYLE_NOBORDER);
start_row();
stock_categories_list_cells(_("Category:"), 'category_id', null, true);
item_tax_types_list_cells(_("Item Tax Type:"), 'tax_type_id', null, true);
sales_persons_list_cells( _("Sales Person:"), 'salesman', null, true);
end_row();
end_table();

start_table(TABLESTYLE_NOBORDER);
start_row();
stock_item_types_list_cells(_("Item Type:"), 'mb_flag', null, true);
stock_units_list_cells(_('Units of Measure:'), 'units', null, true);
end_row();
end_table();

start_table(TABLESTYLE_NOBORDER);
start_row();
dimensions_list_cells("Dimension1 :", 'dimension_id', null, true, " ", 1);
dimensions_list_cells("Dimension2 :", 'dimension2_id', null, true, " ", 2);

end_row();
end_table();

start_table(TABLESTYLE_NOBORDER);
start_row();

if (!@$_GET['popup'])


	if ($trans_type == ST_SALESQUOTE)
		check_cells(_("Show All:"), 'show_all');

submit_cells('SearchOrders', _("Search"),'',_('Select documents'), 'default');
hidden('order_view_mode', $_POST['order_view_mode']);
hidden('type', $trans_type);

end_row();

end_table(1);


set_global_customer($_POST['customer_id']);

//------------------------------------------------------------------------------------------------


function systype_name($dummy, $type)
{
	global $systypes_array;

	return $systypes_array[$type];
}

function order_view($row)
{
	return $row['order_']>0 ?
		get_customer_trans_view_str(ST_SALESORDER, $row['order_'])
		: "";
}

function trans_view($trans)
{
	return get_trans_view_str($trans["type"], $trans["trans_no"]);
}

function due_date($row)
{
	return	$row["type"] == ST_SALESINVOICE	? $row["due_date"] : '';
}

function gl_view($row)
{
	return get_gl_view_str($row["type"], $row["trans_no"]);
}
//ansar 26-08-2017
function fmt_amount($row)
{
	$value =
		$row['type']==ST_CUSTCREDIT || $row['type']==ST_CUSTPAYMENT || $row['type']==ST_BANKDEPOSIT || $row['type']==ST_JOURNAL ?
			-$row["TotalAmount"] : $row["TotalAmount"];
	return price_format($value);
}
function fmt_debit($row)
{
//dz 16.6.17
	/*
    $value =
            $row['type']==ST_CUSTCREDIT || $row['type']==ST_CUSTPAYMENT || $row['type']==ST_BANKDEPOSIT  || $row['type'] == ST_CRV || $row['type']==ST_JOURNAL ?
            -$row["TotalAmount"] : $row["TotalAmount"];
    */
	$value =
		$row['type']==ST_CUSTCREDIT || $row['type']==ST_CUSTPAYMENT || $row['type']==ST_BANKDEPOSIT  || $row['type'] == ST_CRV ?
			-$row["TotalAmount"] : $row["TotalAmount"];
	return $value>=0 ? price_format($value) : '';

}

function fmt_credit($row)
{
//dz 16.6.17
	/*
    $value =
            !($row['type']==ST_CUSTCREDIT || $row['type']==ST_CUSTPAYMENT || $row['type']==ST_BANKDEPOSIT || $row['type'] == ST_CRV || $row['type']==ST_JOURNAL) ?
            -$row["TotalAmount"] : $row["TotalAmount"];
    */
	$value =
		!($row['type']==ST_CUSTCREDIT || $row['type']==ST_CUSTPAYMENT || $row['type']==ST_BANKDEPOSIT || $row['type'] == ST_CRV) ?
			-$row["TotalAmount"] : $row["TotalAmount"];
	return $value>0 ? price_format($value) : '';
}


function credit_link($row)
{
	global $page_nested;

	if ($page_nested)
		return '';
	return $row['type'] == ST_SALESINVOICE && $row["Outstanding"] > 0 ?
		pager_link(_("Credit This") ,
			"/sales/customer_credit_invoice.php?InvoiceNumber=". $row['trans_no'], ICON_CREDIT):'';
}

function edit_link($row)
{
	if (@$_GET['popup'])
		return '';
	global $trans_type;
	$modify = ($trans_type == ST_SALESORDER ? "ModifyOrderNumber" : "ModifyQuotationNumber");
	return pager_link( _("Edit"),
		"/inventory/manage/items.php?stock_id=" . $row['stock_id'], ICON_EDIT);
}

function prt_link($row)
{
	if ($row['type'] == ST_CUSTPAYMENT || $row['type'] == ST_BANKDEPOSIT || $row['type'] == ST_CPV || $row['type'] == ST_CRV)
		return print_document_link($row['trans_no']."-".$row['type'], _("Print Receipt"), true, ST_CUSTPAYMENT, ICON_PRINT);
	elseif ($row['type'] == ST_BANKPAYMENT) // bank payment printout not defined yet.
		return '';
	else
		return print_document_link($row['trans_no']."-".$row['type'], _("Print"), true, $row['type'], ICON_PRINT);
}

function check_overdue($row)
{
	return $row['OverDue'] == 1
	&& floatcmp($row["TotalAmount"], $row["Allocated"]) != 0;
}
////-------------------------------marina----------------------------
function get_category_id_name($category_id)
{
	$sql = "SELECT description FROM ".TB_PREF."stock_category WHERE category_id=".db_escape($category_id['category_id']);

	$result = db_query($sql, "could not get sales type");

	$row = db_fetch_row($result);
	return $row[0];
}
////////////
function get_editable_desc($row)
{
	if ($row["editable"] == 0)
	{
		$a='NO';
	}
	else
	{
		$a='YES';
	}
	return $a;
}

function get_exclude_sale($row)
{
	if ($row["no_sale"] == 0)
	{
		$a='NO';
	}
	else
	{
		$a='YES';
	}
	return $a;
}

function get_exclude_purchase($row)
{
	if ($row["no_purchase"] == 0)
	{
		$a='NO';
	}
	else
	{
		$a='YES';
	}
	return $a;
}

function get_salesman_id_name($salesman)
{
	$sql = "SELECT salesman_name FROM ".TB_PREF."salesman WHERE salesman_code=".db_escape($salesman['salesman']);

	$result = db_query($sql, "could not get sales type");

	$row = db_fetch_row($result);
	return $row[0];
}

function get_sales_account($sales_acc)
{
	$sql = "SELECT account_name FROM ".TB_PREF."chart_master WHERE account_code=".db_escape($sales_acc['sales_account']);

	$result = db_query($sql, "could not get gl account");
	$row= db_fetch_row($result);
	return $row[0];
}

function get_inventory_account($invent_acc)
{
	$sql = "SELECT account_name FROM ".TB_PREF."chart_master WHERE account_code=".db_escape($invent_acc['inventory_account']);

	$result = db_query($sql, "could not get gl account");
	$row= db_fetch_row($result);
	return $row[0];
}

function get_cogs_account($cogs_acc)
{
	$sql = "SELECT account_name FROM ".TB_PREF."chart_master WHERE account_code=".db_escape($cogs_acc['cogs_account']);

	$result = db_query($sql, "could not get gl account");
	$row= db_fetch_row($result);
	return $row[0];
}

function get_adjustment_account($adjust_acc)
{
	$sql = "SELECT account_name FROM ".TB_PREF."chart_master WHERE account_code=".db_escape($adjust_acc['adjustment_account']);

	$result = db_query($sql, "could not get gl account");
	$row= db_fetch_row($result);
	return $row[0];
}

function get_item_tax_types($tax_type_id)
{
	$sql = "SELECT name FROM ".TB_PREF."item_tax_types WHERE id=".db_escape($tax_type_id['tax_type_id']);

	$result = db_query($sql, "could not get tax type rate");

	$row = db_fetch_row($result);
	return $row[0];
}

function get_item_unit_name($unit_id)
{
	$sql="SELECT name FROM ".TB_PREF."item_units WHERE abbr=".db_escape($unit_id['units']);

	$result = db_query($sql, "could not get tax type rate");

	$row = db_fetch_row($result);
	return $row[0];
}
//------------------------------------------------------------------------------------------------


$sql = get_sql_for_items_log_view($_POST['stock_id'], $_POST['description'],
	$_POST['carton'], $_POST['long_description'], $_POST['category_id'], $_POST['tax_type_id'],
	$_POST['mb_flag'], $_POST['units'], $_POST['editable'], $_POST['no_sale'], $_POST['no_purchase'],
	$_POST['salesman'], $_POST['sales_account'], $_POST['inventory_account'], $_POST['cogs_account'],
	$_POST['adjustment_account'], $_POST['dimension_id'],
	$_POST['dimension2_id']);


//------------------------------------------------------------------------------------------------
db_query("set @bal:=0");
$cols = array(
	_("Item Code"),
	_("Name"),
	_("Packing"),
	_("Description"),
	_("Category") => array('fun'=>'get_category_id_name'),
	_("Item Tax Type") => array('fun'=>'get_item_tax_types'),
	_("Item Type"),
	_("Units of Measure") => array('fun'=>'get_item_unit_name'),
	_("Editable description") => array('fun'=>'get_editable_desc'),
	_("Exclude from sales") => array('fun'=>'get_exclude_sale'),
	_("Exclude from purchases") => array('fun'=>'get_exclude_purchase'),
	_("Sales Person") => array('fun'=>'get_salesman_id_name'),
	_("Sales Account") => array('fun'=>'get_sales_account'),
	_("Inventory Account") => array('fun'=>'get_inventory_account'),
	_("C.O.G.S. Account") => array('fun'=>'get_cogs_account'),
	_("Inventory Adjustments Account") => array('fun'=>'get_adjustment_account'),
	_("Dimension 1") => array('fun'=>'get_dimension_name'),
	_("Dimension 2") => array('fun'=>'get_dimension2_name'),
	array('insert'=>true, 'fun'=>'edit_link')
);

$table =& new_db_pager('orders_tbl', $sql, $cols);
$table->set_marker('check_overdue', _("Marked items are overdue."));

$table->width = "80%";
echo '<center><a href="items.php" target="_blank"><input type="button" value="Add Item"></a></center>';

display_db_pager($table);
//submit_center('Update', _("Update"), true, '', null);


if (!@$_GET['popup'])
{
	end_form();
	end_page();
}
?>
