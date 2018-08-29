<?php

require_once('db.php');

$seat_id = $_GET['seat_id'];
$on = $_GET['on'];

// 更新（Update）
$sql = "UPDATE Seats SET seats_on = $on WHERE seats_index = $seat_id";
$stmt = $pdo->prepare($sql);
$stmt->execute();
