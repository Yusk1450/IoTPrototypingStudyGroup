<?php

require_once('db.php');

// 読み込み（Read）
$sql = "SELECT * FROM Seats";
$res = $pdo->query($sql);

$ary = array();
foreach($res as $row)
{
	$data = array();
	$data['index'] = $row['seats_index'];
	$data['on'] = $row['seats_on'];
	$ary []= $data;
}

echo json_encode($ary);