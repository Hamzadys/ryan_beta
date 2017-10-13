<?php

include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/includes/ui/items_cart.inc");

//--------------------------------------------------------------------------------

function add_to_order(&$order, $new_item, $new_item_qty, $standard_cost,$discount, $src_no, $src_id,$units_id,
					  $text1, $text2, $text3, $text4, $text5, $text6,
					  $amount1, $amount2, $amount3, $amount4,$amount5, $amount6,
					  $date1, $date2, $date3,
					  $combo1, $combo2, $combo3,$combo4, $combo5, $comb6)
{
    if ($order->find_cart_item($new_item))
         display_error(_("For Part :") . $new_item . " " . "This item is already on this document. You can change the quantity on the existing line if necessary.");
	else
         $order->add_to_cart (count($order->line_items), $new_item, $new_item_qty, $standard_cost,$discount, $src_no, $src_id,$units_id,
			 $text1, $text2, $text3, $text4, $text5, $text6,
			 $amount1, $amount2, $amount3, $amount4,$amount5, $amount6,
			 $date1, $date2, $date3,
			 $combo1, $combo2, $combo3,$combo4, $combo5, $comb6);
}

//--------------------------------------------------------------------------------

function display_order_header(&$order)
{
	global $Refs;

	start_outer_table(TABLESTYLE, "width='70%'");

	table_section(1);
	
	locations_list_row(_("From Location:"), 'FromStockLocation', null, false, false, $order->fixed_asset);
	locations_list_row(_("To Location:"), 'ToStockLocation', null,false, false, $order->fixed_asset);

	table_section(2, "50%");

    date_row(_("Date:"), 'AdjDate', '', true);

	ref_row(_("Reference:"), 'ref', '', $Refs->get_next(ST_LOCTRANSFER, null, array('date'=>get_post('AdjDate'), 'location'=> get_post('FromStockLocation'))),
		 false, ST_LOCTRANSFER);

	end_outer_table(1); // outer table
}

//---------------------------------------------------------------------------------

