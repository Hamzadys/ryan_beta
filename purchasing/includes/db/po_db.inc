<?php

// ------------------------------------------------------------------------------
//			supp.credit_limit - Sum(IFNULL(IF(trans.type=".ST_SUPPCREDIT.", -1, 1)  //dz 21.8.17
function get_supplier_details_to_order(&$order, $supplier_id)
{
	$sql = "SELECT curr_code, supp_name, tax_group_id, supp.tax_included,
			supp.credit_limit - Sum(IFNULL(IF(trans.type IN(".implode(',', array(ST_SUPPCREDIT, ST_SUPPAYMENT, ST_BANKPAYMENT, ST_CPV))."), 1, 1) 
				* (ov_amount + ov_gst + ov_discount),0)) as cur_credit,
				terms.terms, terms.days_before_due, terms.day_in_following_month
		FROM ".TB_PREF."suppliers supp
			 LEFT JOIN ".TB_PREF."supp_trans trans ON supp.supplier_id = trans.supplier_id
			 LEFT JOIN ".TB_PREF."payment_terms terms ON supp.payment_terms=terms.terms_indicator
		WHERE supp.supplier_id = ".db_escape($supplier_id)."
		GROUP BY
			  supp.supp_name";

	$result = db_query($sql, "The supplier details could not be retreived");
	$myrow = db_fetch($result);

	$order->credit = $myrow["cur_credit"];
	$order->terms = array( 
		'description' => $myrow['terms'],
		'days_before_due' => $myrow['days_before_due'], 
		'day_in_following_month' => $myrow['day_in_following_month'] );

	$_POST['supplier_id'] = $supplier_id;
	$_POST['supplier_name'] = $myrow["supp_name"];
	$_POST['curr_code'] = $myrow["curr_code"];

	$order->set_supplier($supplier_id, $myrow["supp_name"], $myrow["curr_code"], 
		$myrow["tax_group_id"], $myrow["tax_included"]);
}

//----------------------------------------------------------------------------------------
function get_sql_for_po_search2($from, $to, $supplier_id=ALL_TEXT, $location=ALL_TEXT, $order_number='', $stock_id='')
{
	$sql = "SELECT 
		porder.order_no, 
		porder.reference,
		supplier.supp_name, 
		location.location_name,
		porder.requisition_no, 
		porder.ord_date,
		supplier.curr_code,
		Sum(line.unit_price*line.quantity_ordered) AS OrderValue,
		Sum(line.delivery_date < '". date2sql(Today()) ."'
		AND (line.quantity_ordered > line.quantity_received)) As OverDue
		FROM ".TB_PREF."purch_orders as porder,"
			.TB_PREF."purch_order_details as line, "
			.TB_PREF."suppliers as supplier, "
			.TB_PREF."locations as location
		WHERE porder.order_no = line.order_no
		AND porder.supplier_id = supplier.supplier_id
		AND location.loc_code = porder.into_stock_location
		AND (line.quantity_ordered > line.quantity_received) ";

	if ($order_number != "")
	{
		$sql .= "AND porder.reference LIKE ".db_escape('%'. $order_number . '%');
	}
	else
	{
		$data_after = date2sql($from);
		$data_before = date2sql($to);

		$sql .= "  AND porder.ord_date >= '$data_after'";
		$sql .= "  AND porder.ord_date <= '$data_before'";

		if ($location != ALL_TEXT)
		{
			$sql .= " AND porder.into_stock_location = ".db_escape($location);
		}

		if ($stock_id != '')
		{
			$sql .= " AND line.item_code=".db_escape($stock_id);
		}
		if ($supplier_id != ALL_TEXT)
			$sql .= " AND supplier.supplier_id=".db_escape($supplier_id);
	} //end not order number selected

	$sql .= " GROUP BY porder.order_no";
	return $sql;
}
//----------------------------------------------------------------------------------------

function delete_po($po)
{
	global $Refs;

	begin_transaction();
	hook_db_prevoid($po, ST_PURCHORDER);
	$sql = "DELETE FROM ".TB_PREF."purch_orders WHERE order_no=".db_escape($po);
	db_query($sql, "The order header could not be deleted");

	$sql = "DELETE FROM ".TB_PREF."purch_order_details WHERE order_no =".db_escape($po);
	db_query($sql, "The order detail lines could not be deleted");

	$Refs->restore_last(ST_PURCHORDER, $po);
	commit_transaction();
}

