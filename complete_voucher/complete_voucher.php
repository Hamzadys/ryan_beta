<?php

$page_security = 'SA_SALESINVOICE';
$path_to_root = "..";
//include_once($path_to_root . "/sales/includes/cart_class.inc");
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/data_checks.inc");
//include_once($path_to_root . "/includes/manufacturing.inc");
include_once($path_to_root . "/sales/includes/sales_db.inc");
include_once($path_to_root . "/admin/db/fiscalyears_db.inc"); //asad
include_once($path_to_root . "/sales/includes/sales_ui.inc");
include_once($path_to_root . "/reporting/includes/reporting.inc");
//include_once($path_to_root . "/taxes/tax_calc.inc");
include_once($path_to_root . "/includes/ui/items_cart.inc");
include_once($path_to_root . "/complete_voucher/db/cust_trans_db_voucher.inc");
include_once($path_to_root . "/complete_voucher/db/cust_trans_details_db_voucher.inc");
include_once($path_to_root . "/complete_voucher/db/sales_order_db_voucher.inc");


$js = "";
if ($use_popup_windows) {
	$js .= get_js_open_window(900, 500);
}
if ($use_date_picker) {
	$js .= get_js_date_picker();
}

$_SESSION['page_title'] = _($help_context = "Transfer Vouchers");

page($_SESSION['page_title'], false, false, "", $js);

//-----------------------------------------------------------------------------
global $Refs, $systypes_array, $SysPrefs;

if (isset($_GET['AddedID'])) 
{
	$order_no = $_GET['AddedID'];

	display_notification_centered(sprintf(_($systypes_array[$_SESSION['Type']]." # %d has been entered"), $order_no));

	hyperlink_params($path_to_root."/complete_voucher/inquiry/complete_voucher_inquiry.php", _("Select Another For &Batch"), "");

	display_footer_exit();
}

if (isset($_GET['BatchConfirm']))
{
	$src = $_SESSION['DeliveryBatch'];
}

if(isset($_POST['CancelOrder']))
{
	meta_forward($path_to_root . '/complete_voucher/inquiry/complete_voucher_inquiry.php','');
}