function display_transfer_items($title, &$order)
{
	global $path_to_root;

	display_heading($title);
    div_start('items_table');
	start_table(TABLESTYLE, "width='80%'");
	$myrow_1 = get_company_item_pref_from_position(1);
	$myrow_2 = get_company_item_pref_from_position(2);
	$myrow_3 = get_company_item_pref_from_position(3);
	$myrow_4 = get_company_item_pref_from_position(4);
	$myrow_5 = get_company_item_pref_from_position(5);
	$myrow_6 = get_company_item_pref_from_position(6);
	$myrow_7 = get_company_item_pref_from_position(7);
	$myrow_8 = get_company_item_pref_from_position(8);
	$myrow_9 = get_company_item_pref_from_position(9);
	$myrow_10 = get_company_item_pref_from_position(10);
	$myrow_11 = get_company_item_pref_from_position(11);
	$myrow_12 = get_company_item_pref_from_position(12);
	$myrow_13 = get_company_item_pref_from_position(13);
	$myrow_14 = get_company_item_pref_from_position(14);
	$myrow_15 = get_company_item_pref_from_position(15);
	$myrow_16 = get_company_item_pref_from_position(16);
	$myrow_17 = get_company_item_pref_from_position(17);
	$myrow_18 = get_company_item_pref_from_position(18);
	$myrow_19 = get_company_item_pref_from_position(19);
	$myrow_20 = get_company_item_pref_from_position(20);
	$myrow_21 = get_company_item_pref_from_position(21);
	$th = array(_("Item Code"), _("Item Description"));

	//Text Boxes Headings

	if($myrow_1['item_enable']) {
		array_append($th, array($myrow_1['label_value']._("")) );
	}
	if($myrow_2['item_enable']) {
		array_append($th, array($myrow_2['label_value']._("")) );
	}
	if($myrow_3['item_enable']) {
		array_append($th, array($myrow_3['label_value']._("")) );
	}
	if($myrow_4['item_enable']) {
		array_append($th, array($myrow_4['label_value']._("")) );
	}
	if($myrow_5['item_enable']) {
		array_append($th, array($myrow_5['label_value']._("")) );
	}
	if($myrow_6['item_enable']) {
		array_append($th, array($myrow_6['label_value']._("")) );
	}
	if($myrow_7['item_enable']) {
		array_append($th, array($myrow_7['label_value']._("")) );
	}
	if($myrow_8['item_enable']) {
		array_append($th, array($myrow_8['label_value']._("")) );
	}
	if($myrow_9['item_enable']) {
		array_append($th, array($myrow_9['label_value']._("")) );
	}
	if($myrow_10['item_enable']) {
		array_append($th, array($myrow_10['label_value']._("")) );
	}
	if($myrow_11['item_enable']) {
		array_append($th, array($myrow_11['label_value']._("")) );
	}
	if($myrow_12['item_enable']) {
		array_append($th, array($myrow_12['label_value']._("")) );
	}
	if($myrow_13['item_enable']) {
		array_append($th, array($myrow_13['label_value']._("")) );
	}
	if($myrow_14['item_enable']) {
		array_append($th, array($myrow_14['label_value']._("")) );
	}
	if($myrow_15['item_enable']) {
		array_append($th, array($myrow_15['label_value']._("")) );
	}
	if($myrow_16['item_enable']) {
		array_append($th, array($myrow_16['label_value']._("")) );
	}
	if($myrow_17['item_enable']) {
		array_append($th, array($myrow_17['label_value']._("")) );
	}
	if($myrow_18['item_enable']) {
		array_append($th, array($myrow_18['label_value']._("")) );
	}
	if($myrow_19['item_enable']) {
		array_append($th, array($myrow_19['label_value']._("")) );
	}
	if($myrow_20['item_enable']) {
		array_append($th, array($myrow_20['label_value']._("")) );
	}
	if($myrow_21['item_enable']) {
		array_append($th, array($myrow_21['label_value']._("")) );
	}

	{
		array_append($th, array(_, _("Quantity"), _("Unit"), ''));
	}
	if (!$order->fixed_asset && count($order->line_items)) $th[] = '';
	table_header($th);
	$k = 0;  //row colour counter

	$low_stock = $order->check_qoh($_POST['FromStockLocation'], $_POST['AdjDate'], true);
	$id = find_submit('Edit');
	foreach ($order->line_items as $line_no=>$stock_item)
	{

		if ($id != $line_no)
		{
			if (in_array($stock_item->stock_id, $low_stock))
				start_row("class='stockmankobg'");	// notice low stock status
			else 
				alt_table_row_color($k);

			if ($order->fixed_asset)
				label_cell($stock_item->stock_id);
			else
				view_stock_status_cell($stock_item->stock_id);
    		label_cell($stock_item->item_description);
			//text boxes labels
			if($myrow_1['item_enable'])
			{
				label_cell($stock_item->$myrow_1['name']);
			}
			if($myrow_2['item_enable'])
			{
				label_cell($stock_item->$myrow_2['name']);
			}
			if($myrow_3['item_enable'])
			{
				label_cell($stock_item->$myrow_3['name']);
			}
			if($myrow_4['item_enable'])
			{
				label_cell($stock_item->$myrow_4['name']);
			}
			if($myrow_5['item_enable'])
			{
				label_cell($stock_item->$myrow_5['name']);
			}
			if($myrow_6['item_enable'])
			{
				label_cell($stock_item->$myrow_6['name']);
			}
			if($myrow_7['item_enable'])
			{
				label_cell($stock_item->$myrow_7['name']);
			}
			if($myrow_8['item_enable'])
			{
				label_cell($stock_item->$myrow_8['name']);
			}
			if($myrow_9['item_enable'])
			{
				label_cell($stock_item->$myrow_9['name']);
			}
			if($myrow_10['item_enable'])
			{
				label_cell($stock_item->$myrow_10['name']);
			}
			if($myrow_11['item_enable'])
			{
				label_cell($stock_item->$myrow_11['name']);
			}
			if($myrow_12['item_enable'])
			{
				label_cell($stock_item->$myrow_12['name']);
			}

			///combo inputs
			if($myrow_13['item_enable'])
			{
				label_cell($stock_item->$myrow_13['name']);
			}
			if($myrow_14['item_enable'])
			{
				label_cell($stock_item->$myrow_14['name']);
			}
			if($myrow_15['item_enable'])
			{
				label_cell($stock_item->$myrow_15['name']);
			}
			if($myrow_16['item_enable'])
			{
				label_cell($stock_item->$myrow_16['name']);
			}
			if($myrow_17['item_enable'])
			{
				label_cell($stock_item->$myrow_17['name']);
			}
			if($myrow_18['item_enable'])
			{
				label_cell($stock_item->$myrow_18['name']);
			}
			if($myrow_19['item_enable'])
			{
				label_cell($stock_item->$myrow_19['name']);
			}
			if($myrow_20['item_enable'])
			{
				label_cell($stock_item->$myrow_20['name']);
			}
			if($myrow_21['item_enable'])
			{
				label_cell($stock_item->$myrow_21['name']);
			}

			qty_cell($stock_item->quantity, false, get_qty_dec($stock_item->stock_id));
    		label_cell($stock_item->units);

	     	if (!$order->fixed_asset)
				edit_button_cell("Edit$line_no", _("Edit"),	_('Edit document line'));
			delete_button_cell("Delete$line_no", _("Delete"), _('Remove line from document'));
    		end_row();
		}
		else
		{
			transfer_edit_item_controls($order, $line_no);
		}
	}

	if ($id == -1)
		transfer_edit_item_controls($order);

    end_table();
	if ($low_stock)
		display_note(_("Marked items have insufficient quantities in stock as on day of transfer."), 0, 1, "class='stockmankofg'");
	div_end();
}