//----------------------------------------------------------------------------------------

function add_po(&$po_obj)
{
	global $Refs;

	begin_transaction();
	hook_db_prewrite($po_obj, ST_PURCHORDER);

     /*Insert to purchase order header record */
     $sql = "INSERT INTO ".TB_PREF."purch_orders (supplier_id, Comments, ord_date, reference, 
     	requisition_no, into_stock_location, delivery_address, total, tax_included, prep_amount,payments,
     	pr,delivery_term,sales_tax,h_text1,h_text2,h_text3,h_comb1,h_comb2,h_comb3,Comments1,Comments2,Comments3,Comments4,Comments5) VALUES(";
     $sql .= db_escape($po_obj->supplier_id) . "," .
         db_escape($po_obj->Comments) . ",'" .
         date2sql($po_obj->orig_order_date) . "', " .
		 db_escape($po_obj->reference) . ", " .
         db_escape($po_obj->supp_ref) . ", " .
         db_escape($po_obj->Location) . ", " .
         db_escape($po_obj->delivery_address) . ", " .
         db_escape($po_obj->get_trans_total()). ", " .
         db_escape($po_obj->tax_included). ", " .
		 db_escape($po_obj->prep_amount). ", " .
		 db_escape($po_obj->payments). ",".
		 db_escape($po_obj->pr). ",".
		 db_escape($po_obj->delivery_term). ",".
         db_escape($po_obj->sales_tax). ",".
         db_escape($po_obj->h_text1). ",".
         db_escape($po_obj->h_text2). ",".
         db_escape($po_obj->h_text3). ",".
         db_escape($po_obj->h_comb1). ",".
         db_escape($po_obj->h_comb2). ",".
         db_escape($po_obj->h_comb3). ",".
         db_escape($po_obj->Comments1). ",".
         db_escape($po_obj->Comments2). ",".
         db_escape($po_obj->Comments3). ",".
         db_escape($po_obj->Comments4). ",".
		 db_escape($po_obj->Comments5). ")";

	db_query($sql, "The purchase order header record could not be inserted");

     /*Get the auto increment value of the order number created from the sql above */
     $po_obj->order_no = db_insert_id();

     /*Insert the purchase order detail records */
     foreach ($po_obj->line_items as $line_no => $po_line)
     {
		$sql = "INSERT INTO ".TB_PREF."purch_order_details (order_no, item_code, description, delivery_date,	
		unit_price,	quantity_ordered,text1, text2, text3, text4,text5, text6,
				amount1, amount2, amount3, amount4,amount5, amount6,
				date1, date2, date3,
				combo1, combo2, combo3,combo4, combo5, combo6) VALUES (";
		$sql .= $po_obj->order_no . ", " . db_escape($po_line->stock_id). "," .
		db_escape($po_line->item_description). ",'" .
		date2sql($po_line->req_del_date) . "'," .
		db_escape($po_line->price) . ", " .
		db_escape($po_line->quantity). ", "
		.db_escape($po_line->text1).","
			.db_escape($po_line->text2).","
			.db_escape($po_line->text3).","
			.db_escape($po_line->text4).","
			.db_escape($po_line->text5).","
			.db_escape($po_line->text6).","
			.db_escape($po_line->amount1).","
			.db_escape($po_line->amount2).","
			.db_escape($po_line->amount3).","
			.db_escape($po_line->amount4).","
			.db_escape($po_line->amount5).","
			.db_escape($po_line->amount6).","
			.db_escape(date2sql($po_line->date1)).","
			.db_escape(date2sql($po_line->date2)).","
			.db_escape(date2sql($po_line->date3)).","
			.db_escape($po_line->combo1).","
			.db_escape($po_line->combo2).","
			.db_escape($po_line->combo3).","
			.db_escape($po_line->combo4).","
			.db_escape($po_line->combo5).","
			.db_escape($po_line->combo6).")";
		db_query($sql, "One of the purchase order detail records could not be inserted");
		$po_obj->line_items[$line_no]->po_detail_rec = db_insert_id();
     }

	$Refs->save(ST_PURCHORDER, $po_obj->order_no, $po_obj->reference);

	add_audit_trail(ST_PURCHORDER, $po_obj->order_no, $po_obj->orig_order_date);
	hook_db_postwrite($po_obj, ST_PURCHORDER);
	commit_transaction();

	return $po_obj->order_no;
}
//----------------------------------------------------------------------------------------
function get_sql_for_pr_search_completed()
{
	global $order_number, $selected_stock_item , $supplier_id;

	$sql = "SELECT 
		prorder.requisition_id, 
		line.supplier_id, 
		supplier.supp_name, 
		prorder.application_date, 
		supplier.curr_code, 
		Sum(line.price*line.quantity) AS OrderValue
		FROM ".TB_PREF."requisitions as prorder, "
			.TB_PREF."requisition_details as line, "
			.TB_PREF."suppliers as supplier
		WHERE prorder.requisition_id = line.requisition_id
		AND line.supplier_id = supplier.supplier_id";

//	if (isset($_GET['supplier_id']))
//		$sql .= "AND supplier.supplier_id=".@$_GET['supplier_id']." ";
//	if (isset($order_number) && $order_number != "")
//	{
//		$sql .= "AND line.reference LIKE ".db_escape('%'. $order_number . '%');
//	}
//	else
//	{
//
//		$data_after = date2sql($_POST['OrdersAfterDate']);
//		$date_before = date2sql($_POST['OrdersToDate']);
//
//		$sql .= " AND porder.ord_date >= '$data_after'";
//		$sql .= " AND porder.ord_date <= '$date_before'";
//
//		if (isset($_POST['StockLocation']) && $_POST['StockLocation'] != ALL_TEXT)
//		{
//			$sql .= " AND porder.into_stock_location = ".db_escape($_POST['StockLocation']);
//		}
		if (isset($selected_stock_item))
		{
			$sql .= " AND line.item_code=".db_escape($selected_stock_item);
		}
//
		if ($supplier_id != null)
		{
			$sql .= " AND line.supplier_id=".db_escape($supplier_id);
		}
//
//	} //end not order number selected

	$sql .= " GROUP BY line.requisition_id";
	return $sql;
}
//----------------------------------------------------------------------------------------

