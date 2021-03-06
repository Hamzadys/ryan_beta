<?php

//-------------------------------------------------------------------------------------------------------------
//
//	FIXME - this should be revised for transaction update case.
//
function write_supp_trans($type, $trans_no, $supplier_id, $date_, $due_date, $reference, $supp_reference,
	$amount, $amount_tax, $discount, $err_msg="", $rate=0, $included=0, $supply_disc, $service_disc
	,$fbr_disc, $srb_disc, $write_back, 
	  $supply_tax_id, $service_tax_id, $fbr_tax_id, $srb_tax_id,$cheque,$cheque_date_,$text_1,$text_2,$text_3)
{
	$new = $trans_no==0;
	$date = date2sql($date_);
	$cheque_date = date2sql($cheque_date_);
	if ($due_date == "")
		$due_date = "0000-00-00";
	else
		$due_date = date2sql($due_date);

	if ($new)
		$trans_no = get_next_trans_no($type);

	$curr = get_supplier_currency($supplier_id);
	
	if ($rate == 0)
		$rate = get_exchange_rate_from_home_currency($curr, $date_);


	$sql = " INSERT INTO ".TB_PREF."supp_trans (trans_no, type, supplier_id, tran_date, due_date,
		reference, supp_reference, ov_amount, ov_gst, rate, ov_discount, tax_included,supply_disc, service_disc, fbr_disc, srb_disc, write_back,
        supply_tax_id, service_tax_id, fbr_tax_id, srb_tax_id,cheque,cheque_date,text_1,text_2,text_3) ";
	$sql .= "VALUES (".db_escape($trans_no).", ".db_escape($type)
	.", ".db_escape($supplier_id).", '$date', '$due_date',
		".db_escape($reference).", ".db_escape($supp_reference).", ".db_escape($amount)
		.", ".db_escape($amount_tax).", ".db_escape($rate).", ".db_escape($discount).", ".db_escape($included)."
		, ".db_escape($supply_disc)."
		, ".db_escape($service_disc)."
		, ".db_escape($fbr_disc)."
		, ".db_escape($srb_disc)."
		, ".db_escape($write_back)."
		, ".db_escape($supply_tax_id)."
		, ".db_escape($service_tax_id)."
		, ".db_escape($fbr_tax_id)."
		, ".db_escape($srb_tax_id)."
		, ".db_escape($cheque)."
		, ".db_escape($cheque_date)."
		, ".db_escape($text_1)."
		, ".db_escape($text_2)."
		, ".db_escape($text_3)."
		
		)";

	if ($err_msg == "")
		$err_msg = "Cannot insert a supplier transaction record";

	db_query($sql, $err_msg);
	add_audit_trail($type, $trans_no, $date_);

	return $trans_no;
}

//-------------------------------------------------------------------------------------------------------------

function get_supp_trans($trans_no, $trans_type=-1, $supplier_id=null)
{
	$sql = "SELECT trans.*, (trans.ov_amount+trans.ov_gst+trans.ov_discount) AS Total,
		supplier.supp_name AS supplier_name, supplier.curr_code AS curr_code ";

	if ($trans_type == ST_SUPPAYMENT || $trans_type == ST_BANKPAYMENT)
	{
		// it's a payment so also get the bank account
		$sql .= ", account.bank_name, account.bank_account_name, account.bank_curr_code,
			account.account_type AS BankTransType, bank_trans.amount AS bank_amount,
			bank_trans.ref ";
	}

	$sql .= " FROM ".TB_PREF."supp_trans trans,"
			.TB_PREF."suppliers supplier";

	if ($trans_type == ST_SUPPAYMENT || $trans_type == ST_BANKPAYMENT)
	{
		// it's a payment so also get the bank account
		$sql .= ", "
			.TB_PREF."bank_trans bank_trans,"
			.TB_PREF."bank_accounts account";
	}

	$sql .= " WHERE trans.trans_no=".db_escape($trans_no)."
		AND trans.supplier_id=supplier.supplier_id";

	if (isset($supplier_id))
		$sql .= " AND trans.supplier_id=".db_escape($supplier_id);

	if ($trans_type > -1)
		$sql .= " AND trans.type=".db_escape($trans_type);

	if ($trans_type == ST_SUPPAYMENT || $trans_type == ST_BANKPAYMENT)
	{
		// it's a payment so also get the bank account
		$sql .= " AND bank_trans.trans_no =".db_escape($trans_no)."
			AND bank_trans.type=".db_escape($trans_type)."
			AND account.id=bank_trans.bank_act ";
	}

	$result = db_query($sql, "Cannot retreive a supplier transaction");

    if (db_num_rows($result) == 0)
    {
       // can't return nothing
       display_db_error("no supplier trans found for given params", $sql, true);
       exit;
    }

    if (db_num_rows($result) > 1)
    {
       // can't return multiple
       display_db_error("duplicate supplier transactions found for given params", $sql, true);
       exit;
    }
    return db_fetch($result);
}


//----------------------------------------------------------------------------------------

function exists_supp_trans($type, $type_no)
{
	if ($type == ST_SUPPRECEIVE)
		return exists_grn($type_no);

	$sql = "SELECT trans_no FROM ".TB_PREF."supp_trans WHERE type=".db_escape($type)."
		AND trans_no=".db_escape($type_no);
	$result = db_query($sql, "Cannot retreive a supplier transaction");

    return (db_num_rows($result) > 0);
}

