<?php
	if(isset($_POST['submit']) && isset($_POST['submit']) != '')
	{
		require_once 'Db_Functions.php';

		$dbf = new Db_Functions();

		$safeauthor = $dbf->escapeString($_POST['fname']);
		$safename = $dbf->escapeString($_POST['frecipename']);
		$safeingredients = $dbf->escapeString($_POST['fingredients']);
		$safedirections = $dbf->escapeString($_POST['fdirections']);
		$safedifficulty = $dbf->escapeString($_POST['fdifficulty']);
		$safephoto = $dbf->escapeString($_POST['fphoto']);

		if($dbf->storeRecipe($safeauthor, $safename, $safeingredients, $safedirections, $safedifficulty, $safephoto))
		{
			header('Location: http://YOUR-WEB-PAGE.com');
		}
		else
		{
			echo "DEBUG:<br>";
			echo $safeauthor . "<br>";
			echo $safename . "<br>";
			echo $safeingredients . "<br>";
			echo $safedirections . "<br>";
			echo $safedifficulty . "<br>";
			echo $safephoto . "<br>";
		}
	}
	else
	{
		echo "Error";
	}
?>