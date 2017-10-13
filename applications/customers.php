<?php

class customers_app extends application
{
	function customers_app()
	{
		$this->application("orders",_($this->help_context ="&Sales"));

		$this->add_module(_("Transactions"));

		if (get_company_pref_display('sales_quotation'))
			$this->add_lapp_function(0,_(get_company_pref_display('sales_quotation_text')),
				"sales/sales_order_entry.php?NewQuotation=Yes",'SA_SALESQUOTE', MENU_TRANSACTION);

		if (get_company_pref_display('sale_order_entry'))
			$this->add_lapp_function(0,_(get_company_pref_display('sale_order_entry_text')),
				"sales/sales_order_entry.php?NewOrder=Yes",'SA_SALESORDER', MENU_TRANSACTION);
		$this->add_lapp_function(0, "","");

		if (get_company_pref_display('delivery_against'))
			$this->add_lapp_function(0,_(get_company_pref_display('delivery_against_text')),
				"sales/inquiry/sales_orders_view.php?OutstandingOnly=1",'SA_SALESDELIVERY', MENU_TRANSACTION);

		if (get_company_pref_display('invoice_against'))
			$this->add_lapp_function(0,_(get_company_pref_display('invoice_against_text')),
				"sales/inquiry/sales_deliveries_view.php?OutstandingOnly=1",'SA_SALESINVOICE', MENU_TRANSACTION);
		//$this->add_lapp_function(0, _("Direct &Delivery"),
		//	"sales/sales_order_entry.php?NewDelivery=0",'SA_SALESDELIVERY', MENU_TRANSACTION);
		if (get_company_pref_display('direct_invoice'))
			$this->add_lapp_function(0,_(get_company_pref_display('direct_invoice_text')),
				"sales/sales_order_entry.php?NewInvoice=0",'SA_SALESINVOICE', MENU_TRANSACTION);

		//$this->add_rapp_function(0, _("&Template Delivery"),
		//"sales/inquiry/sales_orders_view.php?DeliveryTemplates=Yes", 'SA_SALESDELIVERY', MENU_TRANSACTION);
		//$this->add_rapp_function(0, _("&Template Invoice"),
		//"sales/inquiry/sales_orders_view.php?InvoiceTemplates=Yes", 'SA_SALESINVOICE', MENU_TRANSACTION);
		//$this->add_rapp_function(0, _("&Create and Print Recurrent Invoices"),
		//"sales/create_recurrent_invoices.php?", 'SA_SALESINVOICE', MENU_TRANSACTION);
  if (get_company_pref_display('invoice_prepaid_order'))
		$this->add_lapp_function(0,_(get_company_pref_display('invoice_prepaid_order_text')),
		"sales/inquiry/sales_orders_view.php?PrepaidOrders=Yes", 'SA_SALESINVOICE', MENU_TRANSACTION);



		$this->add_rapp_function(0, "","");
		if (get_company_pref_display('customer_payment'))
			$this->add_rapp_function(0,_(get_company_pref_display('customer_payment_text')),
				"sales/customer_payments.php?",'SA_SALESPAYMNT', MENU_TRANSACTION);
		//$this->add_lapp_function(0,_("Invoice &Prepaid Orders"),
		//"sales/inquiry/sales_orders_view.php?PrepaidOrders=Yes", 'SA_SALESINVOICE', MENU_TRANSACTION);
		if (get_company_pref_display('customer_credit_note'))
			$this->add_rapp_function(0,_(get_company_pref_display('customer_credit_note_text')),
				"sales/credit_note_entry.php?NewCredit=Yes",'SA_SALESCREDIT', MENU_TRANSACTION);

		if (get_company_pref_display('allocate'))
			$this->add_rapp_function(0,_(get_company_pref_display('allocate_text')),
				"sales/allocations/customer_allocation_main.php?",'SA_SALESALLOC', MENU_TRANSACTION);

		$this->add_module(_("Inquiries and Reports"));
		if (get_company_pref_display('sales_quotation_inquiry'))
			$this->add_lapp_function(1,_(get_company_pref_display('sales_quotation_inquiry_text')),
				"sales/inquiry/sales_orders_view.php?type=32",'SA_SALESTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('sale_order_inquiry'))
			$this->add_lapp_function(1,_(get_company_pref_display('sale_order_inquiry_text')),
				"sales/inquiry/sales_orders_view.php?type=30",'SA_SALESTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('customer_inquiry'))
			$this->add_lapp_function(1,_(get_company_pref_display('customer_inquiry_text')),
				"sales/inquiry/customer_inquiry.php?",'SA_SALESTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('customer_allocate_inquiry'))
			$this->add_lapp_function(1,_(get_company_pref_display('customer_allocate_inquiry_text')),
				"sales/inquiry/customer_allocation_inquiry.php?",'SA_SALESALLOC', MENU_INQUIRY);


		if (get_company_pref_display('sales_reports'))
			$this->add_rapp_function(1,_(get_company_pref_display('sales_reports_text')),
				"reporting/reports_main.php?Class=0",'SA_SALESTRANSVIEW', MENU_REPORT);

		$this->add_module(_("Maintenance"));
		if (get_company_pref_display('manage_customer'))
			$this->add_lapp_function(2,_(get_company_pref_display('manage_customer_text')),
				"sales/manage/customers_inquiry.php?",'SA_CUSTOMER', MENU_ENTRY);

		if (get_company_pref_display('customer_branches'))
			$this->add_lapp_function(2,_(get_company_pref_display('customer_branches_text')),
				"sales/manage/customer_branches.php?",'SA_CUSTOMER', MENU_ENTRY);


		if (get_company_pref_display('sales_group'))
			$this->add_lapp_function(2,_(get_company_pref_display('sales_group_text')),
				"sales/manage/sales_groups.php?",'SA_SALESGROUP', MENU_MAINTENANCE);
		//$this->add_lapp_function(2, _("Recurrent &Invoices"),
		//	"sales/manage/recurrent_invoices.php?",'SA_SRECURRENT', MENU_MAINTENANCE);


		if (get_company_pref_display('sales_type'))
			$this->add_rapp_function(2,_(get_company_pref_display('sales_type_text')),
				"sales/manage/sales_types.php?",'SA_SALESTYPES', MENU_MAINTENANCE);


		if (get_company_pref_display('sales_persons'))
			$this->add_rapp_function(2,_(get_company_pref_display('sales_persons_text')),
				"sales/manage/sales_people.php?",'SA_SALESMAN', MENU_MAINTENANCE);


		if (get_company_pref_display('sales_areas'))
			$this->add_rapp_function(2,_(get_company_pref_display('sales_areas_text')),
				"sales/manage/sales_areas.php?",'SA_SALESAREA', MENU_MAINTENANCE);


// 		if (get_company_pref_display('wht_types'))
// 			$this->add_rapp_function(2,_(get_company_pref_display('wht_types_text')),
// 				"sales/manage/wht_type.php?",'SA_SALESAREA', MENU_MAINTENANCE);
		//$this->add_rapp_function(2, _("Credit &Status Setup"),
		//"sales/manage/credit_status.php?",'SA_CRSTATUS', MENU_MAINTENANCE);

		$this->add_extensions();
	}
}


