<?php

class inventory_app extends application
{
	function inventory_app()
	{
		$this->application("stock", _($this->help_context = "&Items and Inventory"));

		$this->add_module(_("Transactions"));
		if (get_company_pref_display('inventry_location'))
			$this->add_lapp_function(0, _(get_company_pref_display('inventry_location_text')),
				"inventory/transfers.php?NewTransfer=1", 'SA_LOCATIONTRANSFER', MENU_TRANSACTION);

		if (get_company_pref_display('inventory_adjustments'))
			$this->add_lapp_function(0, _(get_company_pref_display('inventory_adjustments_text')),
				"inventory/adjustments.php?NewAdjustment=1", 'SA_INVENTORYADJUSTMENT', MENU_TRANSACTION);



		$this->add_module(_("Inquiries and Reports"));
		if (get_company_pref_display('inventory_item_movemnets'))
			$this->add_lapp_function(1, _(get_company_pref_display('inventory_item_movemnets_text')),
				"inventory/inquiry/stock_movements.php?", 'SA_ITEMSTRANSVIEW', MENU_INQUIRY);

		if (get_company_pref_display('inventory_item_status'))
			$this->add_lapp_function(1, _(get_company_pref_display('inventory_item_status_text')),
				"inventory/inquiry/stock_status.php?", 'SA_ITEMSSTATVIEW', MENU_INQUIRY);
$this->add_lapp_function(1, _("Daily Inventory Movements"),
            "inventory/inquiry/daily_stock_movements.php?", 'SA_ITEMSSTATVIEW', MENU_INQUIRY);

		if (get_company_pref_display('inventory_reports'))
			$this->add_rapp_function(1, _(get_company_pref_display('inventory_reports_text')),
				"reporting/reports_main.php?Class=2", 'SA_ITEMSTRANSVIEW', MENU_REPORT);

		$this->add_module(_("Maintenance"));

//		if (get_company_pref_display('items'))
			$this->add_lapp_function(2, _(('Add And Manage Items')),
				"inventory/manage/items_inquiry.php?", 'SA_ITEM', MENU_ENTRY);
				
					$this->add_lapp_function(2, _('items Pref'),
				"admin/item_pref.php?", 'SA_ITEM', MENU_ENTRY);


		$this->add_lapp_function(2, _('Header/Footer Pref'),
			"admin/hf_pref.php?", 'SA_ITEM', MENU_ENTRY);


		if (get_company_pref_display('foreign_item_codes'))
			$this->add_lapp_function(2, _(get_company_pref_display('foreign_item_codes_text')),
				"inventory/manage/item_codes.php?", 'SA_FORITEMCODE', MENU_MAINTENANCE);

		if (get_company_pref_display('sales_kits'))
			$this->add_lapp_function(2, _(get_company_pref_display('sales_kits_text')),
				"inventory/manage/sales_kits.php?", 'SA_SALESKIT', MENU_MAINTENANCE);

		if (get_company_pref_display('item_categories'))
			$this->add_lapp_function(2, _(get_company_pref_display('item_categories_text')),
				"inventory/manage/item_categories.php?", 'SA_ITEMCATEGORY', MENU_MAINTENANCE);

		if (get_company_pref_display('inventory_locations'))
			$this->add_rapp_function(2, _(get_company_pref_display('inventory_locations_text')),
				"inventory/manage/locations.php?", 'SA_INVENTORYLOCATION', MENU_MAINTENANCE);


		if (get_company_pref_display('unit_measure'))
			$this->add_rapp_function(2, _(get_company_pref_display('unit_measure_text')),
				"inventory/manage/item_units.php?", 'SA_UOM', MENU_MAINTENANCE);

		if (get_company_pref_display('recorder_levels'))
			$this->add_rapp_function(2, _(get_company_pref_display('recorder_levels_text')),
				"inventory/reorder_level.php?", 'SA_REORDER', MENU_MAINTENANCE);

		$this->add_module(_("Pricing and Costs"));

		if (get_company_pref_display('sales_pricing'))
			$this->add_lapp_function(3, _(get_company_pref_display('sales_pricing_text')),
				"inventory/prices.php?", 'SA_SALESPRICE', MENU_MAINTENANCE);

		if (get_company_pref_display('purchase_pricing'))
			$this->add_lapp_function(3, _(get_company_pref_display('purchase_pricing_text')),
				"inventory/purchasing_data.php?", 'SA_PURCHASEPRICING', MENU_MAINTENANCE);

		if (get_company_pref_display('standard_cost'))
			$this->add_rapp_function(3, _(get_company_pref_display('standard_cost_text')),
				"inventory/cost_update.php?", 'SA_STANDARDCOST', MENU_MAINTENANCE);

		$this->add_extensions();
	}
}