//---------------------------------------------------------------------------------

function transfer_edit_item_controls(&$order, $line_no=-1)
{
	global $Ajax;
	start_row();
	$myrow_1 = get_company_item_pref_from_position(1);
	$myrow_2 = get_company_item_pref_from_position(2);
	$myrow_3 = get_company_item_pref_from_position(3);
	$myrow_4 = get_company_item_pref_from_position(4);
	$myrow_5 = get_company_item_pref_from_position(5);
	$myrow_6 = get_company_item_pref_from_position(6);
	$myrow_7 = get_company_item_pref_from_position(7);
	$myrow_8 = get_company_item_pref_from_position(8);
	$myrow_9 = get_company_item_pref_from_position(9);
	$myrow_10 = get_company_item_pref_from_position(10);
	$myrow_11 = get_company_item_pref_from_position(11);
	$myrow_12 = get_company_item_pref_from_position(12);
	$myrow_13 = get_company_item_pref_from_position(13);
	$myrow_14 = get_company_item_pref_from_position(14);
	$myrow_15 = get_company_item_pref_from_position(15);
	$myrow_16 = get_company_item_pref_from_position(16);
	$myrow_17 = get_company_item_pref_from_position(17);
	$myrow_18 = get_company_item_pref_from_position(18);
	$myrow_19 = get_company_item_pref_from_position(19);
	$myrow_20 = get_company_item_pref_from_position(20);
	$myrow_21 = get_company_item_pref_from_position(21);
	$id = find_submit('Edit');
	if ($line_no != -1 && $line_no == $id)
	{
		$_POST['stock_id'] = $order->line_items[$id]->stock_id;
		$_POST['qty'] = qty_format($order->line_items[$id]->quantity, $order->line_items[$id]->stock_id, $dec);
		$_POST['units'] = $order->line_items[$id]->units;
		$_POST['text1'] = $order->line_items[$id]->text1;
		$_POST['text2'] = $order->line_items[$id]->text2;
		$_POST['text3'] = $order->line_items[$id]->text3;
		$_POST['text4'] = $order->line_items[$id]->text4;
		$_POST['text5'] = $order->line_items[$id]->text5;
		$_POST['text6'] = $order->line_items[$id]->text6;
		$_POST['amount1'] = price_format($order->line_items[$id]->amount1);
		$_POST['amount2'] = price_format($order->line_items[$id]->amount2);
		$_POST['amount3'] = price_format($order->line_items[$id]->amount3);
		$_POST['amount4'] = price_format($order->line_items[$id]->amount4);
		$_POST['amount5'] = price_format($order->line_items[$id]->amount5);
		$_POST['amount6'] = price_format($order->line_items[$id]->amount6);
		$_POST['date1'] = ($order->line_items[$id]->date1);
		$_POST['date2'] = ($order->line_items[$id]->date2);
		$_POST['date3'] = ($order->line_items[$id]->date3);
		$_POST['combo1'] = $order->line_items[$id]->combo1;
		$_POST['combo2'] = $order->line_items[$id]->combo2;
		$_POST['combo3'] = $order->line_items[$id]->combo3;
		$_POST['combo4'] = $order->line_items[$id]->combo4;
		$_POST['combo5'] = $order->line_items[$id]->combo5;
		$_POST['combo6'] = $order->line_items[$id]->combo6;

		hidden('stock_id', $_POST['stock_id']);
		label_cell($_POST['stock_id']);
		label_cell($order->line_items[$id]->item_description);
	    $Ajax->activate('items_table');
	}
	else
	{
		if ($order->fixed_asset)
			stock_disposable_fa_list_cells(null, 'stock_id', null, false, true, $order->line_items);
		else
			stock_costable_items_list_cells(null, 'stock_id', null, false, true);

		if (list_updated('stock_id')) {
			    $Ajax->activate('units');
			    $Ajax->activate('qty');
		}

    	$item_info = get_item_edit_info($_POST['stock_id']);

		$dec = $item_info['decimals'];
   		$_POST['qty'] = number_format2(0, $dec);
		$_POST['units'] = $item_info["units"];
	}
	if($myrow_1['item_enable'])
	{
		if($myrow_1['type'] == 1)
			small_amount_cells(null, $myrow_1['name']);
		elseif($myrow_1['type'] == 2)
			combo1_list_cells(null, $myrow_1['name'],$item_info['$myrow_1["name"]']);
		elseif($myrow_1['type'] == 3)
			date_cells(null, $myrow_1['name'],null);
		elseif($myrow_1['type'] == 4)
			text_cells(null, $myrow_1['name'], $item_info['$myrow_1["name"]'], $myrow_1["s_width"], 40);
	}
	if($myrow_2['item_enable'])
	{
		if($myrow_2['type'] == 1)
			small_amount_cells(null, $myrow_2['name']);
		elseif($myrow_1['type'] == 2)
			combo1_list_cells(null, $myrow_2['name'],$item_info['$myrow_2["name"]']);
		elseif($myrow_2['type'] == 3)
			date_cells(null, $myrow_2['name'],null);
		elseif($myrow_2['type'] == 4)
			text_cells(null, $myrow_2['name'], $item_info['$myrow_2["name"]'], $myrow_2["s_width"], 40);
	}
	if($myrow_3['item_enable'])
	{
		if($myrow_3['type'] == 1)
			small_amount_cells(null, $myrow_3['name']);
		elseif($myrow_3['type'] == 2)
			combo1_list_cells(null, $myrow_3['name'],$item_info['$myrow_3["name"]']);
		elseif($myrow_3['type'] == 3)
			date_cells(null, $myrow_3['name'],null);
		elseif($myrow_3['type'] == 4)
			text_cells(null, $myrow_3['name'], $item_info['$myrow_3["name"]'], $myrow_3["s_width"], 40);
	}
	if($myrow_4['item_enable'])
	{
		if($myrow_4['type'] == 1)
			small_amount_cells(null, $myrow_4['name']);
		elseif($myrow_4['type'] == 2)
			combo1_list_cells(null, $myrow_4['name'],$item_info['$myrow_4["name"]']);
		elseif($myrow_4['type'] == 3)
			date_cells(null, $myrow_4['name'],null);
		elseif($myrow_4['type'] == 4)
			text_cells(null, $myrow_4['name'], $item_info['$myrow_4["name"]'], $myrow_4["s_width"], 40);
	}
	if($myrow_5['item_enable'])
	{
		if($myrow_5['type'] == 1)
			small_amount_cells(null, $myrow_5['name']);
		elseif($myrow_5['type'] == 2)
			combo1_list_cells(null, $myrow_5['name'],$item_info['$myrow_5["name"]']);
		elseif($myrow_5['type'] == 3)
			date_cells(null, $myrow_5['name'],null);
		elseif($myrow_5['type'] == 4)
			text_cells(null, $myrow_5['name'], $item_info['$myrow_5["name"]'], $myrow_5["s_width"], 40);
	}
	if($myrow_6['item_enable'])
	{
		if($myrow_6['type'] == 1)
			small_amount_cells(null, $myrow_6['name']);
		elseif($myrow_6['type'] == 2)
			combo1_list_cells(null, $myrow_6['name'],$item_info['$myrow_6["name"]']);
		elseif($myrow_6['type'] == 3)
			date_cells(null, $myrow_6['name'],null);
		elseif($myrow_6['type'] == 4)
			text_cells(null, $myrow_6['name'], $item_info['$myrow_6["name"]'], $myrow_6["s_width"], 40);
	}
	if($myrow_7['item_enable'])
	{
		if($myrow_7['type'] == 1)
			small_amount_cells(null, $myrow_7['name']);
		elseif($myrow_7['type'] == 2)
			combo1_list_cells(null, $myrow_7['name'],$item_info['$myrow_7["name"]']);
		elseif($myrow_7['type'] == 3)
			date_cells(null, $myrow_7['name'],null);
		elseif($myrow_7['type'] == 4)
			text_cells(null, $myrow_7['name'], $item_info['$myrow_7["name"]'], $myrow_7["s_width"], 40);
	}
	if($myrow_8['item_enable'])
	{
		if($myrow_8['type'] == 1)
			small_amount_cells(null, $myrow_8['name']);
		elseif($myrow_8['type'] == 2)
			combo1_list_cells(null, $myrow_8['name'],$item_info['$myrow_8["name"]']);
		elseif($myrow_8['type'] == 3)
			date_cells(null, $myrow_8['name'],null);
		elseif($myrow_8['type'] == 4)
			text_cells(null, $myrow_8['name'], $item_info['$myrow_8["name"]'], $myrow_8["s_width"], 40);
	}
	if($myrow_9['item_enable'])
	{
		if($myrow_9['type'] == 1)
			small_amount_cells(null, $myrow_9['name']);
		elseif($myrow_9['type'] == 2)
			combo1_list_cells(null, $myrow_9['name'],$item_info['$myrow_9["name"]']);
		elseif($myrow_9['type'] == 3)
			date_cells(null, $myrow_9['name'],null);
		elseif($myrow_9['type'] == 4)
			text_cells(null, $myrow_9['name'], $item_info['$myrow_9["name"]'], $myrow_9["s_width"], 40);
	}
	if($myrow_10['item_enable'])
	{
		if($myrow_10['type'] == 1)
			small_amount_cells(null, $myrow_10['name']);
		elseif($myrow_10['type'] == 2)
			combo1_list_cells(null, $myrow_10['name'],$item_info['$myrow_10["name"]']);
		elseif($myrow_10['type'] == 3)
			date_cells(null, $myrow_10['name'],null);
		elseif($myrow_10['type'] == 4)
			text_cells(null, $myrow_10['name'], $item_info['$myrow_10["name"]'], $myrow_10["s_width"], 40);
	}
	if($myrow_11['item_enable'])
	{
		if($myrow_11['type'] == 1)
			small_amount_cells(null, $myrow_11['name']);
		elseif($myrow_11['type'] == 2)
			combo1_list_cells(null, $myrow_11['name'],$item_info['$myrow_11["name"]']);
		elseif($myrow_11['type'] == 3)
			date_cells(null, $myrow_11['name'],null);
		elseif($myrow_11['type'] == 4)
			text_cells(null, $myrow_11['name'], $item_info['$myrow_11["name"]'], $myrow_11["s_width"], 40);
	}
	if($myrow_12['item_enable'])
	{
		if($myrow_12['type'] == 1)
			small_amount_cells(null, $myrow_12['name']);
		elseif($myrow_12['type'] == 2)
			combo1_list_cells(null, $myrow_12['name'],$item_info['$myrow_12["name"]']);
		elseif($myrow_12['type'] == 3)
			date_cells(null, $myrow_12['name'],null);
		elseif($myrow_12['type'] == 4)
			text_cells(null, $myrow_12['name'], $item_info['$myrow_12["name"]'], $myrow_12["s_width"], 40);
	}
	if($myrow_13['item_enable'])
	{
		if($myrow_13['type'] == 1)
			small_amount_cells(null, $myrow_13['name']);
		elseif($myrow_13['type'] == 2)
			combo1_list_cells(null, $myrow_13['name'],$item_info['$myrow_13["name"]']);
		elseif($myrow_13['type'] == 3)
			date_cells(null, $myrow_13['name'],null);
		elseif($myrow_13['type'] == 4)
			text_cells(null, $myrow_13['name'], $item_info['$myrow_13["name"]'], $myrow_13["s_width"], 40);
	}
	if($myrow_14['item_enable'])
	{
		if($myrow_14['type'] == 1)
			small_amount_cells(null, $myrow_14['name']);
		elseif($myrow_14['type'] == 2)
			combo1_list_cells(null, $myrow_14['name'],$item_info['$myrow_14["name"]']);
		elseif($myrow_14['type'] == 3)
			date_cells(null, $myrow_14['name'],null);
		elseif($myrow_14['type'] == 4)
			text_cells(null, $myrow_14['name'], $item_info['$myrow_14["name"]'], $myrow_14["s_width"], 40);
	}
	if($myrow_15['item_enable'])
	{
		if($myrow_15['type'] == 1)
			small_amount_cells(null, $myrow_15['name']);
		elseif($myrow_15['type'] == 2)
			combo1_list_cells(null, $myrow_15['name'],$item_info['$myrow_15["name"]']);
		elseif($myrow_15['type'] == 3)
			date_cells(null, $myrow_15['name'],null);
		elseif($myrow_15['type'] == 4)
			text_cells(null, $myrow_15['name'], $item_info['$myrow_15["name"]'], $myrow_15["s_width"], 40);
	}
	if($myrow_16['item_enable'])
	{
		if($myrow_16['type'] == 1)
			small_amount_cells(null, $myrow_16['name']);
		elseif($myrow_16['type'] == 2)
			combo1_list_cells(null, $myrow_16['name'],$item_info['$myrow_16["name"]']);
		elseif($myrow_16['type'] == 3)
			date_cells(null, $myrow_16['name'],null);
		elseif($myrow_16['type'] == 4)
			text_cells(null, $myrow_16['name'], $item_info['$myrow_16["name"]'], $myrow_16["s_width"], 40);
	}
	if($myrow_17['item_enable'])
	{
		if($myrow_17['type'] == 1)
			small_amount_cells(null, $myrow_17['name']);
		elseif($myrow_17['type'] == 2)
			combo1_list_cells(null, $myrow_17['name'],$item_info['$myrow_17["name"]']);
		elseif($myrow_17['type'] == 3)
			date_cells(null, $myrow_17['name'],null);
		elseif($myrow_17['type'] == 4)
			text_cells(null, $myrow_17['name'], $item_info['$myrow_17["name"]'], $myrow_17["s_width"], 40);
	}
	if($myrow_18['item_enable'])
	{
		if($myrow_18['type'] == 1)
		{
			small_amount_cells(null, $myrow_18['name']);
		}
		elseif($myrow_18['type'] == 2)
		{	combo1_list_cells(null, $myrow_18['name'],$item_info['$myrow_18["name"]']);
		}
		elseif($myrow_18['type'] == 3)
			date_cells(null, $myrow_18['name'],null);
		elseif($myrow_18['type'] == 4)
		{
			text_cells(null, $myrow_18['name'], $item_info['$myrow_18["name"]'], $myrow_18["s_width"], 20);
		}
	}
	if($myrow_19['item_enable'])
	{
		if($myrow_19['type'] == 1)
		{
			small_amount_cells(null, $myrow_19['name']);
		}
		elseif($myrow_19['type'] == 2)
		{	combo1_list_cells(null, $myrow_19['name'],$item_info['$myrow_19["name"]']);
		}
		elseif($myrow_19['type'] == 3)
			date_cells(null, $myrow_19['name'],null);
		elseif($myrow_19['type'] == 4)
		{	text_cells(null, $myrow_19['name'], $item_info['$myrow_19["name"]'], $myrow_19["s_width"], 20);
		}
	}
	if($myrow_20['item_enable'])
	{
		if($myrow_20['type'] == 1)
			small_amount_cells(null, $myrow_20['name']);
		elseif($myrow_20['type'] == 2)
			combo1_list_cells(null, $myrow_20['name'],$item_info['$myrow_20["name"]']);
		elseif($myrow_20['type'] == 3)
			date_cells(null, $myrow_20['name'],null);
		elseif($myrow_20['type'] == 4)
			text_cells(null, $myrow_20['name'], $item_info['$myrow_20["name"]'], $myrow_20["s_width"], 40);
	}
	if($myrow_21['item_enable'])
	{
		if($myrow_21['type'] == 1)
			small_amount_cells(null, $myrow_21['name']);
		elseif($myrow_21['type'] == 2)
			combo1_list_cells(null, $myrow_21['name'],$item_info['$myrow_21["name"]']);
		elseif($myrow_21['type'] == 3)
			date_cells(null, $myrow_21['name'],null);
		elseif($myrow_21['type'] == 4)
			text_cells(null, $myrow_21['name'], $item_info['$myrow_21["name"]'], $myrow_21["s_width"], 40);
	}
	if ($order->fixed_asset) {
		hidden('qty', 1);
		qty_cell(1, false, 0);
	} else
		small_qty_cells(null, 'qty', $_POST['qty'], null, null, $dec);

	label_cell($_POST['units'], '', 'units');

	if ($id != -1)
	{
		button_cell('UpdateItem', _("Update"),
				_('Confirm changes'), ICON_UPDATE);
		button_cell('CancelItemChanges', _("Cancel"),
				_('Cancel changes'), ICON_CANCEL);
		hidden('LineNo', $line_no);
 		set_focus('qty');
	}
	else
	{
		submit_cells('AddItem', _("Add Item"), "colspan=2",
		    _('Add new item to document'), true);
	}

	end_row();
}


//---------------------------------------------------------------------------------

function transfer_options_controls()
{
	  echo "<br>";
	  start_table();

	  textarea_row(_("Memo"), 'memo_', null, 50, 3);

	  end_table(1);
}


//---------------------------------------------------------------------------------