function update_po(&$po_obj)
{
	begin_transaction();
	hook_db_prewrite($po_obj, ST_PURCHORDER);

    /*Update the purchase order header with any changes */
    $sql = "UPDATE ".TB_PREF."purch_orders SET Comments=" . db_escape($po_obj->Comments) . ",
		requisition_no= ". db_escape( $po_obj->supp_ref). ",
		into_stock_location=" . db_escape($po_obj->Location). ",
		ord_date='" . date2sql($po_obj->orig_order_date) . "',
		delivery_address=" . db_escape($po_obj->delivery_address).",
		total=". db_escape($po_obj->get_trans_total()).",
		prep_amount=". db_escape($po_obj->prep_amount).",
tax_included=". db_escape($po_obj->tax_included).",
payments=". db_escape($po_obj->payments).",
pr=". db_escape($po_obj->pr).",
delivery_term=". db_escape($po_obj->delivery_term).",
sales_tax=". db_escape($po_obj->sales_tax).",
h_text1=". db_escape($po_obj->h_text1).",
h_text2=". db_escape($po_obj->h_text2).",
h_text3=". db_escape($po_obj->h_text3).",
h_comb1=". db_escape($po_obj->h_comb1).",
h_comb2=". db_escape($po_obj->h_comb2).",
h_comb3=". db_escape($po_obj->h_comb3).",
Comments1=". db_escape($po_obj->Comments1).",
Comments2=". db_escape($po_obj->Comments2).",
Comments3=". db_escape($po_obj->Comments3).",
Comments4=". db_escape($po_obj->Comments4).",
Comments5=". db_escape($po_obj->Comments5);

    $sql .= " WHERE order_no = " . $po_obj->order_no;
	db_query($sql, "The purchase order could not be updated");

	$sql = "DELETE FROM ".TB_PREF."purch_order_details WHERE order_no="
		.db_escape($po_obj->order_no);
	db_query($sql, "could not delete old purch order details");

    /*Now Update the purchase order detail records */
    foreach ($po_obj->line_items as $po_line)
    {
    	$sql = "INSERT INTO ".TB_PREF."purch_order_details (po_detail_item, order_no, item_code, 
    		description, delivery_date, unit_price, quantity_ordered, quantity_received,
    		text1, text2, text3, text4,text5, text6,
				amount1, amount2, amount3, amount4,amount5, amount6,
				date1, date2, date3,
				combo1, combo2, combo3,combo4, combo5, combo6) VALUES ("
			.db_escape($po_line->po_detail_rec ? $po_line->po_detail_rec : 0). ","
			.$po_obj->order_no . ","
			.db_escape($po_line->stock_id). ","
			.db_escape($po_line->item_description). ",'"
			.date2sql($po_line->req_del_date) . "',"
			.db_escape($po_line->price) . ", "
			.db_escape($po_line->quantity) . ", "
			.db_escape($po_line->qty_received) . ", "
			.db_escape($po_line->text1).","
			.db_escape($po_line->text2).","
			.db_escape($po_line->text3).","
			.db_escape($po_line->text4).","
			.db_escape($po_line->text5).","
			.db_escape($po_line->text6).","
			.db_escape($po_line->amount1).","
			.db_escape($po_line->amount2).","
			.db_escape($po_line->amount3).","
			.db_escape($po_line->amount4).","
			.db_escape($po_line->amount5).","
			.db_escape($po_line->amount6).","
			.db_escape(date2sql($po_line->date1)).","
			.db_escape(date2sql($po_line->date2)).","
			.db_escape(date2sql($po_line->date3)).","
			.db_escape($po_line->combo1).","
			.db_escape($po_line->combo2).","
			.db_escape($po_line->combo3).","
			.db_escape($po_line->combo4).","
			.db_escape($po_line->combo5).","
			.db_escape($po_line->combo6).")";
		db_query($sql, "One of the purchase order detail records could not be updated");
    }

	reallocate_payments($po_obj->order_no, ST_PURCHORDER, $po_obj->orig_order_date, $po_obj->get_trans_total(),
        $po_obj->prepayments, $po_obj->supplier_id); //ansar 26-08-17

	add_audit_trail($po_obj->trans_type, $po_obj->order_no, Today(), _("Updated."));
	hook_db_postwrite($po_obj, ST_PURCHORDER);
	commit_transaction();

	return $po_obj->order_no;
}

