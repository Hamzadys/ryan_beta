<?php


function update_item($stock_id, $description, $long_description,$carton, $category_id, 
	$tax_type_id, $units='', $mb_flag='', $sales_account, $inventory_account, 
	$cogs_account, 	$adjustment_account, $wip_account, $dimension_id, 
	$dimension2_id, $no_sale, $editable, $no_purchase,
	$depreciation_method = 'D', $depreciation_rate=100, $depreciation_factor=1,
	$depreciation_start=null, $fa_class_id=null,$salesman,$batch_status,$amount1,$amount2,$amount3,$amount4,$amount5,$amount6
,$text1,$text2,$text3,$combo1,$combo2,$combo3,$combo4,$combo5,$combo6,$alt_units,$con_factor,$con_type)
{
	$sql = "UPDATE ".TB_PREF."stock_master SET long_description=".db_escape($long_description).",
		description=".db_escape($description).",
carton=".db_escape($carton).",
		category_id=".db_escape($category_id).",
		sales_account=".db_escape($sales_account).",
		inventory_account=".db_escape($inventory_account).",
		cogs_account=".db_escape($cogs_account).",
		adjustment_account=".db_escape($adjustment_account).",
		wip_account=".db_escape($wip_account).",
		dimension_id=".db_escape($dimension_id).",
		dimension2_id=".db_escape($dimension2_id).",
		tax_type_id=".db_escape($tax_type_id).",
		no_sale=".db_escape($no_sale).",
		no_purchase=".db_escape($no_purchase).",
		editable=".db_escape($editable).",
		depreciation_method=".db_escape($depreciation_method).",
		depreciation_rate=".db_escape($depreciation_rate).",
		depreciation_factor=".db_escape($depreciation_factor).",
		fa_class_id=".db_escape($fa_class_id).",
		salesman=".db_escape($salesman).",
		batch_status=".db_escape($batch_status).",
		amount1=".db_escape($amount1).",
		amount2=".db_escape($amount2).",
		amount3=".db_escape($amount3).",
		amount4=".db_escape($amount4).",
		amount5=".db_escape($amount5).",
		amount6=".db_escape($amount6).",
		text1=".db_escape($text1).",
		text2=".db_escape($text2).",
		text3=".db_escape($text3).",
		combo1=".db_escape($combo1).",
		combo2=".db_escape($combo2).",
		combo3=".db_escape($combo3).",
		combo4=".db_escape($combo4).",
		combo5=".db_escape($combo5).",
		combo6=".db_escape($combo6).",
		alt_units=".db_escape($alt_units).",
		con_factor=".db_escape($con_factor).",
		con_type=".db_escape($con_type);

	if ($units != '')
		$sql .= ", units=".db_escape($units);

	if ($mb_flag != '')
		$sql .= ", mb_flag=".db_escape($mb_flag);

	if ($depreciation_start != '') {
		$sql .= ", depreciation_start='".date2sql($depreciation_start)."'"
			.", depreciation_date='".date2sql($depreciation_start)."'";
	}

	$sql .= " WHERE stock_id=".db_escape($stock_id);

	db_query($sql, "The item could not be updated");

	update_item_code(-1, $stock_id, $stock_id, $description, $category_id, 1, 0);
}

