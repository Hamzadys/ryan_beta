<?php

////////////////footer combo 1,2,3
function add_f_combo1($description)
{
	$sql = "INSERT INTO ".TB_PREF."f_combo1 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_f_combo1($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."f_combo1 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_f_combo1($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."f_combo1 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo");
}

function get_f_combo1($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo1";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combof_code1($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo1 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}

///////////////combo_2
function add_f_combo2($description)
{
	$sql = "INSERT INTO ".TB_PREF."f_combo2 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_f_combo2($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."f_combo2 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_f_combo2($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."f_combo2 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_f_combo2($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo2";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_f_combo_code2($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo2 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
/////////////////////////combo 3

function add_f_combo3($description)
{
	$sql = "INSERT INTO ".TB_PREF."f_combo3 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_f_combo3($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."f_combo3 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_f_combo3($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."f_combo3 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_f_combo3($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo3";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_f_combo_code3($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."f_combo3 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}




function add_ph_combo_1($description)
{
	$sql = "INSERT INTO ".TB_PREF."ph_combo1 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_ph_combo_1($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."ph_combo1 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_ph_combo_1($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."ph_combo1 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo");
}

function get_ph_combo_1($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo1";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_ph_combo_code($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo1 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}

///////////////combo_2
function add_ph_combo_2($description)
{
	$sql = "INSERT INTO ".TB_PREF."ph_combo2 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_ph_combo_2($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."ph_combo2 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_ph_combo_2($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."ph_combo2 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_ph_combo_2($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo2";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_ph_combo_code_2($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo2 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
/////////////////////////combo 3

function add_ph_combo_3($description)
{
	$sql = "INSERT INTO ".TB_PREF."ph_combo3 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_ph_combo_3($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."ph_combo3 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_ph_combo_3($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."ph_combo3 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_ph_combo_3($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo3";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_ph_combo_code_3($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."ph_combo3 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}

/////////////////////////////////////////////////////////////////


////////////////////////////////hareem
function add_combo_1($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_1 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_1($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_1 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_1($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_1 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo");
}

function get_combo_1($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_1";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_1 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}

///////////////combo_2
function add_combo_2($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_2 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_2($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_2 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_2($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_2 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_combo_2($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_2";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_2($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_2 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
/////////////////////////combo 3

function add_combo_3($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_3 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_3($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_3 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_3($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_3 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_combo_3($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_3";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_3($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_3 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
//////////////////////////hareeeeeem end
////////////////sales order
/////////combo 1,2,3
function add_combo_so_1($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_1 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_1($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_1 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_1($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_1 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo");
}

function get_combo_so_1($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_1";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_1($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_1 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}

function add_combo_so_2($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_2 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_2($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_2 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_2($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_2 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_combo_so_2($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_2";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_2($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_2 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
/////////////////////////combo 3

function add_combo_so_3($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_3 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_3($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_3 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_3($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_3 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_combo_so_3($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_3";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_3($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_3 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
//////////////combo 4
function add_combo_so_4($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_4 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_4($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_4 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_4($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_4 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 2");
}

function get_combo_so_4($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_4";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_4($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_4 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
//////////////combo 5
function add_combo_so_5($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_5 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_5($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_5 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_5($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_5 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 5");
}

function get_combo_so_5($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_5";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_5($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_5 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
//////////////combo 6
function add_combo_so_6($description)
{
	$sql = "INSERT INTO ".TB_PREF."combo_so_6 (description) VALUES (".db_escape($description) . ")";
	db_query($sql,"The combo could not be added");
}

function update_combo_so_6($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."combo_so_6 SET description=".db_escape($description)." WHERE combo_code = ".db_escape($selected_id);
	db_query($sql,"The combo could not be updated");
}

function delete_combo_so_6($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."combo_so_6 WHERE combo_code=".db_escape($selected_id);
	db_query($sql,"could not delete combo 6");
}

function get_combo_so_6($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_6";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get combo");
}

function get_combo_code_so_6($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."combo_so_6 WHERE combo_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get combo");
	return db_fetch($result);
}
////////////////////////



function add_sales_group($description)
{
	$sql = "INSERT INTO ".TB_PREF."groups (description) VALUES (".db_escape($description) . ")";
   	db_query($sql,"The sales group could not be added");
}

function update_sales_group($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."groups SET description=".db_escape($description)." WHERE id = ".db_escape($selected_id);
   	db_query($sql,"The sales group could not be updated");
}

function delete_sales_group($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."groups WHERE id=".db_escape($selected_id);
	db_query($sql,"could not delete sales group");
}

function get_sales_groups($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."groups";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	$sql .= " ORDER BY description";
	return db_query($sql,"could not get groups");
}

function get_sales_group($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."groups WHERE id=".db_escape($selected_id);

	$result = db_query($sql,"could not get group");
	return db_fetch($result);
}

function get_sales_group_name($group_no)
{
	$sql = "SELECT description FROM ".TB_PREF."groups WHERE id = ".db_escape($group_no);
	$result = db_query($sql, "could not get group");
	$row = db_fetch($result);
	return $row[0];
}

function add_sales_area($description)
{
	$sql = "INSERT INTO ".TB_PREF."areas (description) VALUES (".db_escape($description) . ")";
   	db_query($sql,"The sales area could not be added");
}

function update_sales_area($selected_id, $description)
{
	$sql = "UPDATE ".TB_PREF."areas SET description=".db_escape($description)." WHERE area_code = ".db_escape($selected_id);
   	db_query($sql,"The sales area could not be updated");
}

function delete_sales_area($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."areas WHERE area_code=".db_escape($selected_id);
	db_query($sql,"could not delete sales area");
}

function get_sales_areas($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."areas";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get areas");
}

function get_sales_area($selected_id)
{
	$sql = "SELECT * FROM ".TB_PREF."areas WHERE area_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get area");
	return db_fetch($result);
}

function get_area_name($id)
{
	$sql = "SELECT description FROM ".TB_PREF."areas WHERE area_code=".db_escape($id);

	$result = db_query($sql, "could not get sales type");

	$row = db_fetch_row($result);
	return $row[0];
}

function add_salesman($salesman_name, $salesman_phone, $salesman_fax,
	$salesman_email, $provision, $break_pt, $provision2)
{
	$sql = "INSERT INTO ".TB_PREF."salesman (salesman_name, salesman_phone, salesman_fax, salesman_email,
		provision, break_pt, provision2)
		VALUES (".db_escape($salesman_name) . ", "
		  .db_escape($salesman_phone) . ", "
		  .db_escape($salesman_fax) . ", "
		  .db_escape($salesman_email) . ", ".
		$provision.", ".$break_pt.", "
		.$provision2.")";
   	db_query($sql,"The insert of the sales person failed");
}

function update_salesman($selected_id, $salesman_name, $salesman_phone, $salesman_fax,
	$salesman_email, $provision, $break_pt, $provision2)
{
	$sql = "UPDATE ".TB_PREF."salesman SET salesman_name=".db_escape($salesman_name) . ",
		salesman_phone=".db_escape($salesman_phone) . ",
		salesman_fax=".db_escape($salesman_fax) . ",
		salesman_email=".db_escape($salesman_email) . ",
		provision=".$provision.",
		break_pt=".$break_pt.",
		provision2=".$provision2."
		WHERE salesman_code = ".db_escape($selected_id);
   	db_query($sql,"The update of the sales person failed");
}

function delete_salesman($selected_id)
{
	$sql="DELETE FROM ".TB_PREF."salesman WHERE salesman_code=".db_escape($selected_id);
	db_query($sql,"The sales-person could not be deleted");
}

function get_salesmen($show_inactive)
{
	$sql = "SELECT * FROM ".TB_PREF."salesman";
	if (!$show_inactive) $sql .= " WHERE !inactive";
	return db_query($sql,"could not get sales persons");
}

function get_salesman($selected_id)
{
	$sql = "SELECT *  FROM ".TB_PREF."salesman WHERE salesman_code=".db_escape($selected_id);

	$result = db_query($sql,"could not get sales person");
	return db_fetch($result);
}

function get_salesman_name($id)
{
	$sql = "SELECT salesman_name FROM ".TB_PREF."salesman WHERE salesman_code=".db_escape($id);

	$result = db_query($sql, "could not get sales type");

	$row = db_fetch_row($result);
	return $row[0];
}



