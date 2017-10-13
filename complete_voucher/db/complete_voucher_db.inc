<?php

function get_vouchers_from_bank_trans($trans_type, $order_no)
{
	$sql = "SELECT trans.*
			FROM 0_bank_trans trans
		WHERE trans.type = " . db_escape($trans_type) ."
		AND trans.trans_no = " . db_escape($order_no);

	$result = db_query($sql, "Error");

	return db_fetch($result);
}
function get_vouchers_from_debtor_trans($trans_type, $order_no)
{
	$sql = "SELECT trans.*, master.name
			FROM 0_debtor_trans trans, 0_debtors_master master
		WHERE trans.debtor_no = master.debtor_no
		AND trans.type = " . db_escape($trans_type) ."
		AND trans.trans_no = " . db_escape($order_no);

	$result = db_query($sql, "Error");

	return db_fetch($result);
}
// #1 
function write_customer_trans_complete_voucher($trans_type, $trans_no, $debtor_no, $BranchNo,
	$date_, $reference, $Total, $discount=0, $Tax=0, $Freight=0, $FreightTax=0,
	$sales_type=0, $order_no=0, $ship_via=0, $due_date="",
	$AllocAmt=0, $rate=0, $dimension_id=0, $dimension2_id=0, $payment_terms=null, 
	$tax_included=0, $prep_amount=0,$supply_disc, $service_disc, $fbr_disc, $srb_disc, $write_back,
	$supply_tax_id, $service_tax_id, $fbr_tax_id, $srb_tax_id)
{
	$new = $trans_no==0;
	$curr = get_customer_currency($debtor_no);
	if ($rate == 0)
		$rate = get_exchange_rate_from_home_currency($curr, $date_);

	$SQLDate = date2sql($date_);
	if ($due_date == "")
		$SQLDueDate = "0000-00-00";
	else
		$SQLDueDate = date2sql($due_date);

	if ($trans_type == ST_BANKPAYMENT)
		$Total = -$Total;

	if ($new || !exists_customer_trans($trans_type, $trans_no))
	{
		if ($new)
			$trans_no = get_next_trans_no($trans_type);

		$sql = "INSERT INTO ".TB_PREF."debtor_trans (
		trans_no, type,
		debtor_no, branch_code,
		tran_date, due_date,
		reference, tpe,
		order_, ov_amount, ov_discount,
		ov_gst, ov_freight, ov_freight_tax,
		rate, ship_via, alloc,
		dimension_id, dimension2_id, payment_terms, tax_included, prep_amount,
		supply_disc, service_disc, fbr_disc, srb_disc, write_back,
        supply_tax_id, service_tax_id, fbr_tax_id, srb_tax_id
		) VALUES (".db_escape($trans_no).", ".db_escape($trans_type).",
		".db_escape($debtor_no).", ".db_escape($BranchNo).",
		'$SQLDate', '$SQLDueDate', ".db_escape($reference).",
		".db_escape($sales_type).", ".db_escape($order_no).", $Total, ".db_escape($discount).", $Tax,
		".db_escape($Freight).",
		$FreightTax, $rate, ".db_escape($ship_via).", $AllocAmt,
		".db_escape($dimension_id).", ".db_escape($dimension2_id).", "
		.db_escape($payment_terms, true).", "
		.db_escape($tax_included).", ".db_escape($prep_amount)."
		, ".db_escape($supply_disc)."
		, ".db_escape($service_disc)."
		, ".db_escape($fbr_disc)."
		, ".db_escape($srb_disc)."
		, ".db_escape($write_back)."
		, ".db_escape($supply_tax_id)."
		, ".db_escape($service_tax_id)."
		, ".db_escape($fbr_tax_id)."
		, ".db_escape($srb_tax_id).")";
	}
	else
	{	// may be optional argument should stay unchanged ?
		$sql = "UPDATE ".TB_PREF."debtor_trans SET
		debtor_no=".db_escape($debtor_no)." , branch_code=".db_escape($BranchNo).",
		tran_date='$SQLDate', due_date='$SQLDueDate',
		reference=".db_escape($reference).", tpe=".db_escape($sales_type).", order_=".db_escape($order_no).",
		ov_amount=$Total, ov_discount=".db_escape($discount).", ov_gst=$Tax,
		ov_freight=".db_escape($Freight).", ov_freight_tax=$FreightTax, rate=$rate,
		ship_via=".db_escape($ship_via).", alloc=$AllocAmt,
		dimension_id=".db_escape($dimension_id).", dimension2_id=".db_escape($dimension2_id).",
		payment_terms=".db_escape($payment_terms, true).",
		tax_included=".db_escape($tax_included).",
		prep_amount =".db_escape($prep_amount)."
		WHERE trans_no=".db_escape($trans_no)." 
		AND type=".db_escape($trans_type);
	}
	db_query($sql, "The debtor transaction record could not be inserted");

	if ($trans_type != ST_JOURNAL) // skip for journal entries
		add_audit_trail($trans_type, $trans_no, $date_, $new ? '': _("Updated."));

	return $trans_no;
}