function add_item2($stock_id, $description, $long_description,$carton, $category_id,
	$tax_type_id, $units, $mb_flag,	$sales_account, $inventory_account, 
	$cogs_account, $adjustment_account,	$wip_account, $dimension_id, 
	$dimension2_id, $no_sale, $editable, $no_purchase,
	$depreciation_method='D', $depreciation_rate=100,  $depreciation_factor=1, $depreciation_start=null,
	$fa_class_id=null,$salesman,$batch_status,$amount1,$amount2,$amount3,$amount4,$amount5,$amount6
	,$text1,$text2,$text3,$combo1,$combo2,$combo3,$combo4,$combo5,$combo6,$alt_units,$con_factor,$con_type)
{
	$sql = "INSERT INTO ".TB_PREF."stock_master (stock_id, description, long_description,carton, category_id,
		tax_type_id, units, mb_flag, sales_account, inventory_account, cogs_account,
		adjustment_account, wip_account, dimension_id, dimension2_id, no_sale, no_purchase, editable,
		depreciation_method, depreciation_rate, depreciation_factor, depreciation_start
		, depreciation_date, fa_class_id, salesman,batch_status,amount1,amount2,amount3,amount4,amount5,amount6
		,text1,text2,text3,combo1,combo2,combo3,combo4,combo5,combo6,alt_units,con_factor,con_type)
		VALUES (".db_escape($stock_id).", ".db_escape($description).", 
		".db_escape($long_description).", ".db_escape($carton).",
		".db_escape($category_id).", ".db_escape($tax_type_id).", "
		.db_escape($units).", ".db_escape($mb_flag).",
		".db_escape($sales_account).", ".db_escape($inventory_account)
		.", ".db_escape($cogs_account).",".db_escape($adjustment_account)
		.", ".db_escape($wip_account).", "
		.db_escape($dimension_id).", ".db_escape($dimension2_id).","
		.db_escape($no_sale).","
		.db_escape($no_purchase).","
		.db_escape($editable).","
		.db_escape($depreciation_method).",".db_escape($depreciation_rate)."
		,".db_escape($depreciation_factor).","
		.db_escape(date2sql($depreciation_start)).","
		.db_escape(date2sql($depreciation_start)).","
		.db_escape($fa_class_id).","
		.db_escape($salesman).","
		.db_escape($batch_status).","
		.db_escape($amount1).","
		.db_escape($amount2).","
		.db_escape($amount3).","
		.db_escape($amount4).","
		.db_escape($amount5).","
		.db_escape($amount6).","
		.db_escape($text1).","
		.db_escape($text2).","
		.db_escape($text3).","
		.db_escape($combo1).","
		.db_escape($combo2).","
		.db_escape($combo3).","
		.db_escape($combo4).","
		.db_escape($combo5).","
		.db_escape($combo6).","
		.db_escape($alt_units).","
		.db_escape($con_factor).","
		.db_escape($con_type).")";

	db_query($sql, "The item could not be added");

	$sql = "INSERT INTO ".TB_PREF."loc_stock (loc_code, stock_id)
		SELECT loc_code, ".db_escape($stock_id)
		." FROM ".TB_PREF."locations";

	db_query($sql, "The item locstock could not be added");

	add_item_code($stock_id, $stock_id, $description, $category_id, 1, 0);
}


function delete_item($stock_id)
{
	$sql="DELETE FROM ".TB_PREF."stock_master WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item");

	/*and cascade deletes in loc_stock */
	$sql ="DELETE FROM ".TB_PREF."loc_stock WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item loc stock");

	/*and cascade deletes in purch_data */
	$sql ="DELETE FROM ".TB_PREF."purch_data WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item purch data");

	/*and cascade deletes in prices */
	$sql ="DELETE FROM ".TB_PREF."prices WHERE stock_id=".db_escape($stock_id);
	db_query($sql, "could not delete stock item prices");

	/*and cascade delete the bill of material if any */
	$sql = "DELETE FROM ".TB_PREF."bom WHERE parent=".db_escape($stock_id);
	db_query($sql, "could not delete stock item bom");

	delete_item_kit($stock_id);
}

function get_item($stock_id)
{
	$sql = "SELECT item.*, taxtype.name AS tax_type_name
		FROM ".TB_PREF."stock_master item,"
			.TB_PREF."item_tax_types taxtype
		WHERE taxtype.id=item.tax_type_id
		AND stock_id=".db_escape($stock_id);
	$result = db_query($sql,"an item could not be retreived");

	return db_fetch($result);
}

function get_items($fixed_asset = 0)
{
	$sql = "SELECT * FROM ".TB_PREF."stock_master WHERE fixed_asset=".db_escape($fixed_asset);
	return db_query($sql,"items could not be retreived");
}

