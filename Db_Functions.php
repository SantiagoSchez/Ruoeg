<?php
 
class Db_Functions {
	private $mysqli;

	public function __construct() 
	{
		$this->mysqli = new mysqli("MySQL Host Name", "MySQL User Name", "MySQL Password", "MySQL DB Name");
		if ($this->mysqli->connect_errno) 
		{
   			printf("Connection failed: %s\n", $mysqli->connect_error);
   			exit();
		}
		$this->mysqli->query("SET NAMES 'utf8'");
	}

	public function __destruct()
	{
		$this->mysqli->close();
	}

	public function escapeString($string)
	{
		return $this->mysqli->real_escape_string($string);
	}
	
	public function storeRecipe($author, $name, $ingredients, $directions, $difficulty, $photo) 
	{
		$query = "INSERT INTO recipe_table(author, name, ingredients, directions, difficulty, photo) VALUES('$author', '$name', '$ingredients', '$directions', '$difficulty', '$photo')";
		$result = $this->mysqli->query($query);
		return $result;
	}

	public function retrieveRecipe()
	{
		$no_of_rows = $this->mysqli->query("SELECT id FROM recipe_table")->num_rows;
		$id = rand(1, $no_of_rows);

		if($result = $this->mysqli->query("SELECT * FROM recipe_table WHERE id='$id'"))
		{
			$recipe = $result->fetch_object();
			$result->close();
		}

		return $recipe;
	}

	public function retrieveRecipeById($id)
	{
		if($result = $this->mysqli->query("SELECT * FROM recipe_table WHERE id='$id'"))
		{
			$recipe = $result->fetch_object();
			$result->close();
		}

		return $recipe;
	}
}
 
?>