if ( isset($_POST['process_invoice']) ) {

	processing_start();
	foreach ($_SESSION['DeliveryBatch'] as $val)
	{
		if ($_SESSION['Type'] == ST_SALESINVOICE)
		{

			$debtor_trans = get_customer_trans_voucher($val, $_SESSION['Type']);
			
			$debtor_trans_details = get_customer_trans_details_voucher($debtor_trans['type'], $debtor_trans['trans_no']);

			$sales_order = get_sales_order_header_voucher($debtor_trans['order_'], ST_SALESORDER);

			$sales_order_details = get_sales_order_details_voucher($sales_order['order_no'], $sales_order['trans_type']);



			$trans_no = add_sales_order_voucher($sales_order, $sales_order_details);

			commit_transaction();

			unset($_SESSION['DeliveryBatch']);
			unset($_SESSION['TransToDate']);

			meta_forward($_SERVER['PHP_SELF'], "AddedID=".$trans_no);
		}
		elseif($_SESSION['Type'] == ST_BANKPAYMENT ||
			$_SESSION['Type'] == ST_BANKDEPOSIT ||
			$_SESSION['Type'] == ST_CPV ||
			$_SESSION['Type'] == ST_CRV)
		{

			$invoice_detail = array();

			$fydetail = get_current_fiscalyear();
			$tax_rate = $fydetail['tax_rate'];

//	 add_batch_data($_SESSION['date_']);

			if ($_SESSION['Type'] != '') {
				$Type = $_SESSION['Type'];

				/*if ($_SESSION['Type'] == ST_SALESINVOICE)
					$myrow = get_vouchers_from_debtor_trans($Type, $val);
				else*/if ($_SESSION['Type'] == ST_BANKPAYMENT ||
					$_SESSION['Type'] == ST_BANKDEPOSIT ||
					$_SESSION['Type'] == ST_CPV ||
					$_SESSION['Type'] == ST_CRV)
					$myrow = get_vouchers_from_bank_trans($Type, $val);

//		S.H
				$company_data = get_company_prefs();
//		$trans_no = get_next_trans_no($Type);
				$ref = $Refs->get_next($Type);
//		$Refs->save($Type, $trans_no, $ref);

				$Total_Amount = round($myrow['amount']);
				$date_ = sql2date($myrow['trans_date']);
//		$trans_no = write_customer_trans($Type, 0, $myrow['debtor_no'],
//		$myrow['branch_code'], $date_, $ref, $Total_Amount);

				$cart = new items_cart($Type);

//		$cart->order_id = $trans_no;
				$do_exchange_variance = false;
				$bank_trans = db_fetch(get_bank_trans_for_complete_invoice($Type, $val));
				$currency = get_bank_account_currency_for_c_i($bank_trans['bank_act']);
				$bank_gl_account = get_bank_gl_account_for_c_i($bank_trans['bank_act']);
				$memo_ = get_comments_string_for_C_I($Type, $val);
				$aid = 0;

				if ($trans_no)
				{
					$old_trans = $trans_no;
//			$Refs->restore_last($Type, $val);
					$aid = has_attachment_for_C_I($Type, $val);
				}
				else
					$old_trans = false;

				$result = get_gl_trans_for_complete_voucher($Type, $val);
				if ($result) {
					while ($row = db_fetch($result)) {
						if (is_bank_account($row['account'])) {
							// date exchange rate is currenly not stored in bank transaction,
							// so we have to restore it from original gl amounts
							$ex_rate = $bank_trans['amount'] / $row['amount'];
						} else {
							$cart->add_gl_item($row['account'], $row['dimension_id'],
								$row['dimension2_id'], $row['amount'], $row['memo_']);
						}
					}
				}

				$total_amount = $cart->gl_items_total();

				if ($bank_trans['person_type_id'] == PT_CUSTOMER)
				{
					// we need to add a customer transaction record
					// convert to customer currency
					if (!isset($settled_amount)) // leaved for backward/ext compatibility
						$cust_amount = exchange_from_to(abs($total_amount), $currency, get_customer_currency($bank_trans['person_id']), $date_);
					else
						$cust_amount = $total_amount;

					if ($Type == ST_BANKPAYMENT)
						$cust_amount = -$total_amount;

					$trans_no = write_customer_trans($Type, 0, $bank_trans['person_id'], $bank_trans['person_type_id'], $date_,
						$ref, $cust_amount);

					if ($old_trans)
						move_trans_attachments($Type, $old_trans, $trans_no);
				}
				elseif ($bank_trans['person_type_id'] == PT_SUPPLIER) {
					// we need to add a supplier transaction record
					// convert to supp currency
					if (!isset($settled_amount)) // leaved for for backward/ext compatibility
						$supp_amount = exchange_from_to_for_C_I(abs($total_amount), $currency, get_supplier_currency($bank_trans['person_id']), $date_);
					else
						$supp_amount = $settled_amount;

					if ($Type == ST_BANKPAYMENT || $Type == ST_CPV)
						$supp_amount = -$supp_amount;

					$trans_no = write_supp_trans($Type, 0, $bank_trans['person_id'], $date_, '',
						$ref, "", $supp_amount, 0, 0);
					if ($old_trans)
						move_trans_attachments($Type, $old_trans, $trans_no);
				}
				else
				{
					$trans_no = get_next_trans_no($Type);
					$do_exchange_variance = $SysPrefs->auto_currency_revaluation();
					if ($do_exchange_variance)
						$trans_no1 = get_next_trans_no(ST_JOURNAL);
				}

				add_bank_trans($Type, $trans_no, $bank_trans['bank_act'], $ref,
					$date_, -$total_amount,
					$bank_trans['person_type_id'], $bank_trans['person_id'],
					$currency,
					"Cannot insert a source bank transaction");

				$exchanged = false;
				$total = 0;
				foreach ($cart->gl_items as $gl_item)
				{
					$is_bank_to = is_bank_account($gl_item->code_id);

					if ($Type == ST_BANKPAYMENT AND $is_bank_to)
					{
						// we don't allow payments to go to a bank account. use transfer for this !
						display_db_error("invalid payment entered. Cannot pay to another bank account", "");
					}
					// do the destination account postings
					$total += add_gl_trans($Type, $trans_no, $date_, $gl_item->code_id,
						$gl_item->dimension_id, $gl_item->dimension2_id, $gl_item->reference,
						$gl_item->amount, $currency, $bank_trans['person_type_id'], $bank_trans['person_id']);

					/*if ($is_bank_to)
                    {
                        add_bank_trans($Type, $trans_no, $is_bank_to, $ref,
                            $date_, $gl_item->amount,
                            $bank_trans['person_type_id'], $bank_trans['person_id'], $currency,
                            "Cannot insert a destination bank transaction");
                        if ($do_exchange_variance)
                        {
                            add_exchange_variation($trans_no1, $date_, $is_bank_to, $gl_item->code_id,
                                $currency, $bank_trans['person_type_id'], $bank_trans['person_id']);
                        }
                    }*/
					// store tax details if the gl account is a tax account

					$amount = $gl_item->amount;
					$ex_rate = get_exchange_rate_from_home_currency($currency, $date_);

					add_gl_tax_details($gl_item->code_id, $Type, $trans_no, -$amount,
						$ex_rate, $date_, $memo_);
					update_invoices($Type, $bank_trans['trans_no']);
				}

				add_gl_trans($Type, $trans_no, $date_, $bank_gl_account, 0, 0, $memo_,
					-$total, null, $bank_trans['person_type_id'], $bank_trans['person_id']);

				if ($do_exchange_variance)
				{
					if ($exchanged || add_exchange_variation($trans_no1, $date_, $bank_trans['bank_act'], $bank_gl_account,
							$currency, $bank_trans['person_type_id'], $bank_trans['person_id']))
					{
						$ref1 = $Refs->get_next(ST_JOURNAL, null, $date_);
						$Refs->save(ST_JOURNAL, $trans_no1, $ref1);
						add_audit_trail(ST_JOURNAL, $trans_no1, $date_);
					}
				}

				add_comments($Type, $trans_no, $date_, $memo_);

				$Refs->save($Type, $trans_no, $ref);
				add_audit_trail($Type, $trans_no, $date_);

				$invoice_detail[$trans_no] = $ref;
			}
			else
				display_error("Please go back and make a batch again.");


			commit_transaction();

			unset($_SESSION['DeliveryBatch']);
			unset($_SESSION['TransToDate']);

			meta_forward($_SERVER['PHP_SELF'], "AddedID=".$trans_no);
		}
		else
			display_error("Please go back and make a batch again.");
	}
}

