<?php
$path_to_root="././.";
include_once($path_to_root . "/includes/session.inc");
include_once($path_to_root . "/includes/date_functions.inc");
include($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/modules/dashboard/charts/charts_utils.php");
	
class dashboard 
{
    
    public function renderDash()
    {
        $path_to_root = ".";
     echo "<section class='content'>";
     
         echo"<div class='row'>"; 
           
                
	$today = Today();
	 $begin = begin_fiscalyear();
	$begin1 = date2sql($begin);
	$today1 = date2sql($today);
	$sql = "SELECT SUM((ov_amount + ov_discount) * rate*IF(trans.type = ".ST_CUSTCREDIT.", -1, 1)) AS sales,d.debtor_no, d.name 
	FROM ".TB_PREF."debtor_trans AS trans, ".TB_PREF."debtors_master AS d 
	WHERE trans.debtor_no=d.debtor_no
		AND (trans.type = ".ST_SALESINVOICE." OR trans.type = ".ST_CUSTCREDIT.")
		AND tran_date = '$today1'";
	$salesresult = db_query($sql);
   	$salesmyrow = db_fetch($salesresult);
	//total return
	$sql = "SELECT SUM((ov_amount + ov_discount) * rate*IF(trans.type = ".ST_CUSTCREDIT.", -1, 1)) AS salesreturn,d.debtor_no, d.name 
	FROM ".TB_PREF."debtor_trans AS trans, ".TB_PREF."debtors_master AS d 
	WHERE trans.debtor_no=d.debtor_no
		AND trans.type = ".ST_CUSTCREDIT."
		AND tran_date = '$today1'";
	$salesreturnresult = db_query($sql);
   	$salesreturnmyrow = db_fetch($salesreturnresult);	

	$sql = "SELECT SUM((ov_amount) * rate) AS recovery
	FROM ".TB_PREF."debtor_trans AS trans, ".TB_PREF."debtors_master AS d 
	WHERE trans.debtor_no=d.debtor_no
		AND (trans.type = ".ST_BANKDEPOSIT." OR trans.type = ".ST_CUSTPAYMENT.")
		AND tran_date = '$today1'";
	$recoveryresult = db_query($sql);
	$recoverymyrow = db_fetch($recoveryresult);

	$sql = "SELECT SUM((ov_amount) * rate) AS payments
	FROM ".TB_PREF."supp_trans AS strans, ".TB_PREF."suppliers AS s
	WHERE strans.supplier_id=s.supplier_id
		AND (strans.type = ".ST_BANKPAYMENT." OR strans.type = ".ST_SUPPAYMENT.")
		AND strans.tran_date = '$today1'";
	$paymentsresult = db_query($sql);
   	$paymentsmyrow = db_fetch($paymentsresult);		


	$sql = "SELECT SUM(total) AS sorders
	FROM ".TB_PREF."sales_orders AS so, ".TB_PREF."debtors_master AS d
	WHERE so.debtor_no=d.debtor_no
		AND so.reference != 'auto'
		AND so.ord_date = '$today1'";		
	$sresult = db_query($sql);
   	$smyrow = db_fetch($sresult);		


	$sql = "SELECT SUM(total) AS porders
	FROM ".TB_PREF."purch_orders AS po, ".TB_PREF."suppliers AS s
	WHERE po.supplier_id=s.supplier_id
		AND po.reference != 'auto'	
		AND po.ord_date = '$today1'";
	$poresult = db_query($sql);
   	$pomyrow = db_fetch($poresult);	
	$sales = $salesmyrow['sales']; 
							if($sales > 0)
						{	
							$sales1 = $sales; 
								}
							else
						{ 
						  	 $sales1 = 0; 
							 	}
								//total return
								$salesreturn = $salesreturnmyrow['salesreturn']; 
							if($salesreturn > 0)
						{	
							$salesreturn1 = $salesreturn; 
								}
							else
						{ 
						  	 $salesreturn1 = 0; 
							 	}
								$recovery = $recoverymyrow['recovery']; 
							if ($recovery > 0)
						{
							$recovery1 = $recovery;
							}
						else
						{   
							$recovery1 = 0; 
							}
							 $payments = $paymentsmyrow['payments']; 
						   			
						if($payments > 0)
						{
							$payments1 = $payments;
							}
						else
						{  $payments1 = 0; 
							}
							 $sorders = $smyrow['sorders']; 
						 if($sorders > 0)
						 {
							 $sorders1 = $sorders;
						   			}
						else
						{  $sorders1 = 0; 
							}
							 $porders = $pomyrow['porders']; 
						 if($porders > 0)  			
						 {
							 $porders1 = $porders;
						 	}
						else
						{  $porders1 = 0; 
							}
                            
            echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
			 echo"<div class='info-box'>";  
                echo"<span class='info-box-icon bg-aqua'>
						<i class='fa fa-line-chart' style='margin-top:20px' ></i>
						</span>";
                  echo"<div class='info-box-content'>";
                     echo"<span class='info-box-text'>Today's Sales</span>";
                     echo"<span class='info-box-number'>".number_format($sales1)."<small></small></span>";
                  echo"</div>";//<!-- /.info-box-content -->  
              echo"</div>";//<!-- /.info-box -->      
            echo"</div>";//<!-- /.col -->    
			
             
			 echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
			 echo"<div class='info-box'>";  
                echo"<span class='info-box-icon bg-green'>
						<i class='ion ion-android-sync' style='margin-top:20px' ></i>
						</span>";
                  echo"<div class='info-box-content'>";
                     echo"<span class='info-box-text'>Today's Recovery</span>";
                     echo"<span class='info-box-number'>".number_format($recovery1)."<small></small></span>";
                  echo"</div>";//<!-- /.info-box-content -->  
              echo"</div>";//<!-- /.info-box -->      
            echo"</div>";//<!-- /.col --> 
			echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
			 echo"<div class='info-box'>";  
                echo"<span class='info-box-icon bg-yellow'>
						<i class='ion ion-ios-calculator' style='margin-top:20px' ></i>
						</span>";
                  echo"<div class='info-box-content'>";
                     echo"<span class='info-box-text'>Today's Payments</span>";
                     echo"<span class='info-box-number'>".number_format($payments1)."<small></small></span>";
                  echo"</div>";//<!-- /.info-box-content -->  
              echo"</div>";//<!-- /.info-box -->      
            echo"</div>";//<!-- /.col --> 
			 
            echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
              echo"<div class='info-box'>";
                  echo"<span class='info-box-icon bg-orange'><i class='fa fa-bar-chart'  style='margin-top:20px'></i></span>";
                   echo"<div class='info-box-content'>";
                     echo"<span class='info-box-text'>Today's Sale Order</span>";
                     echo"<span class='info-box-number'>".number_format($sorders1)."</span>";
                   echo"</div>";//<!-- /.info-box-content -->
               echo"</div>";//<!-- /.info-box -->
             echo"</div>";//<!-- /.col -->
            //<!-- fix for small devices only -->
           echo"<div class='clearfix visible-sm-block'></div>";
             echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
              echo"<div class='info-box'>";
                echo"<span class='info-box-icon bg-blue'><i class='fa fa-shopping-cart'  style='margin-top:20px'></i></span>";
                echo"<div class='info-box-content'>";
                  echo"<span class='info-box-text'>Today's Purchase Order</span>";
                  echo"<span class='info-box-number'>".number_format($porders1)."</span>";
                echo"</div>";//<!-- /.info-box-content -->
              echo"</div>";//<!-- /.info-box -->
           echo"</div>";//<!-- /.col -->
           
            echo"<div class='col-md-4 col-sm-6 col-xs-12'>";
              echo"<div class='info-box'>";
                echo"<span class='info-box-icon bg-red'><i class='fa fa-exchange'  style='margin-top:20px'></i></span>";
                echo"<div class='info-box-content'>";
                  echo"<span class='info-box-text'>Today's Returns</span>";
                  echo"<span class='info-box-number'>".number_format($salesreturn1)."</span>";
                echo"</div>";//<!-- /.info-box-content -->
              echo"</div>";//<!-- /.info-box -->
            echo"</div>";//<!-- /.col -->
         
                             
         echo"</div>";//row
         
//echo"<div class='row'>"; /************ Chart Row starts here **/
//  echo"<div class='col-md-12 '>";
      
//    include_once("$path_to_root/themes/".user_theme()."/Dashboard_Widgets/IncomeandExpences.php");                    
//                            $_IE = new IncomeAndExpences();
                            
//                            $_IE->IEwidget();
//  echo"</div>";//<!-- /.col -->                    
//echo"</div>";//chart row ends here

 /*************************Mid-table Finantial Ratios*****/
 
// include_once("$path_to_root/themes/".user_theme()."/Dashboard_Widgets/GlobalFinancialRatios.php");                    
//                            $_Donuts = new GlobalFinancialRatios();
                            
 //                           $_Donuts->RatiosTable();
                            
 /*************************Mid-table ends*****/
 
 //********************************************* Donuts charts **************************/
echo"<div class='row hidden-xs' >";

echo'
        <div class="col-xs-12">
          <div class="">
           
            <!-- /.box-header -->
            <div class="">
              <table class="col-md-12 col-sx-12">
              <tr><td >
';
 include_once("$path_to_root/themes/".user_theme()."/Dashboard_Widgets/tabspanel.php");                    
                            $_tab = new tabs();
                            
                            $_tab->Alltabs();
/* include_once("$path_to_root/themes/grayblue/Dashboard_Widgets/Donutcharts.php");                    
                            $_Donuts = new Donuts();
                            
                            $_Donuts->DonutsCharts();*/
                   echo'</td></tr></table>';       
                            
echo'  </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->
      </div>';

echo"</div>";///**********************************************//
 echo"<div>&nbsp;   </div>";

echo"<div class='row hidden-md hidden-sm hidden-lg'>";

    echo'
    <div class="col-xs-12">
          <div class="box">
            <div class="box-header">
              <h3 class="box-title">Donust charts</h3>
            </div>
            <!-- /.box-header -->
            <div class="box-body table-responsive no-padding">
              <table class="table">
                <tbody><tr>
                  <th style="border-right:1px solid #CCC;">Client</th>
                  <th style="border-right:1px solid #CCC;">Client Balances</th>
                  <th style="border-right:1px solid #CCC;">Client Profitability</th>
                  <th style="border-right:1px solid #CCC;">Salesman Balances</th>
                  <th style="border-right:1px solid #CCC;">Top 10 Zones</th>
                  <th style="border-right:1px solid #CCC;">Salesmen</th>
                  <th style="border-right:1px solid #CCC;">Zone Balances</th>
                  <th style="border-right:1px solid #CCC;">Supplier Balances</th>
                  <th style="border-right:1px solid #CCC;">Top 10 Suppliers</th>
                  <th style="border-right:1px solid #CCC;">Top 10 Sold items</th>
                  <th style="border-right:1px solid #CCC;">Items Profitability</th>
                  <th style="border-right:1px solid #CCC;">To 10 Bank Position</th>
                  <th style="border-right:1px solid #CCC;">To 10 Cost Centres</th>

                </tr>
                <tr>
                  <td style="border-right:1px solid #CCC;">';
       include_once("$path_to_root/themes/".user_theme()."/Dashboard_Widgets/MBcharts.php");                    
                            $_M = new MBcahrts();   
                            $_M->Customer();   
                  
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                  
                            $_M->CustomerBalances();
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Customerprofibility();    
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->salesbalances();
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->tenzone(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10Salesmen(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Zonbalances(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->SupplierBalances(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10Suppliers(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10SoldItems(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10ItemsProfitability(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10BankPosition(); 
                  echo'</td>
                  <td style="border-right:1px solid #CCC;">';
                           $_M->Top10CostCentres(); 
                  echo'</td>
                  
                </tr>
               </tbody></table>
            </div>
            <!-- /.box-body -->
          </div>
          <!-- /.box -->
        </div>
    ';

echo"</div>";

 echo"<div>&nbsp;   </div>";

 echo"<div class='row'>"; /** row starts **/
 
 echo"<div class='col-md-12'>";
 
  include_once("$path_to_root/themes/".user_theme()."/Dashboard_Widgets/invoicesWidget.php");                    
                            $_Invoices = new invoiceswidgets();
                            
                            $_Invoices->AllInvoices();
                            
echo"</div>";
 echo"</div>"; /** row ends here */
 
 /////////////////////////////////////data model 
          echo'
<!-- Modal -->
  <div class="modal fade" id="OverduePurchaseInvoices" role="dialog">';
   
include_once("$path_to_root/themes/".user_theme()."/InvoicesModals.php");                    
                            $_invoices = new InvoicesModals();
                            
                            $_invoices->OverduePurchaseInvoices();
 echo'</div>';

//**********************************
  echo'
<!-- Modal -->
  <div class="modal fade" id="OverdueSalesInvoices" role="dialog">';
   
include_once("$path_to_root/themes/".user_theme()."/InvoicesModals.php");                    
                            $_invoices = new InvoicesModals();
                            
                            $_invoices->OverdueSalesInvoices();
 echo'</div>';

//**********************************

  echo'
<!-- Modal -->
  <div class="modal fade" id="RecentSalesInvoices" role="dialog">';
   
include_once("$path_to_root/themes/".user_theme()."/InvoicesModals.php");                    
                            $_invoices = new InvoicesModals();
                            
                            $_invoices->RecentSalesInvoices();
 echo'</div>';
 
//**********************************
 
 echo'
<!-- Modal -->
  <div class="modal fade" id="RecentSaleOrder" role="dialog">';
   
include_once("$path_to_root/themes/".user_theme()."/InvoicesModals.php");                    
                            $_invoices = new InvoicesModals();
                            $_invoices->RecentSaleOrder();
 echo'</div>';

       echo"</section>";
        echo"<div class='row'><section>";
       
       
        
      echo"</section></div>";
       }
}                      
?>