function focus_alloc(i) {
	save_focus(i);
	i.setAttribute('_last', get_amount(i.name));
}

function blur_alloc(i)
{
	var change=get_amount(i.name);
	if
	(i.name!='amount'&&i.name!='charge'&&i.name!='discount'&&i.name!='charge'&&i.name!='gst_wh'
		&&i.name!='wht_supply_amt'&&i.name!='wht_service_amt'&&i.name!='wht_fbr_amt'&&i.name!='wht_srb_amt'&&i.name!='advance_payment'


	)
		change=Math.min(change,get_amount('maxval'+i.name.substr(6),1))
	price_format(i.name,change,user.pdec);

	if
	(i.name!='amount'&&i.name!='charge')
	{
		if(change<0)
			change=0;
		change=change-i.getAttribute('_last');

		if(i.name=='discount')
			change=-change;

		if(i.name=='gst_wh')
			change=-change;
		//
		// if(i.name=='wht_supply_amt')
		//     change=-change;
		// if(i.name=='wht_service_amt')
		//     change=-change;
		if(i.name=='wht_fbr_amt')
			change=-change;

		if(i.name=='wht_srb_amt')
			change=-change;

		var total=get_amount('amount') + change ;
		price_format('amount',total,user.pdec,0);

		var total1=get_amount('amount') ;
		price_format('amount_new',total1   ,user.pdec,0);


		var wht_supply_amt1 = total1 - ( get_amount('wht_supply_amt')) ;
		var wht_service_amt1 = wht_supply_amt1 - ( get_amount('wht_service_amt')) ;
		// var wht_fbr_amt1 = wht_service_amt1 - ( get_amount('wht_fbr_amt')) ;
		// var wht_srb_amt1 = wht_fbr_amt1 - ( get_amount('wht_srb_amt')) ;


		price_format('amount_new',wht_service_amt1 ,user.pdec,0);




	}


}

function allocate_all(doc)
{




	var amount=get_amount('amount'+doc);
	var unallocated=get_amount('un_allocated'+doc);
	var discount1=get_amount('discount');
	var gst_wh1=get_amount('gst_wh');
	var wht_supply_amt1=get_amount('wht_supply_amt');
	var wht_service_amt1=get_amount('wht_service_amt');
	var wht_fbr_amt1=get_amount('wht_fbr_amt');
	var wht_srb_amt1=get_amount('wht_srb_amt');
	// var wht_supply_tax1=get_amount('wht_supply_tax');
	var total=get_amount('amount');


	var wht_supply_tax1=get_amount('wht_supply_tax');
	if(wht_supply_tax1 == 11)
		var percentage = 12 / 100;
	var discounted_value=percentage * total;

	var left=0;
	wht_supply_amt1-=( (  amount ));
	wht_service_amt1-=( (amount));
	wht_fbr_amt1-=( (amount));
	wht_srb_amt1-=( (amount));
	discount1-=( (amount));
	gst_wh1-=( (amount));
	total-=(amount-unallocated);

	left-=(amount-unallocated - discounted_value);
	amount=unallocated;


	if(left<0)
	{

		wht_supply_amt1+=left;
		wht_service_amt1+=left;
		wht_fbr_amt1+=left;
		wht_srb_amt1+=left;
		discount1+=left;
		gst_wh1+=left;
		total+=left;
		amount+=left;
		left=0;
	}

	price_format('amount'+doc,amount,user.pdec);
	price_format('wht_supply_amt',discounted_value ,user.pdec);
	price_format('wht_service_amt',wht_service_amt1,user.pdec);
	price_format('wht_fbr_amt',wht_fbr_amt1,user.pdec);
	price_format('wht_srb_amt',wht_srb_amt1,user.pdec);
	price_format('discount',discount1,user.pdec);
	price_format('gst_wh',gst_wh1,user.pdec);
	price_format('amount',total-discount1-gst_wh1 - wht_srb_amt1 - wht_fbr_amt1 ,user.pdec);


}

function allocate_none(doc) {
	amount = get_amount('amount'+doc);
	total = get_amount('amount');
	price_format('amount'+doc, 0, user.pdec);
	price_format('amount', total-amount, user.pdec);
}


var allocations = {
	'.amount': function(e) {
		e.onblur = function() {
			blur_alloc(this);
		};

		e.onfocus = function() {
			focus_alloc(this);
		};
	}



}

var allocations1 = {
	'amount': function(e) {
		e.onblur = function() {
			blur_alloc(this);
		};

		e.onfocus = function() {
			focus_alloc(this);
		};
	}



}


Behaviour.register(allocations);
Behaviour.register(allocations1);
