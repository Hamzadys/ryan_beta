<?php

//----------------------------------------------------------------------------------------

function get_customer_trans_details($debtor_trans_type, $debtor_trans_no)
{
if (!is_array($debtor_trans_no))
	$debtor_trans_no = array( 0=>$debtor_trans_no );

	$sql = "SELECT line.*,
		line.unit_price+line.unit_tax AS FullUnitPrice,
		line.description As StockDescription,
		item.units, item.mb_flag
		FROM "
			.TB_PREF."debtor_trans_details line,"
			.TB_PREF."stock_master item
		WHERE (";

	$tr=array();
	foreach ($debtor_trans_no as $trans_no)
		$tr[] = 'debtor_trans_no='.db_escape($trans_no);

	$sql .= implode(' OR ', $tr);


	$sql.=	") AND debtor_trans_type=".db_escape($debtor_trans_type)."
		AND item.stock_id=line.stock_id
		ORDER BY id";
	return db_query($sql, "The debtor transaction detail could not be queried");
}

//----------------------------------------------------------------------------------------

function void_customer_trans_details($type, $type_no)
{
	$sql = "UPDATE ".TB_PREF."debtor_trans_details SET quantity=0, unit_price=0,
		unit_tax=0, discount_percent=0, standard_cost=0, src_id=0
		WHERE debtor_trans_no=".db_escape($type_no)."
		AND debtor_trans_type=".db_escape($type);

	db_query($sql, "The debtor transaction details could not be voided");

	// clear the stock move items
	void_stock_move($type, $type_no);
}
//----------------------------------------------------------------------------------------

function write_customer_trans_detail_item($debtor_trans_type, $debtor_trans_no, $stock_id, $description,
	$quantity, $unit_price, $unit_tax, $discount_percent, $std_cost, $src_id, $line_id=0,$units_id,
	  $con_factor, $text1, $text2, $text3, $text4, $text5, $text6,
  $amount1, $amount2, $amount3, $amount4, $amount5, $amount6,
  $date1, $date2, $date3,
  $combo1, $combo2, $combo3,$combo4, $combo5, $combo6)
{
	if ($line_id!=0)
		$sql = "UPDATE ".TB_PREF."debtor_trans_details SET
			stock_id=".db_escape($stock_id).",
			description=".db_escape($description).",
			quantity=$quantity,
			unit_price=$unit_price,
			unit_tax=$unit_tax,
			discount_percent=$discount_percent,
			standard_cost=$std_cost,
			src_id=".db_escape($src_id).",
			units_id=".db_escape($units_id).",
			con_factor=".db_escape($con_factor).",
			text1=".db_escape($text1).",
			text2=".db_escape($text2).",
			text3=".db_escape($text3).",
			text4=".db_escape($text4).",
			text5=".db_escape($text5).",
			text6=".db_escape($text6).",
			amount1=".db_escape($amount1).",
			amount2=".db_escape($amount2).",
			amount3=".db_escape($amount3).",
			amount4=".db_escape($amount4).",
			amount5=".db_escape($amount5).",
			amount6=".db_escape($amount6).",
			date1=".db_escape(date2sql($amount2)).",
			date2=".db_escape(date2sql($amount3)).",
			date3=".db_escape(date2sql($amount4)).",
			combo1=".db_escape($combo1).",
			combo2=".db_escape($combo2).",
			combo3=".db_escape($combo3).",
			combo4=".db_escape($combo4).",
			combo5=".db_escape($combo5).",
			combo6=".db_escape($combo6)."
			WHERE
			id=".db_escape($line_id);
	else
			$sql = "INSERT INTO ".TB_PREF."debtor_trans_details (debtor_trans_no,
				debtor_trans_type, stock_id, description, quantity, unit_price,
				unit_tax, discount_percent, standard_cost, src_id,units_id,con_factor,
				text1, text2, text3, text4,text5, text6,
				amount1, amount2, amount3, amount4,amount5, amount6,
				date1, date2, date3,
				combo1, combo2, combo3,combo4, combo5, combo6)
			VALUES (".db_escape($debtor_trans_no).", ".db_escape($debtor_trans_type).", ".db_escape($stock_id).
			", ".db_escape($description).",
				$quantity, $unit_price, $unit_tax, 
				$discount_percent, $std_cost,".db_escape($src_id).",
				".db_escape($units_id).",
				".db_escape($con_factor).",
				".db_escape($text1).",
				".db_escape($text2).",
				".db_escape($text3).",
				".db_escape($text4).",
				".db_escape($text5).",
				".db_escape($text6).",
				".db_escape($amount1).",
				".db_escape($amount2).",
				".db_escape($amount3).",
				".db_escape($amount4).",
				".db_escape($amount5).",
				".db_escape($amount6).",
				".db_escape(date2sql($date1)).",
				".db_escape(date2sql($date2)).",
				".db_escape(date2sql($date3)).",
				".db_escape($combo1).",
				".db_escape($combo2).",
				".db_escape($combo3).",
				".db_escape($combo4).",
				".db_escape($combo5).",
				".db_escape($combo6).")";

	db_query($sql, "The debtor transaction detail could not be written");
}

