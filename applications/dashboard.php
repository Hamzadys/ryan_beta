<?php

class dashboard_app extends application 
{
	function dashboard_app() 
	{
		$this->application("dashboard", _($this->help_context = "Dash&Board"));
	
//		$this->add_module(_("My Dashboard"));
//		$this->add_lapp_function(1, _("My &Dashboard"),
//			"modules/dashboard/dashboard.php", 'SS_DASHBOARD', MENU_INQUIRY);
	
		$this->add_extensions();
	}
}


?>