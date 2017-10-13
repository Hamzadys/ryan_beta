<?php


$page_security = 'SA_COMPLETEINVOICE';
$path_to_root="../..";

include($path_to_root . "/includes/db_pager.inc");
include_once($path_to_root . "/includes/session.inc");

include_once($path_to_root . "/includes/date_functions.inc");
include_once($path_to_root . "/includes/ui.inc");
$js = "";
if ($SysPrefs->use_popup_windows)
	$js .= get_js_open_window(800, 500);
if (user_use_date_picker())
	$js .= get_js_date_picker();

page(_($help_context = "Complete Voucher"), false, false, "", $js);

//-----------------------------------------------------------------------------------
// Ajax updates
//
if (get_post('Search'))
{
	$Ajax->activate('journal_tbl');
}
//--------------------------------------------------------------------------------------
if (!isset($_POST['filterType']))
	$_POST['filterType'] = -1;

start_form();

start_table(TABLESTYLE_NOBORDER);
start_row();

ref_cells(_("Reference:"), 'Ref', '',null, _('Enter reference fragment or leave empty'));

journal_types_list_cells(_("Type:"), "filterType", null, true);
date_cells(_("From:"), 'FromDate', '', null, 0, -1, 0);
date_cells(_("To:"), 'ToDate');

end_row();
start_row();
ref_cells(_("Memo:"), 'Memo', '',null, _('Enter memo fragment or leave empty'));
users_list_cells(_("User:"), 'userid', null, false);
if (get_company_pref('use_dimension') && isset($_POST['dimension'])) // display dimension only, when started in dimension mode
	dimensions_list_cells(_('Dimension:'), 'dimension', null, true, null, true);
//check_cells( _("Show closed:"), 'AlsoClosed', null);
submit_cells('Search', _("Search"), '', '', 'default');
end_row();
end_table();

function journal_pos($row)
{
	return $row['gl_seq'] ? $row['gl_seq'] : '-';
}

function systype_name($dummy, $type)
{
	global $systypes_array;

	return $systypes_array[$type];
}

function view_link($row) 
{
	return get_trans_view_str($row["trans_type"], $row["trans_no"]);
}

function gl_link($row) 
{
	return get_gl_view_str($row["trans_type"], $row["trans_no"]);
}

function edit_link($row)
{

	$ok = true;
	if ($row['trans_type'] == ST_SALESINVOICE)
	{
		$myrow = get_customer_trans($row["trans_no"], $row["trans_type"]);
		if ($myrow['alloc'] != 0 || get_voided_entry(ST_SALESINVOICE, $row["trans_no"]) !== false)
			$ok = false;
	}
	return $ok ? trans_editor_link( $row["trans_type"], $row["trans_no"]) : '';
}

function invoice_supp_reference($row)
{
	return $row['supp_reference'];
}

function batch_checkbox($row)
{
	
	{
		$name = "Sel_" . $row['trans_no'];
		return
			"<input type='checkbox' name='$name' value='1' >"
			// add also trans_no => branch code for checking after 'Batch' submit
			. "<input name='Sel_[" . $row['trans_no'] . "]' type='hidden' value='"
			. $row['trans_type'] . "'>\n";
	}
}

function selectAll($label=null, $name='', $onchange=''){
	$str = "";

	if($label != null)
		$str .= "<label> All</label>";

	$str .= "<input type='checkbox' name='".$name."' onchange='".$onchange."'>";

	return $str;
}


if (isset($_POST['BatchInvoice']))
{
	$_SESSION['Type'] = $_POST['filterType'];
	$Allow = 0;
	if($_POST['filterType'] == -1)
	{
		display_error("Type must be select.");
		$Allow = 1;
		set_focus('filterType');
	}
	if($Allow == 0)
	{
		$del_count = 0;
		foreach($_POST['Sel_'] as $delivery => $branch)
		{
			$checkbox = 'Sel_'.$delivery;
			if(check_value($checkbox))	{
				if(!$del_count) {
					$del_branch = $branch;
				}
				else {
					if ($del_branch != $branch)	{
						$del_count=0;
						break;
					}
				}
				$selected[] = $delivery;
				$del_count++;
			}
		}
		if (!$del_count) {
			display_error(_('All Type must be same.'));
			set_focus('filterType');
		} else {
			$_SESSION['DeliveryBatch'] = $selected;
			meta_forward($path_to_root . '/complete_voucher/complete_voucher.php','BatchConfirm=Yes');
		}
	}
}

$sql = get_rpl_data_for_journal_inquiry(get_post('filterType', -1), get_post('FromDate'),
	get_post('ToDate'), get_post('Ref'), get_post('Memo'), true);

$cols = array(
	_("#") => array('fun'=>'journal_pos', 'align'=>'center'), 
	_("Date") =>array('name'=>'tran_date','type'=>'date','ord'=>'desc'),
	_("Type") => array('fun'=>'systype_name'), 
	_("Trans #") => array('fun'=>'view_link'), 
 	_("Counterparty") => array('ord' => ''),
	_("Supplier's Reference") => 'skip',
	_("Reference"), 
	_("Amount") => array('type'=>'amount'),
	_("Memo"),
	_("User"),
	_("View") => array('insert'=>true, 'fun'=>'gl_link'),
//	array(
//		'insert'=>true, 'fun'=>'edit_link')
	submit('BatchInvoice',_("Batch"), false, _("Transfer")).selectAll('Select All', 'selectAll', 'checkAll(this)')
	=> array('insert'=>true, 'fun'=>'batch_checkbox', 'align'=>'center'),
);

if (!check_value('AlsoClosed')) {
	$cols[_("#")] = 'skip';
}


if (isset($_SESSION['Batch']))
{
	foreach($_SESSION['Batch'] as $trans=> $del)
		unset($_SESSION['Batch'][$trans]);
	unset($_SESSION['Batch']);
}

if($_POST['filterType'] == ST_SUPPINVOICE) //add the payment column if shown supplier invoices only
{
	$cols[_("Supplier's Reference")] = array('fun'=>'invoice_supp_reference', 'align'=>'center');
}

$table =& new_db_pager('journal_tbl', $sql, $cols);

$table->width = "80%";

display_db_pager($table);

end_form();
end_page();

?>
<script type="text/javascript">
 function checkAll(ele) {
	 var checkboxes = document.getElementsByTagName('input');
	 if (ele.checked) {
		 for (var i = 0; i < checkboxes.length; i++) {
			 if (checkboxes[i].type == 'checkbox') {
				 checkboxes[i].checked = true;
			 }
		 }
     } else {
		 for (var i = 0; i < checkboxes.length; i++) {
			 if (checkboxes[i].type == 'checkbox') {
				 checkboxes[i].checked = false;
			 }
		 }
     }
 }
</script>