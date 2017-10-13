<?php

class reporting_app extends application
{
	function reporting_app()
	{
		$this->application("reporting", _($this->help_context = "&Reporting"));

		{
		    if (get_company_pref_display('sales_reports'))
			$this->add_rapp_function(1,_(get_company_pref_display('sales_reports_text')),
				"reporting/reports_main.php?Class=0",'SA_SALESTRANSVIEW', MENU_REPORT);
            if (get_company_pref_display('supplier_reports'))
	    	$this->add_rapp_function(1, _(get_company_pref_display('supplier_reports_text')),
			"reporting/reports_main.php?Class=1",'SA_SUPPTRANSVIEW', MENU_REPORT);	
			if (get_company_pref_display('inventory_reports'))
			$this->add_rapp_function(1, _(get_company_pref_display('inventory_reports_text')),
				"reporting/reports_main.php?Class=2", 'SA_ITEMSTRANSVIEW', MENU_REPORT);
			$this->add_rapp_function(1, _("Manufacturing &Reports"),
			"reporting/reports_main.php?Class=3", 'SA_MANUFTRANSVIEW', MENU_REPORT);
			$this->add_rapp_function(1, _("Fixed Assets &Reports"),
			"reporting/reports_main.php?Class=7", 'SA_ASSETSANALYTIC', MENU_REPORT);
			$this->add_rapp_function(1, _("Dimension &Reports"),
				"reporting/reports_main.php?Class=4", 'SA_DIMENSIONREP', MENU_REPORT);
           if (get_company_pref_display('banking_reports'))
			$this->add_rapp_function(1, _(get_company_pref_display('banking_reports_text')),
				"reporting/reports_main.php?Class=5", 'SA_BANKREP', MENU_REPORT);
			if (get_company_pref_display('general_ledger'))
			$this->add_rapp_function(1, _(get_company_pref_display('general_ledger_text')),
				"reporting/reports_main.php?Class=6", 'SA_GLREP', MENU_REPORT);
			
			$this->add_extensions();
		}
	}
}

