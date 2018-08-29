
/*---------------------------------------------------------
 * エントリポイント
---------------------------------------------------------*/
$(function()
{
	// 各席の状態を更新する
	updateSittings();

	// 0.5秒おきに各席の状態を更新する
	setInterval(function()
	{
		updateSittings();
	}, 500);
});

/*---------------------------------------------------------
 * 各席の状態を更新する
---------------------------------------------------------*/
function updateSittings()
{
	var data = loadJsonFromPHP('./db/get.php');

	$.each(data, function(key, val)
	{
		var seat_idx = val['index'];
		var is_sitting = val['on'];

		var $seat = $('div.seat_idx_'+seat_idx);

		if (is_sitting == 0)
		{
			$seat.css('background-color', '#fff');
			$seat.css('border', '1px solid #ccc');
		}
		else
		{
			$seat.css('background-color', '#ee0000');
			$seat.css('border', '1px solid #ee0000');
		}
	});
}

/*---------------------------------------------------------
 * ajaxを利用してJSONデータを取得する
---------------------------------------------------------*/
function loadJsonFromPHP(phpname)
{
	var json = null;
	$.ajax(
	{
		url       : phpname
		,type     : 'GET'
		,async    : false
		,dataType : 'json'
		,cache    : false
		,success  : function(data)
		{
			json = data;
		}
	});
	return json;
}