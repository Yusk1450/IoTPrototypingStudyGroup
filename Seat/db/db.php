<?php

$database_address   = "localhost:8889";		// データベースアドレス
$database_name      = "arduino_ws0401";		// データベース名
$database_username  = "root";				// データベースユーザ名
$database_password  = "root";				// データベースパスワード

$pdo = new PDO("mysql:host=".$database_address.";dbname=".$database_name, $database_username, $database_password);