// #2
function add_gl_trans_customer_for_complete_voucher($type, $type_no, $date_, $account, $dimension, $dimension2,
							   $amount, $customer_id, $err_msg="", $rate=0)
{
	if ($err_msg == "")
		$err_msg = "The customer GL transaction could not be inserted";

	return add_gl_trans_for_complete_voucher($type, $type_no, $date_, $account, $dimension, $dimension2, "", $amount,
		get_customer_currency($customer_id),
		PT_CUSTOMER, $customer_id, $err_msg, $rate);
}
// #3
function add_gl_trans_for_complete_voucher($type, $trans_id, $date_, $account, $dimension, $dimension2, $memo_,
					  $amount, $currency=null, $person_type_id=null, $person_id=null,	$err_msg="", $rate=0)
{
	global $SysPrefs;

	$date = date2sql($date_);
	if ($currency != null)
	{
		if ($rate == 0)
			$amount_in_home_currency = to_home_currency($amount, $currency, $date_);
		else
			$amount_in_home_currency = round2($amount * $rate,  user_price_dec());
	}
	else
		$amount_in_home_currency = round2($amount, user_price_dec());
	if ($dimension == null || $dimension < 0)
		$dimension = 0;
	if ($dimension2 == null || $dimension2 < 0)
		$dimension2 = 0;
	if (isset($SysPrefs->use_audit_trail) && $SysPrefs->use_audit_trail)
	{
		if ($memo_ == "" || $memo_ == null)
			$memo_ = $_SESSION["wa_current_user"]->username;
		else
			$memo_ = $_SESSION["wa_current_user"]->username . " - " . $memo_;
	}
	if (!is_subledger_account($account) || $account==get_company_pref('grn_clearing_act'))
		$person_id = $person_type_id = null;

	$sql = "INSERT INTO ".TB_PREF."gl_trans ( type, type_no, tran_date,
		account, dimension_id, dimension2_id, memo_, amount";

	if ($person_type_id != null)
		$sql .= ", person_type_id, person_id";

	$sql .= ") ";

	$sql .= "VALUES (".db_escape($type).", ".db_escape($trans_id).", '$date',
		".db_escape($account).", ".db_escape($dimension).", "
		.db_escape($dimension2).", ".db_escape($memo_).", "
		.db_escape($amount_in_home_currency);

	if ($person_type_id != null)
		$sql .= ", ".db_escape($person_type_id).", ". db_escape($person_id);

	$sql .= ") ";

	if ($err_msg == "")
		$err_msg = "The GL transaction could not be inserted";

	db_query($sql, $err_msg);
	return $amount_in_home_currency;
}
// #4
function add_audit_trail_for_complete_voucher($trans_type, $trans_no, $trans_date, $descr='')
{
	begin_transaction();

	$date = date2sql($trans_date);
	$sql = "INSERT INTO 1_audit_trail"
		. " (type, trans_no, user, gl_date, description)
			VALUES(".db_escape($trans_type).", ".db_escape($trans_no).","
		. $_SESSION["wa_current_user"]->user . ","
		. "'$date',". db_escape($descr). ")";

	db_query($sql, "Cannot add audit info");
	// all audit records beside just inserted one should have gl_seq set to NULL
	// to avoid need for subqueries (not existing in MySQL 3) all over the code
	$sql = "UPDATE 1_audit_trail audit LEFT JOIN 1_fiscal_year year ON year.begin<='$date' AND year.end>='$date'
		SET audit.gl_seq = IF(audit.id=".db_insert_id().", 0, NULL),"
		."audit.fiscal_year=year.id"
		. " WHERE type=".db_escape($trans_type)." AND trans_no="
		. db_escape($trans_no);

	db_query($sql, "Cannot update audit gl_seq");
	commit_transaction();
}


function get_rpl_data_for_journal_inquiry($filter, $from, $to, $ref='', $memo='', $alsoclosed=false,
									 $user_id=null, $contractor_id=null, $dimension=null)
{
	$sql = "SELECT IFNULL(a.gl_seq,0) as gl_seq,
		gl.tran_date,
		gl.type as trans_type,
		gl.type_no as trans_no,
		IFNULL(MAX(supp.supp_name), MAX(cust.name)) as name,
		IF(ISNULL(st.supp_reference), '', st.supp_reference) AS supp_reference,
		refs.reference,"
		.($dimension ? " -SUM(IF(dim.dimension in(gl.dimension_id,gl.dimension2_id), gl.amount, 0)) as amount,":" SUM(IF(gl.amount>0, gl.amount,0)) as amount,")
		."com.memo_,
		IF(ISNULL(u.user_id),'',u.user_id) as user_id";

	if ($contractor_id > 0) {
		$sql.= ", st.supplier_id, dt.debtor_no ";
	}

	$sql.= " FROM 0_gl_trans as gl
		 LEFT JOIN 0_audit_trail as a ON
			(gl.type=a.type AND gl.type_no=a.trans_no)
		 LEFT JOIN 0_comments as com ON
			(gl.type=com.type AND gl.type_no=com.id)
		 LEFT JOIN 0_refs as refs ON
			(gl.type=refs.type AND gl.type_no=refs.id)
		 LEFT JOIN 0_users as u ON
			a.user=u.id
	 	 LEFT JOIN 0_debtor_trans dt ON dt.type=gl.type AND gl.type_no=dt.trans_no
	 	 LEFT JOIN 0_debtors_master cust ON gl.person_type_id=2 AND gl.person_id=cust.debtor_no
	 	 LEFT JOIN 0_supp_trans st ON st.type=gl.type AND gl.type_no=st.trans_no
	 	 LEFT JOIN 0_suppliers supp ON gl.person_type_id=3 AND gl.person_id=supp.supplier_id"
		.($dimension ?
			" LEFT JOIN (SELECT type, type_no, MAX(IFNULL(dimension_id, dimension2_id)) dimension FROM 0_gl_trans GROUP BY type, type_no) dim 
		 		ON gl.type=dim.type AND gl.type_no=dim.type_no" : '')
		." WHERE gl.tran_date >= '" . date2sql($from) . "'
		AND gl.tran_date <= '" . date2sql($to) . "'
		AND gl.amount!=0 AND gl.inactive=0";
	if ($ref) {
		$sql .= " AND refs.reference LIKE ". db_escape("%$ref%");
	}
	if ($memo) {
		$sql .= " AND com.memo_ LIKE ". db_escape("%$memo%");
	}
	if ($filter != -1) {
		$sql .= " AND gl.type=".db_escape($filter);
	}
	if (!$alsoclosed) {
		$sql .= " AND gl_seq=0";
	}
	else
		$sql .= " AND NOT ISNULL(a.gl_seq)";

	if ($user_id != null)
		$sql .= " AND user_id = ".db_escape($user_id);

	if ($contractor_id > 0) {
		$sql.= " AND (dt.debtor_no =".$contractor_id;
		$sql.= " OR st.supplier_id =".$contractor_id.") ";
	}

	if ($dimension != null)
		$sql .= " AND dim.dimension = ".db_escape($dimension);

	$sql .= " GROUP BY gl.tran_date, a.gl_seq, gl.type, gl.type_no";

	return $sql;
}
//--------------------------------------------------------------------------------

function get_gl_trans_for_complete_voucher($type, $trans_id)
{
	$sql = "SELECT gl.*, cm.account_name, IFNULL(refs.reference, '') AS reference, user.real_name, 
			COALESCE(st.tran_date, dt.tran_date, bt.trans_date, grn.delivery_date, gl.tran_date) as doc_date,
			IF(ISNULL(st.supp_reference), '', st.supp_reference) AS supp_reference
	FROM 0_gl_trans as gl
		LEFT JOIN 0_chart_master as cm ON gl.account = cm.account_code
		LEFT JOIN 0_refs as refs ON (gl.type=refs.type AND gl.type_no=refs.id)
		LEFT JOIN 0_audit_trail as audit ON (gl.type=audit.type AND gl.type_no=audit.trans_no AND NOT ISNULL(gl_seq))
		LEFT JOIN 0_users as user ON (audit.user=user.id)
	# all this below just to retrieve doc_date :>
		LEFT JOIN 0_supp_trans st ON gl.type_no=st.trans_no AND st.type=gl.type AND (gl.type!=".ST_JOURNAL." OR gl.person_id=st.supplier_id)
		LEFT JOIN 0_grn_batch grn ON grn.id=gl.type_no AND gl.type=".ST_SUPPRECEIVE." AND gl.person_id=grn.supplier_id
		LEFT JOIN 0_debtor_trans dt ON gl.type_no=dt.trans_no AND dt.type=gl.type AND (gl.type!=".ST_JOURNAL." OR gl.person_id=dt.debtor_no)
		LEFT JOIN 0_bank_trans bt ON bt.type=gl.type AND bt.trans_no=gl.type_no AND bt.amount!=0
			 AND bt.person_type_id=gl.person_type_id AND bt.person_id=gl.person_id
		LEFT JOIN 0_journal j ON j.type=gl.type AND j.trans_no=gl.type_no"

		." WHERE gl.type= ".db_escape($type)
		." AND gl.type_no = ".db_escape($trans_id)
		." AND gl.amount <> 0"
		." ORDER BY tran_date, counter";

	return db_query($sql, "The gl transactions could not be retrieved");
}

//----------------------------------------------------------------------------------------

function get_bank_trans_for_complete_invoice($type, $trans_no=null, $person_type_id=null, $person_id=null)
{
	$sql = "SELECT bt.*, act.*,
		IFNULL(abs(dt.ov_amount), IFNULL(ABS(st.ov_amount), bt.amount)) settled_amount,
		IFNULL(abs(dt.ov_amount/bt.amount), IFNULL(ABS(st.ov_amount/bt.amount), 1)) settle_rate,
		IFNULL(debtor.curr_code, IFNULL(supplier.curr_code, act.bank_curr_code)) settle_curr

		FROM 0_bank_trans bt
				 LEFT JOIN 0_debtor_trans dt ON dt.type=bt.type AND dt.trans_no=bt.trans_no
				 LEFT JOIN 0_debtors_master debtor ON debtor.debtor_no = dt.debtor_no
				 LEFT JOIN 0_supp_trans st ON st.type=bt.type AND st.trans_no=bt.trans_no
				 LEFT JOIN 0_suppliers supplier ON supplier.supplier_id = st.supplier_id,
			 0_bank_accounts act
		WHERE act.id=bt.bank_act ";
	if ($type != null)
		$sql .= " AND bt.type=".db_escape($type);
	if ($trans_no != null)
		$sql .= " AND bt.trans_no = ".db_escape($trans_no);
	if ($person_type_id != null)
		$sql .= " AND bt.person_type_id = ".db_escape($person_type_id);
	if ($person_id != null)
		$sql .= " AND bt.person_id = ".db_escape($person_id);
	$sql .= " ORDER BY trans_date, bt.id";

	return db_query($sql, "query for bank transaction");
}
//----------------------------------------------------------------------------------

function get_bank_account_currency_for_c_i($id)
{
	$sql= "SELECT bank_curr_code FROM 0_bank_accounts WHERE id=".db_escape($id);
	$result = db_query($sql, "retreive bank account currency");

	$myrow = db_fetch_row($result);
	return $myrow[0];
}
//---------------------------------------------------------------------------------------------

function get_bank_gl_account_for_c_i($id)
{
	$sql = "SELECT account_code FROM 0_bank_accounts WHERE id=".db_escape($id);

	$result = db_query($sql, "could not retreive bank account for $id");

	$bank_account = db_fetch($result);

	return $bank_account['account_code'];
}

//--------------------------------------------------------------------------------------

function get_comments_string_for_C_I($type, $type_no)
{
	$str_return = "";
	$result = get_comments_for_C_I($type, $type_no);
	while ($comment = db_fetch($result))
	{
		if (strlen($str_return))
			$str_return = $str_return . " \n";
		$str_return = $str_return . $comment["memo_"];
	}
	return $str_return;
}

//--------------------------------------------------------------------------------------------------

function get_comments_for_C_I($type, $type_no)
{
	$sql = "SELECT * FROM 0_comments WHERE type="
		.db_escape($type)." AND id=".db_escape($type_no);

	return db_query($sql, "could not query comments transaction table");
}
//--------------------------------------------------------------------------------
function has_attachment_for_C_I($type, $id)
{
	$sql = "SELECT DISTINCT * FROM 0_attachments WHERE type_no=".db_escape($type)." AND trans_no=".db_escape($id);
	$result = db_query($sql, "Could not retrieve attachments");
	$myrow = db_fetch($result);
	if ($myrow === false)
		return 0;
	else
		return $myrow['id'];
}
//--------------------------------------------------------------------------------
function exchange_from_to_for_C_I($amount, $from_curr_code, $to_curr_code, $date_)
{
	$ex_rate = get_exchange_rate_from_to($from_curr_code, $to_curr_code, $date_);
	return $amount / $ex_rate;

}
function update_invoices($type, $trans_no)
{
	$sql = "UPDATE 0_gl_trans SET inactive = 1 
			WHERE type_no = ". db_escape($trans_no)."
			AND type =".db_escape($type);
	db_query($sql, 'Error');
}

function get_supplier_name_voucher($supplier_id)
{
	$sql = "SELECT supp_name AS name FROM ".TB_PREF."suppliers WHERE supplier_id=".db_escape($supplier_id);

	$result = db_query($sql, "could not get supplier");

	$row = db_fetch_row($result);

	return $row[0];
}

?>
