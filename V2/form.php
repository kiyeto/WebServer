<?php

function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}

$name = $lname = $login = "";

if ($_SERVER["REQUEST_METHOD"] == "POST")
{
	$name = test_input($_POST["fname"]);
	$email = test_input($_POST["login"]);
}


echo "<h2>Your Input:</h2>";
echo $name;
echo "<br>";
echo $lname;
echo "<br>";
echo $login;

?>