//----------------------------------------------------------------------------------------

function read_po_header($order_no, &$order)
{
    	$sql = "SELECT po.*, supplier.*, loc.location_name 
		FROM ".TB_PREF."purch_orders po,"
			.TB_PREF."suppliers supplier,"
			.TB_PREF."locations loc
		WHERE po.supplier_id = supplier.supplier_id
		AND loc.loc_code = into_stock_location
		AND po.order_no = ".db_escape($order_no);

   	$result = db_query($sql, "The order cannot be retrieved");

	if (db_num_rows($result) == 1)
	{

      	$myrow = db_fetch($result);

      	$order->trans_type = ST_PURCHORDER;
      	$order->order_no = $order_no;

      	$order->set_supplier($myrow["supplier_id"], $myrow["supp_name"], $myrow["curr_code"],
      		$myrow['tax_group_id'], $myrow["tax_included"]);

		$order->credit = get_current_supp_credit($order->supplier_id);

      	$order->orig_order_date = sql2date($myrow["ord_date"]);
      	$order->Comments = nl2br($myrow["comments"]);
        $order->Comments1 = nl2br($myrow["Comments1"]);
        $order->Comments2 = nl2br($myrow["Comments2"]);
      	$order->Location = $myrow["into_stock_location"];
      	$order->supp_ref = $myrow["requisition_no"];
      	$order->reference = $myrow["reference"];

$order->pr= $myrow["pr"];
$order->delivery_term= $myrow["delivery_term"];
$order->sales_tax= $myrow["sales_tax"];
//
        $order->h_text1= $myrow["h_text1"];
        $order->h_text2= $myrow["h_text2"];
        $order->h_text3= $myrow["h_text3"];
        $order->h_comb1= $myrow["h_comb1"];
        $order->h_comb2= $myrow["h_comb2"];
        $order->h_comb3= $myrow["h_comb3"];

      	$order->delivery_address = $myrow["delivery_address"];
      	$order->alloc = $myrow["alloc"];
      	$order->prep_amount = $myrow["prep_amount"];
        $order->prepayments = get_payments_for($order_no, ST_PURCHORDER, $myrow["supplier_id"]);//ansar 26-08-17

      	return true;
	}

	display_db_error("FATAL : duplicate purchase order found", "", true);
	return false;
}

