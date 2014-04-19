<?php
	$mysql_hostname = "";
	$mysql_username = "";
	$mysql_password = "";
	$mysql_database = "";

	$con = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mysql_database);
	$query = ";";

	$result = mysqli_query($con, $query);

	if (!$result)
	{
		echo "Error";
		return;
	}

	echo "[";
	while($row = mysqli_fetch_array($result))
	{
		echo json_encode($row) . ",";
	}
	echo "null]";

	mysqli_close($con);
?>