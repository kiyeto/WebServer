<?php
$user = $_POST["username"];
if($user != null)
{
    echo $user;
    echo " is your username";
}
else
{
    echo "no username supplied";
}
?>