//----------------------------------------------------------------------------------------

function void_supp_trans($type, $type_no)
{
	$sql = "UPDATE ".TB_PREF."supp_trans SET ov_amount=0, ov_discount=0, ov_gst=0,
		alloc=0 WHERE type=".db_escape($type)." AND trans_no=".db_escape($type_no);

	db_query($sql, "could not void supp transactions for type=$type and trans_no=$type_no");
}

//----------------------------------------------------------------------------------------

function clear_supp_trans($type, $type_no)
{
	$sql = "DELETE FROM ".TB_PREF."supp_trans 
			WHERE type=".db_escape($type)." AND trans_no=".db_escape($type_no);

	db_query($sql, "could not clear supp transactions for type=$type and trans_no=$type_no");
}
//----------------------------------------------------------------------------------------

function post_void_supp_trans($type, $type_no)
{
	if ($type == ST_SUPPAYMENT)
	{
		void_supp_payment($type, $type_no);
		return true;
	}

	if ($type == ST_SUPPINVOICE || $type == ST_SUPPCREDIT || $type == ST_SUPPCREDIT_IMPORT)
	{
	 	void_supp_invoice($type, $type_no);
		return true;
	}

	if ($type == ST_SUPPRECEIVE)
	{
	 	return void_grn($type_no);
	}

	return false;
}

//----------------------------------------------------------------------------------------

function get_sql_for_supplier_inquiry($filter, $after_date, $to_date, $supplier_id=ALL_TEXT)
{
    $date_after = date2sql($after_date);
    $date_to = date2sql($to_date);

    $sql = "SELECT trans.type, 
		trans.trans_no,
		trans.reference, 
		supplier.supp_name, 
		trans.supp_reference,
    	trans.tran_date, 
		trans.due_date,
		supplier.curr_code, 
    	(trans.ov_amount + trans.ov_gst  + trans.ov_discount) AS TotalAmount, 
		trans.alloc AS Allocated,
		((trans.type = ".ST_SUPPINVOICE." OR trans.type = ".ST_SUPPCREDIT." OR trans.type = ".ST_SUPPCREDIT_IMPORT." OR trans.type = ".ST_CPV." OR trans.type = ".ST_CRV.") 
		AND trans.due_date < '" . date2sql(Today()) . "') AS OverDue,
    	(ABS(trans.ov_amount + trans.ov_gst  + trans.ov_discount) - trans.alloc <= ".FLOAT_COMP_DELTA.") AS Settled
    	FROM ".TB_PREF."supp_trans as trans, ".TB_PREF."suppliers as supplier
    	WHERE supplier.supplier_id = trans.supplier_id
     	AND trans.tran_date >= '$date_after'
    	AND trans.tran_date <= '$date_to'
		AND trans.ov_amount != 0";	// exclude voided transactions

	$sql2 = "SELECT ".ST_SUPPRECEIVE." as type, 
		trans.id as trans_no,
		trans.reference, 
		supplier.supp_name, 
		po.requisition_no AS supp_reference,
    	delivery_date as tran_date, 
		'' as due_date,
		supplier.curr_code, 
    	'' AS TotalAmount,
		'' AS Allocated,
		0 as OverDue,
    	1 as Settled
    	FROM ".TB_PREF."grn_batch as trans, ".TB_PREF."suppliers as supplier, ".TB_PREF."purch_orders as po
    	WHERE supplier.supplier_id = trans.supplier_id
    	AND trans.purch_order_no = po.order_no
     	AND trans.delivery_date >= '$date_after'
    	AND trans.delivery_date <= '$date_to'";

   	if ($supplier_id != ALL_TEXT) {
   		$sql .= " AND trans.supplier_id = ".db_escape($supplier_id);
   		$sql2 .= " AND trans.supplier_id = ".db_escape($supplier_id);
	}
	if (($filter == '6')) 
	{
			$sql = $sql2;
	} 
	elseif (!isset($filter) || $filter == ALL_TEXT || $filter == '6') {
		$sql = "SELECT * FROM (($sql) UNION ($sql2)) as tr WHERE 1";
   	}

   	if (isset($filter) && $filter != ALL_TEXT)
   	{
   		if (($filter == '1')) 
   		{
   			$sql .= " AND (trans.type = ".ST_SUPPINVOICE." OR trans.type = ".ST_BANKDEPOSIT.")";
   		} 
   		elseif (($filter == '2')) 
   		{
   			$sql .= " AND trans.type = ".ST_SUPPINVOICE." ";
   		} 
   		elseif ($filter == '3') 
   		{
			$sql .= " AND (trans.type = ".ST_SUPPAYMENT." OR trans.type = ".ST_BANKPAYMENT.") ";
   		} 
   		elseif (($filter == '4') || ($filter == '5')) 
   		{
			$sql .= " AND trans.type = ".ST_SUPPCREDIT."  ";
   		}
        elseif (($filter == '7'))
        {
            $sql .= " AND trans.type = ".ST_SUPPCREDIT_IMPORT."  ";
        }
   		if (($filter == '2') || ($filter == '5') )
   		{
   			$today =  date2sql(Today());
			$sql .= " AND trans.due_date < '$today' ";
   		}
   	}
   	return $sql;
}