//----------------------------------------------------------------------------------------

function read_po_items($order_no, &$order, $open_items_only=false)
{
	/*now populate the line po array with the purchase order details records */

	$sql = "SELECT poline.*, units
		FROM ".TB_PREF."purch_order_details poline
			LEFT JOIN ".TB_PREF."stock_master item	ON poline.item_code=item.stock_id
		WHERE order_no =".db_escape($order_no);

    if ($open_items_only)
 		$sql .= " AND (poline.quantity_ordered > poline.quantity_received) ";

	$sql .= " ORDER BY po_detail_item";

	$result = db_query($sql, "The lines on the purchase order cannot be retrieved");

    if (db_num_rows($result) > 0)
    {
		while ($myrow = db_fetch($result))
        {
        	$data = get_purchase_data($order->supplier_id, $myrow['item_code']);
        	if ($data !== false)
        	{
        		if ($data['supplier_description'] != "")
        			$myrow['description'] = $data['supplier_description'];
        	}		
            if (is_null($myrow["units"]))
            {
		       	$units = "";
            }
            else
            {
                $units = $myrow["units"];
            }

            if ($order->add_to_order($order->lines_on_order, $myrow["item_code"],
            	$myrow["quantity_ordered"],$myrow["description"],
            	$myrow["unit_price"],$units, sql2date($myrow["delivery_date"]),
            	$myrow["qty_invoiced"], $myrow["quantity_received"], $myrow['text1'], $myrow['text2'], $myrow['text3'], $myrow['text4'], $myrow['text5'], $myrow['text6'],
				$myrow['amount1'], $myrow['amount2'], $myrow['amount3'], $myrow['amount4'], $myrow['amount5'], $myrow['amount6'],
				sql2date($myrow['date1']), sql2date($myrow['date2']), sql2date($myrow['date3']),
				$myrow['combo1'], $myrow['combo2'], $myrow['combo3'], $myrow['combo4'], $myrow['combo5'], $myrow['combo6'])) {
				$order->line_items[$order->lines_on_order-1]->po_detail_rec = $myrow["po_detail_item"];
			}
        } /* line po from purchase order details */
    } //end of checks on returned data set
}

//----------------------------------------------------------------------------------------

function read_po($order_no, &$order, $open_items_only=false)
{
	$result = read_po_header($order_no, $order);

	if ($result)
		read_po_items($order_no, $order, $open_items_only);
}

//----------------------------------------------------------------------------------------

function get_po_items($order_no)
{
	$sql = "SELECT item_code, quantity_ordered, quantity_received, qty_invoiced
		FROM ".TB_PREF."purch_order_details
		WHERE order_no=".db_escape($order_no)
		." ORDER BY po_detail_item";

	$result = db_query($sql, "could not query purch order details");
    check_db_error("Could not check that the details of the purchase order had not been changed by another user ", $sql);
    return $result;
}
//----------------------------------------------------------------------------------------

function get_short_info($stock_id)
{
	$sql = "SELECT description, units, mb_flag
		FROM ".TB_PREF."stock_master WHERE stock_id = ".db_escape($stock_id);

	return db_query($sql,"The stock details for " . $stock_id . " could not be retrieved");
}