function item_in_foreign_codes($stock_id)
{
	$sqls=  array(
	"SELECT COUNT(*) FROM "
		.TB_PREF."stock_moves WHERE stock_id=".db_escape($stock_id) =>
	 _('Cannot delete this item because there are stock movements that refer to this item.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."bom WHERE component=".db_escape($stock_id)=>
	 _('Cannot delete this item record because there are bills of material that require this part as a component.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."sales_order_details WHERE stk_code=".db_escape($stock_id) =>
	 _('Cannot delete this item because there are existing purchase order items for it.'),
	"SELECT COUNT(*) FROM "
		.TB_PREF."purch_order_details WHERE item_code=".db_escape($stock_id)=>
	 _('Cannot delete this item because there are existing purchase order items for it.')
	);

	$msg = '';

	foreach($sqls as $sql=>$err) {
		$result = db_query($sql, "could not query stock usage");
		$myrow = db_fetch_row($result);
		if ($myrow[0] > 0) 
		{
			$msg = $err; break;
		}
	}
	if ($msg == '') {

		$kits = get_where_used($stock_id);
		$num_kits = db_num_rows($kits);
		if ($num_kits) {
			$msg = _("This item cannot be deleted because some code aliases 
				or foreign codes was entered for it, or there are kits defined 
				using this item as component")
				.':<br>';

			while($num_kits--) {
				$kit = db_fetch($kits);
				$msg .= "'".$kit[0]."'";
				if ($num_kits) $msg .= ',';
			}

		}
	}
	return $msg;
}

function get_items_search($description, $type)
{
	global $SysPrefs;

	$sql = "SELECT COUNT(i.item_code) AS kit, i.item_code, i.description, c.description category
		FROM ".TB_PREF."stock_master s, ".TB_PREF."item_codes i
			LEFT JOIN ".TB_PREF."stock_category c ON i.category_id=c.category_id
		WHERE i.stock_id=s.stock_id
			AND !i.inactive AND !s.inactive
			AND (  i.item_code LIKE " . db_escape("%" . $description. "%") . " OR 
				i.description LIKE " . db_escape("%" . get_post("description"). "%") . " OR 
				c.description LIKE " . db_escape("%" . get_post("description"). "%") . ") ";

	switch ($type) {
		case "sales":
			$sql .= " AND !s.no_sale AND mb_flag <> 'F'";
			break;
		case "manufactured":
			$sql .= " AND mb_flag = 'M'";
			break;
    	case "purchasable":
    		$sql .= " AND NOT no_purchase AND mb_flag <> 'F' AND i.item_code=i.stock_id";
    		break;
		case "costable":
			$sql .= " AND mb_flag <> 'D' AND mb_flag <> 'F' AND  i.item_code=i.stock_id";
			break;
		case "component":
			$parent = $_GET['parent'];
			$sql .= " AND  i.item_code=i.stock_id AND i.stock_id <> '$parent' AND mb_flag <> 'F' ";
			break;
		case "kits":
			$sql .= " AND !i.is_foreign AND i.item_code!=i.stock_id AND mb_flag <> 'F'";
			break;
		case "all":
			$sql .= " AND mb_flag <> 'F' AND i.item_code=i.stock_id";
			break;
	}

	if (isset($SysPrefs->max_rows_in_search))
		$limit = $SysPrefs->max_rows_in_search;
	else
		$limit = 10;

	$sql .= " GROUP BY i.item_code ORDER BY i.description LIMIT 0,".(int)($limit);

	return db_query($sql, "Failed in retreiving item list.");
}

///////for supplier_dashboard---------------marina----------
function get_sql_for_items_log_view($stock_id, $description, $carton, $long_description, $category_id,
									$tax_type_id, $mb_flag='', $units='', $editable, $no_sale, $no_purchase,
									$salesman, $sales_account, $inventory_account,
									$cogs_account,$adjustment_account, $dimension_id, $dimension2_id)
{

	$sql = "SELECT
			stock_id,
			description,
			carton,
			long_description,
			category_id,
			tax_type_id,
			mb_flag,
			units,
			editable,
			no_sale,
			no_purchase,
			salesman,
			sales_account,	
			inventory_account,
			cogs_account,
  			adjustment_account,
  			dimension_id,
			dimension2_id
			
		FROM ".TB_PREF."stock_master
		WHERE stock_id != -1 ";

//	$sql .= " GROUP BY stock_id";

	//marina-----------------start-------------
	if ($stock_id != "")
	{
		$number_like = "%".$stock_id."%";
		$sql .= " AND stock_id LIKE ".db_escape($number_like);
	}

	if ($description != "")
	{
		$number_like = "%".$description."%";
		$sql .= " AND description LIKE ".db_escape($number_like);
	}

	if ($carton != "")
	{
		$number_like = "%".$carton."%";
		$sql .= " AND carton LIKE ".db_escape($number_like);
	}

	if ($long_description != "")
	{
		$number_like = "%".$long_description."%";
		$sql .= " AND long_description LIKE ".db_escape($number_like);
	}

	if ($category_id != -1)
	{
		$sql .= " AND category_id = ".db_escape($category_id);
	}

	if ($tax_type_id != null)
	{
		$sql .= " AND tax_type_id = ".db_escape($tax_type_id);
	}

	if ($mb_flag != '')
	{
		$sql .= " AND mb_flag = ".db_escape($mb_flag);
	}

	if ($units != ''  )
	{
		$sql .= " AND units = ".db_escape($units);
	}

	if ($salesman != -1 )
	{
		$sql .= " AND salesman = ".db_escape($salesman);
	}

	if ($dimension_id != "")
	{
		$sql .= " AND dimension_id = ".db_escape($dimension_id);
	}

	if ($dimension2_id != "")
	{
		$sql .= " AND dimension2_id = ".db_escape($dimension2_id);
	}

	$sql .= " GROUP BY stock_id";

	return $sql;
}


///marina-------end-------------