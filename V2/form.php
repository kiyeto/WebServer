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
	if (isset($_POST['fname'])) {
		$name = test_input($_POST['fname']);
	 }
	 if (isset($_POST['lname'])) {
		 $lname = test_input($_POST['lname']);
	}
	 if (isset($_POST['login'])) {
		 $login = test_input($_POST['login']);
		// do something
	 }
}


echo "<h2>Your Input:</h2>";
// echo $_POST['fname'];
echo $name;
echo "<br>";
echo $lname;
echo "<br>";
echo $login;

?>