<?php

$page_security = 'SA_GLACCOUNT';
$path_to_root = "../..";
include($path_to_root . "/includes/session.inc");

$js = "";
if ($SysPrefs->use_popup_windows && $SysPrefs->use_popup_search)
	$js .= get_js_open_window(900, 500);

page(_($help_context = "Chart of Accounts"), false, false, "", $js);

include($path_to_root . "/includes/ui.inc");
include($path_to_root . "/gl/includes/gl_db.inc");
include($path_to_root . "/admin/db/tags_db.inc");
include_once($path_to_root . "/includes/data_checks.inc");

check_db_has_gl_account_groups(_("There are no account groups defined. Please define at least one account group before entering accounts."));

//-------------------------------------------------------------------------------------

if (isset($_POST['_AccountList_update']))
{
	$_POST['selected_account'] = $_POST['AccountList'];
	unset($_POST['account_code']);
}

if (isset($_POST['selected_account']))
{
	$selected_account = $_POST['selected_account'];
}
elseif (isset($_GET['selected_account']))
{
	$selected_account = $_GET['selected_account'];
}
else
	$selected_account = "";
//-------------------------------------------------------------------------------------

if (isset($_POST['add']) || isset($_POST['update']))
{

	$input_error = 0;
    if(isset($_POST['add'])){
	if (strlen(trim($_POST['account_code'])) == 0)
	{
		$input_error = 1;
		display_error( _("The account code must be entered."));
		set_focus('account_code');
	}
	elseif (strlen(trim($_POST['account_name'])) == 0)
	{
		$input_error = 1;
		display_error( _("The account name cannot be empty."));
		set_focus('account_name');
	}
	elseif (!$SysPrefs->accounts_alpha() && !preg_match("/^[0-9.]+$/",$_POST['account_code'])) // we only allow 0-9 and a dot
	{
	    $input_error = 1;
	    display_error( _("The account code must be numeric."));
		set_focus('account_code');
	}}
	if ($input_error != 1)
	{
		if ($SysPrefs->accounts_alpha() == 2)
			$_POST['account_code'] = strtoupper($_POST['account_code']);

		if (!isset($_POST['account_tags']))
			$_POST['account_tags'] = array();

    	if ($selected_account)
		{
			if (get_post('inactive') == 1 && is_bank_account($_POST['account_code']))
			{
				display_error(_("The account belongs to a bank account and cannot be inactivated."));
			}
    		elseif (update_gl_account($_POST['account_code'], $_POST['account_name'],
				$_POST['account_type'], $_POST['account_code2'])) {
				update_record_status($_POST['account_code'], $_POST['inactive'],
					'chart_master', 'account_code');
				update_tag_associations(TAG_ACCOUNT, $_POST['account_code'],
					$_POST['account_tags']);
				$Ajax->activate('account_code'); // in case of status change
				display_notification(_("Account data has been updated."));
			}
		}
    	else {
            if (!isset($_POST['update'])) {
                if (add_gl_account($_POST['account_code'], $_POST['account_name'],
                    $_POST['account_type'], $_POST['account_code2'])) {
                    if (isset($_POST['add'])) {
                        add_tag_associations($_POST['account_code'], $_POST['account_tags']);
                        display_notification(_("New account has been added."));
                        $selected_account = $_POST['AccountList'] = $_POST['account_code'];
                    }
                } else {
                    if (isset($_POST['add'])) {
                        display_error(_("Account not added, possible duplicate Account Code."));
                    }
                }

            }
        }
		$Ajax->activate('_page_body');
	}
}

//-------------------------------------------------------------------------------------

