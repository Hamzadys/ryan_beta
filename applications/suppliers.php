<?php

class suppliers_app extends application 
{
	function suppliers_app() 
	{
		$this->application("AP", _($this->help_context ="&Purchases"));

		$this->add_module(_("Transactions"));
		$this->add_lapp_function(0, _('Requisitions Entries'), $path_to_root.'/modules/requisitions/requisitions.php',
					'SA_REQUISITIONS',	MENU_TRANSACTION);
		$this->add_lapp_function(0, _('Requisitions Allocation'), $path_to_root.'/modules/requisitions/requisition_allocations.php',
					 'SA_REQUISITION_ALLOCATIONS', MENU_TRANSACTION);
					 
		if (get_company_pref_display('purchase_order'))
		$this->add_lapp_function(0, _(get_company_pref_display('purchase_order_text')),
			"purchasing/po_entry_items.php?NewOrder=Yes",'SA_PURCHASEORDER', MENU_TRANSACTION);

		if (get_company_pref_display('opom'))
		$this->add_lapp_function(0, _(get_company_pref_display('opom_text')),
			"purchasing/inquiry/po_search.php?",'SA_GRN', MENU_TRANSACTION);
	//	$this->add_lapp_function(0, _("Direct &GRN"),
		//	"purchasing/po_entry_items.php?NewGRN=Yes", 'SA_GRN', MENU_TRANSACTION);

	//	if (get_company_pref_display('direct_supplier_invoice'))
            $this->add_lapp_function(0, _("Local Purchase"),
                "purchasing/supplier_invoice.php?New=1", 'SA_SUPPLIERINVOICE', MENU_TRANSACTION);
        $this->add_lapp_function(0, _("Import Purchase"),
            "purchasing/supplier_invoice_import_reg.php?New=1", 'SA_SUPPLIERINVOICE', MENU_TRANSACTION);
if (get_company_pref_display('direct_supplier_invoice'))
		$this->add_lapp_function(0, _(get_company_pref_display('direct_supplier_invoice_text')),
			"purchasing/po_entry_items.php?NewInvoice=Yes",'SA_SUPPLIERINVOICE', MENU_TRANSACTION);
       // $this->add_lapp_function(0, _(get_company_pref_display('direct_supplier_invoice_text')),
		//	"purchasing/po_entry_items.php?NewInvoice=Yes",'SA_SUPPLIERINVOICE', MENU_TRANSACTION);

		if (get_company_pref_display('payments_suppliers'))
		$this->add_rapp_function(0, _(get_company_pref_display('payments_suppliers_text')),
			"purchasing/supplier_payment.php?",'SA_SUPPLIERPAYMNT', MENU_TRANSACTION);
		$this->add_rapp_function(0, "","");

		if (get_company_pref_display('supplier_invoice'))
		$this->add_rapp_function(0, _(get_company_pref_display('supplier_invoice_text')),
			"purchasing/supplier_invoice.php?New=1",'SA_SUPPLIERINVOICE', MENU_TRANSACTION);

		if (get_company_pref_display('supplier_credit'))
		$this->add_rapp_function(0, _(get_company_pref_display('supplier_credit_text')),
			"purchasing/supplier_credit.php?New=1",'SA_SUPPLIERCREDIT', MENU_TRANSACTION);

		if (get_company_pref_display('allocates'))
		$this->add_rapp_function(0, _(get_company_pref_display('allocates_text')),
			"purchasing/allocations/supplier_allocation_main.php?",'SA_SUPPLIERALLOC', MENU_TRANSACTION);

		$this->add_module(_("Inquiries and Reports"));

		if (get_company_pref_display('purchase_order_inquiry'))
		$this->add_lapp_function(1, _(get_company_pref_display('purchase_order_inquiry_text')),
			"purchasing/inquiry/po_search_completed.php?",'SA_SUPPTRANSVIEW', MENU_INQUIRY);
	
		$this->add_lapp_function(1, _("Purchase Requisition Inquiry"),
			"purchasing/inquiry/pr_search_completed.php?", 'SA_SUPPTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('supplier_transaction_inquiry'))
		$this->add_lapp_function(1, _(get_company_pref_display('supplier_transaction_inquiry_text')),
			"purchasing/inquiry/supplier_inquiry.php?",'SA_SUPPTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('allocate_inquiry'))
		$this->add_lapp_function(1, _(get_company_pref_display('allocate_inquiry_text')),
			"purchasing/inquiry/supplier_allocation_inquiry.php?",'SA_SUPPLIERALLOC', MENU_INQUIRY);

		if (get_company_pref_display('supplier_reports'))
		$this->add_rapp_function(1, _(get_company_pref_display('supplier_reports_text')),
			"reporting/reports_main.php?Class=1",'SA_SUPPTRANSVIEW', MENU_REPORT);

		$this->add_module(_("Maintenance"));


       // $this->add_lapp_function(2, _('Combo 1'),
		//	"purchasing/manage/combo_1.php?",'SA_SUPPLIER', MENU_ENTRY);

	//	$this->add_lapp_function(2, _('Combo 2'),
		//	"purchasing/manage/combo_2.php?",'SA_SUPPLIER', MENU_ENTRY);

	//	$this->add_lapp_function(2, _('Combo 3'),
			//"purchasing/manage/combo_3.php?",'SA_SUPPLIER', MENU_ENTRY);


//		if (get_company_pref_display('suppliers'))
			$this->add_lapp_function(2, _('Add And Manage Supplier'),
				"purchasing/manage/suppliers_inquiry.php?",'SA_SUPPLIER', MENU_ENTRY);

		$this->add_extensions();
	}
}