if(count($src) == 0)
{
	display_error(_("There are no invoices for processing."));
	display_footer_exit();
	unset($_SESSION['DeliveryBatch']);
	unset($_SESSION['TransToDate']);
}

start_form();

div_start('Items');

display_heading(_($systypes_array[$_SESSION['Type']]));

start_table(TABLESTYLE, "width=60%");

$th = array(_("#"), _("Customer"), _("Date"), _("Amount")/*, _("Batch No")*/);

table_header($th);
$k = 0;
$has_marked = false;
$show_qoh = true;
$dn_line_cnt = 0;

foreach($src as $val)
{
	alt_table_row_color($k);

	if($_SESSION['Type'] == ST_SALESINVOICE)
	{
		$myrow = get_vouchers_from_debtor_trans($_SESSION['Type'], $val);

		label_cell($myrow['trans_no']);

		label_cell($myrow['name']);

		label_cell(sql2date($myrow['tran_date']));

		amount_cell($myrow['ov_amount']);

		$_SESSION['date_'] = sql2date($myrow['trans_date']);
	}
	elseif($_SESSION['Type'] == ST_BANKPAYMENT ||
			$_SESSION['Type'] == ST_BANKDEPOSIT ||
			$_SESSION['Type'] == ST_CPV ||
			$_SESSION['Type'] == ST_CRV)
	{
		$myrow = get_vouchers_from_bank_trans($_SESSION['Type'], $val);

		label_cell($myrow['trans_no']);

		if($myrow['person_type_id'] == 2)
			label_cell(get_customer_name($myrow['person_id']));
		elseif($myrow['person_type_id'] == 3)
			label_cell(get_supplier_name_voucher($myrow['person_id']));

		label_cell(sql2date($myrow['trans_date']));

		amount_cell($myrow['amount']);

		$_SESSION['date_'] = sql2date($myrow['trans_date']);
	}
	end_row();		
}
end_table(1);

submit_center_first('process_invoice',  _("Process"),
	_('Check entered data and save document'), 'default');

submit_center_last('CancelOrder',  _("Cancel"),
	_('Cancels document entry or removes sales order when editing an old document'));
 
div_end(); 
end_form();   

end_page();

?>