function can_delete($selected_account)
{
	if ($selected_account == "")
		return false;

	if (key_in_foreign_table($selected_account, 'gl_trans', 'account'))
	{
		display_error(_("Cannot delete this account because transactions have been created using this account."));
		return false;
	}

	if (gl_account_in_company_defaults($selected_account))
	{
		display_error(_("Cannot delete this account because it is used as one of the company default GL accounts."));
		return false;
	}

	if (key_in_foreign_table($selected_account, 'bank_accounts', 'account_code'))
	{
		display_error(_("Cannot delete this account because it is used by a bank account."));
		return false;
	}

	if (gl_account_in_stock_category($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more Item Categories."));
		return false;
	}

	if (gl_account_in_stock_master($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more Items."));
		return false;
	}

	if (gl_account_in_tax_types($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more Taxes."));
		return false;
	}

	if (gl_account_in_cust_branch($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more Customer Branches."));
		return false;
	}
	if (gl_account_in_suppliers($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more suppliers."));
		return false;
	}

	if (gl_account_in_quick_entry_lines($selected_account))
	{
		display_error(_("Cannot delete this account because it is used by one or more Quick Entry Lines."));
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------

if (isset($_POST['delete']))
{

	if (can_delete($selected_account))
	{
		delete_gl_account($selected_account);
		$selected_account = $_POST['AccountList'] = '';
		delete_tag_associations(TAG_ACCOUNT,$selected_account, true);
		$selected_account = $_POST['AccountList'] = '';
		display_notification(_("Selected account has been deleted"));
		unset($_POST['account_code']);
		$Ajax->activate('_page_body');
	}
}

//-------------------------------------------------------------------------------------

start_form();

/*
    gl_account_types_list_cells(_("Account Group:"), 'account_type1', null, false, false, true);
*/
if (db_has_gl_accounts())
{
	start_table(TABLESTYLE_NOBORDER);
	start_row();

	$myrow_ = get_gl_account($selected_account);

    echo '<a href="https://erp30.com/reporting/prn_redirect.php?PARAM_0=1-12&PARAM_1=1-12&PARAM_2=&PARAM_3=&PARAM_4=0&REP_ID=701" class="btn btn-info" role="button" target="_blank">Print COA</a>';


    echo '<a href="https://erp30.com/reporting/prn_redirect.php?PARAM_0=1-12&PARAM_1=1-12&PARAM_2=&PARAM_3=1-12&PARAM_4=0&REP_ID=701" class="btn btn-info" role="button" target="_blank">Export COA</a>';

   gl_account_types_list_cells(_("Account Group:"), 'account_type1', $myrow_["account_type"], false, false, true);   
      $_POST['account_type']=get_post('account_type1');

    gl_all_accounts_list_cells(_("GL Account:"), 'AccountList', null, false, false,_('New account'), true, check_value('show_inactive'));
	check_cells(_("Show inactive:"), 'show_inactive', null, true);
	end_row();
	end_table();
	if (get_post('_show_inactive_update')) {
		$Ajax->activate('AccountList');
		set_focus('AccountList');
	}
}

br(1);
start_table(TABLESTYLE2);

if ($selected_account != "")
{
	//editing an existing account
	$myrow = get_gl_account($selected_account);

	$_POST['account_code'] = $myrow["account_code"];
	$_POST['account_code2'] = $myrow["account_code2"];
	$_POST['account_name']	= $myrow["account_name"];
	$_POST['account_type'] = $myrow["account_type"];
 	$_POST['inactive'] = $myrow["inactive"];

 	$tags_result = get_tags_associated_with_record(TAG_ACCOUNT, $selected_account);
 	$tagids = array();
 	while ($tag = db_fetch($tags_result))
 	 	$tagids[] = $tag['id'];
 	$_POST['account_tags'] = $tagids;

	hidden('account_code', $_POST['account_code']);
	hidden('selected_account', $selected_account);


	label_row(_("Account Code:"), $_POST['account_code']);
        }
        else
        {
	if (!isset($_POST['account_code'])) {
		$_POST['account_tags'] = array();
		$_POST['account_code'] = $_POST['account_code2'] = '';
		$_POST['account_name']	= $_POST['account_type'] = '';
 		$_POST['inactive'] = 0;
	}

    $code = 0;
    submit_center('update', _("Fetch Account Code"), "colspan=2 align='center'", _("Refresh"), true);
    if($_POST['account_type1'] !='') {
        if (isset($_POST['update'])) {
//dz 15.6.17
    if(check_presence_in_group($_POST['account_type1']) == 0 && check_presence_in_coa($_POST['account_type1']) == 0 ) //if the account does not have a child both in COA and group
    {
           $code = $_POST['account_type1'] . '000' . get_code_increment_from_coa($_POST['account_type1']);
    }
   elseif(check_presence_in_group($_POST['account_type1']) > 0 && check_presence_in_coa($_POST['account_type1']) > 0)
    {
            $code = get_code_increment_from_coa($_POST['account_type1']);
    }
   elseif(check_presence_in_coa($_POST['account_type1']) > 0) 
    {
            $code = get_code_increment_from_coa($_POST['account_type1']);
    }
    else
    {
            $code = get_code_increment_from_group($_POST['account_type1']);
    }


            $_POST['account_code'] = $code;
            $Ajax->activate('account_code');

        }
        text_row_ex(_("Account Code:"), 'account_code', 15);
    }
}
hidden('account_type', $_POST['account_type1']);

text_row_ex(_("Account Code 2:"), 'account_code2', 15);

text_row_ex(_("Account Name:"), 'account_name', 60);

//gl_account_types_list_row(_("Account Group:"), 'account_type', null);

tag_list_row(_("Account Tags:"), 'account_tags', 5, TAG_ACCOUNT, true);

record_status_list_row(_("Account status:"), 'inactive');
end_table(1);

if ($selected_account == "")
{
	submit_center('add', _("Add Account"), true, '', 'default');
}
else
{
    submit_center_first('update', _("Update Account"), '', 'default');
    submit_center_last('delete', _("Delete account"), '',true);
}
end_form();

end_page();

