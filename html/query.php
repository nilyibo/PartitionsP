<?php
	$mysql_hostname = "";
	$mysql_username = "";
	$mysql_password = "";
	$mysql_database = "";

	$con = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mysql_database);

	$param_l = $_GET["l"];
	$param_nmin = $_GET["nmin"];
	$param_nmax = $_GET["nmax"];
	$param_nmin = $param_nmin - $param_nmin % 32;

	if ($param_l == 2)
		$query = "SELECT * FROM `p_n_mod2` WHERE `n`>=$param_nmin AND `n`<=$param_nmax ORDER BY `n`;";
	elseif ($param_l == 3)
		$query = "SELECT * FROM `p_n_mod3` WHERE `n`>=$param_nmin AND `n`<=$param_nmax ORDER BY `n`;";
	elseif ($param_l == 4)
		$query = "SELECT * FROM `p_n_mod4` WHERE `n`>=$param_nmin AND `n`<=$param_nmax ORDER BY `n`;";
	else
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