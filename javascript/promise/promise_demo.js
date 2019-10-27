
$(function()
{
	//alert('document loaded');
	//longOperation(2);
	hello_promise();
});

function sleep(ms) 
{
	return new Promise(resolve => setTimeout(resolve, ms));
}

async function longOperation(secs)
{
	console.log('longOperation entry');
	//setTimeout(function(){ console.log('Timed out');}, 3000);
	await sleep(2000);
	$("#label").html('long operation finished');
	console.log('longOperation exit');
}


function hello_promise()
{
	var promise = new Promise(function(resolve, reject)
	{
		var ok = true;
		if (ok)
		{
			resolve('resolv ret value');
		} else {
			reject('reject with err');
		}
	});

	promise.then(function(value){
		console.log('resolved func' + value);
	}, function(error) {
		console.log('rejected ' + error);
	});
		
}