function get_sql_for_po_search_completed($from, $to, $supplier_id=ALL_TEXT, $location=ALL_TEXT,
	$order_number = '', $stock_id = '', $also_closed=false)
{
	$sql = "SELECT 
		porder.order_no, 
		porder.reference, 
		supplier.supp_name, 
		location.location_name,
		porder.requisition_no, 
		porder.ord_date, 
		supplier.curr_code, 
		Sum(line.unit_price*line.quantity_ordered) AS OrderValue,
		porder.into_stock_location,
		chk.isopen
		FROM ".TB_PREF."purch_orders as porder
				LEFT JOIN (
					SELECT order_no, SUM(quantity_ordered-quantity_received + quantity_ordered-qty_invoiced) isopen
					FROM ".TB_PREF."purch_order_details
					GROUP BY order_no
				) chk ON chk.order_no=porder.order_no,"
			.TB_PREF."purch_order_details as line, "
			.TB_PREF."suppliers as supplier, "
			.TB_PREF."locations as location
		WHERE porder.order_no = line.order_no
		AND porder.supplier_id = supplier.supplier_id
		AND location.loc_code = porder.into_stock_location ";

	if ($supplier_id != ALL_TEXT)
		$sql .= "AND supplier.supplier_id=".$supplier_id." ";
	if ($order_number != "")
	{
		$sql .= "AND porder.reference LIKE ".db_escape('%'. $order_number . '%');
	}
	else
	{

		$data_after = date2sql($from);
		$date_before = date2sql($to);

		$sql .= " AND porder.ord_date >= '$data_after'";
		$sql .= " AND porder.ord_date <= '$date_before'";

		if ($location != ALL_TEXT)
		{
			$sql .= " AND porder.into_stock_location = ".db_escape($location);
		}
		if ($stock_id !== '')
		{
			$sql .= " AND line.item_code=".db_escape($stock_id);
		}
		if ($supplier_id != ALL_TEXT)
			$sql .= " AND supplier.supplier_id=".db_escape($supplier_id);

	}

	if (!$also_closed)
		$sql .= " AND isopen";
	$sql .= " GROUP BY porder.order_no";
	return $sql;
}

function get_sql_for_po_search($from, $to, $supplier_id=ALL_TEXT, $location=ALL_TEXT, $order_number='', $stock_id='')
{
	$sql = "SELECT 
		porder.order_no, 
		porder.reference,
		supplier.supp_name, 
		location.location_name,
		porder.requisition_no, 
		porder.ord_date,
		supplier.curr_code,
		Sum(line.unit_price*line.quantity_ordered) AS OrderValue,
		Sum(line.delivery_date < '". date2sql(Today()) ."'
		AND (line.quantity_ordered > line.quantity_received)) As OverDue
		FROM ".TB_PREF."purch_orders as porder,"
			.TB_PREF."purch_order_details as line, "
			.TB_PREF."suppliers as supplier, "
			.TB_PREF."locations as location
		WHERE porder.order_no = line.order_no
		AND porder.supplier_id = supplier.supplier_id
		AND location.loc_code = porder.into_stock_location
		AND (line.quantity_ordered > line.quantity_received) ";

	if ($order_number != "")
	{
		$sql .= "AND porder.reference LIKE ".db_escape('%'. $order_number . '%');
	}
	else
	{
		$data_after = date2sql($from);
		$data_before = date2sql($to);

		$sql .= "  AND porder.ord_date >= '$data_after'";
		$sql .= "  AND porder.ord_date <= '$data_before'";

		if ($location != ALL_TEXT)
		{
			$sql .= " AND porder.into_stock_location = ".db_escape($location);
		}

		if ($stock_id != '')
		{
			$sql .= " AND line.item_code=".db_escape($stock_id);
		}
		if ($supplier_id != ALL_TEXT)
			$sql .= " AND supplier.supplier_id=".db_escape($supplier_id);
	} //end not order number selected

	$sql .= " GROUP BY porder.order_no";
	return $sql;
}
//----------------------------------------------------------------------------------------
function get_pr_history($selected_id)
{
    $sql = "SELECT prorder.* ,
				line.supplier_id,
				supplier.supp_name,
				(line.price*line.order_quantity) AS OrderValue
 			FROM ".TB_PREF."requisitions as prorder, "
        .TB_PREF."requisition_details as line, "
        .TB_PREF."suppliers as supplier
			WHERE line.requisition_id = prorder.requisition_id
			AND prorder.requisition_id = ".db_escape($selected_id)."
			AND line.supplier_id = supplier.supplier_id";

    return db_query($sql,"could not get history");

}
