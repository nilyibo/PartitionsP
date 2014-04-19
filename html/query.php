<?php
	require("./credentials.php");
	$con = mysqli_connect($mysql_hostname, $mysql_username, $mysql_password, $mysql_database);

	$param_l = $_GET["l"];
	$param_nmin = $_GET["nmin"];
	$param_nmax = $_GET["nmax"];
	if ($param_l == 2)
		$BITS = 32;
	elseif ($param_l == 3 || $param_l == 4)
		$BITS = 16;
	else // 5 or 6
		$BITS = 10;
	$param_nmin = $param_nmin - $param_nmin % $BITS;

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