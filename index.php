<?php 
  require_once 'Db_Functions.php';
  $url = "http://YOUR-WEB-PAGE.com"; 
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>Rand-O-Mater</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- Bootstrap -->
    <link href="css/bootstrap.min.css" rel="stylesheet" media="screen">
    <link href="css/footer_style.css" rel="stylesheet" media="screen">
    <link href="css/hide.css" rel="stylesheet" media="screen">

    <script type="text/javascript" src="js/jquery.min.js"></script>
    <script type="text/javascript" src="js/jquery.validate.min.js"></script>
    <script type="text/javascript">
    $(document).ready(function()
    {
      // Validation
      $("#submitRecipe").validate({
        rules:{
          fname:"required",
          frecipename:"required",
          fingredients:"required",
          fdirections:"required",
          fphoto:"required"
        },

        messages:{
          fname:"Enter your name",
          frecipename:"Enter your recipe name",
          fingredients:"Enter some ingredients",
          fdirections:"Enter some directions",
          fphoto:"Upload a photo of your food"
        },

        errorClass: "help-inline",
        errorElement: "span",
        highlight:function(element, errorClass, validClass)
        {
          $(element).parents('.control-group').addClass('error');
        },
          unhighlight: function(element, errorClass, validClass)
        {
          $(element).parents('.control-group').removeClass('error');
          $(element).parents('.control-group').addClass('success');
        }
      });
    });
    </script>
    <script type="text/javascript">
      function reloadPage()
      {
        window.location.assign("index.php")
      }
    </script>
    <script type="text/javascript" src="js/bootstrap.min.js"></script>
  </head>
  <body>
    <div style="display: none" id="hideAll">&nbsp;</div>
    <script type="text/javascript">
      document.getElementById("hideAll").style.display = "block";
      window.onload = function() { document.getElementById("hideAll").style.display = "none"; }
    </script>

    <!-- Modal -->
    <div id="submitModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
        <h3 id="myModalLabel">Send us your recipe</h3>
      </div>
      <form id="submitRecipe" method="post" action="submitRecipe.php">
        <div class="modal-body">
          <fieldset>
            <div class="control-group">
              <label>Your name</label>
              <input id="fname" name="fname" type="text" placeholder="Author" autocomplete="off">
            </div>
            <div class="control-group">
              <label>Recipe name</label>
              <input id="frecipename" name="frecipename" type="text" placeholder="Give it an awesome name!" autocomplete="off">
            </div>
            <div class="control-group">
            <label>Difficulty</label>
              <select id="fdifficulty" name="fdifficulty">
                <option>EASY</option>
                <option>MEDIUM</option>
                <option>HARD</option>
              </select>
            </div>
            <div class="control-group">
              <label>Ingredients</label>
              <textarea id="fingredients" name="fingredients" rows="4" placeholder="One per line" autocomplete="off"></textarea>
            </div>
            <div class="control-group">
              <label>Directions</label>
              <textarea id="fdirections" name="fdirections" rows="4" placeholder="One per line" autocomplete="off"></textarea>
            </div>
            <div class="control-group">
              <label>Link a photo</label>
              <input id="fphoto" name="fphoto" type="text" placeholder="http://www.example.com/path/to/photo.jpg" autocomplete="off">
            </div>
          </fieldset>
        </div>
        <div class="modal-footer">
          <button class="btn" data-dismiss="modal" aria-hidden="true">Close</button>
          <button name="submit" type="submit" class="btn btn-primary">Submit recipe</button>
        </div>
      </form>
    </div>

    <!-- Content -->
    <div id="wrap">
      <div class="container-fluid">
        <div class="row-fluid">
          <div class="span2"></div>

          <!-- MAIN CONTENT -->
          <div class="span8">
            <div class="page-header">
              <h1 class="text-center">
                <div class="btn-group text-center">
                  <button class="btn btn-primary" onClick="reloadPage()">Give me another recipe!</button>
                  <a href="#submitModal" role="button" class="btn" data-toggle="modal">Submit my own recipe!</a>
                </div>
              </h1>
              <h1>
                <?php
                  $dbf = new Db_Functions();
                  if(isset($_GET['recipe']) && $_GET['recipe'] != '')
                  {
                    $recipe = $dbf->retrieveRecipeById($_GET['recipe']);
                  }
                  else
                  {
                    $recipe = $dbf->retrieveRecipe();
                  }

                  if($recipe == NULL) 
                  { 
                    echo "Unknown recipe";
                  }
                  else
                  {
                    $url_aux = $url . "?recipe=" . $recipe->id;
                    echo "<a href=\"$url_aux\">$recipe->name</a>";
                  }
                ?>
              </h1>
              <h4>
                <?php 
                  echo "Difficulty: ";
                  if($recipe->difficulty == "EASY")
                  {
                    echo "<span style=\"color:#37C837;\">EASY</span>";
                  }
                  else if($recipe->difficulty == "MEDIUM")
                  {
                    echo "<span style=\"color:#ED8F12;\">MEDIUM</span>";
                  }
                  else
                  {
                    echo "<span style=\"color:#B71604;\">HARD</span>";
                  }
                ?>
              </h4>
              <h5>
                <div class="row-fluid">
                  <div class="span6 text-left">
                    <div style="margin-right:5px; margin-top:-1px;" class="pull-left">
                      <a href="https://twitter.com/share" class="twitter-share-button" data-url="<?php echo $url . "?recipe=" . $recipe->id ?>" data-text="<?php echo $recipe->name ?>" data-count="none">Tweet</a>
                      <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>
                    </div>
                    <div class="pull-left">
                      <a name="fb_share" type="button" share_url="<?php echo $url . "?recipe=" . $recipe->id ?>">Share</a> 
                      <script src="http://static.ak.fbcdn.net/connect.php/js/FB.Share" type="text/javascript"></script>
                    </div>
                  </div>
                  <div class="span6 text-right">
                    <?php
                      echo "Submitted by " . $recipe->author;
                    ?>
                  </div>
                </div>
              </h5>
            </div>

            <div class="row-fluid">
              <div class="span4">
                <p class="lead">Ingredients:</p>
                <p>
                  <ul>
                  <?php
                    $ingredients = explode("\n", $recipe->ingredients);
                    for($i = 0; $i < count($ingredients); $i++)
                    {
                      echo "<li>$ingredients[$i]</li>";
                    }
                  ?>
                  </ul>
                </p>
              </div>
              <div class="span8">
                <a href="<?php echo $recipe->photo ?>" class="thumbnail">
                  <img src="<?php echo $recipe->photo ?>" alt="" width="100%" height="100%" class="img-rounded" />
                </a>
              </div>
            </div>

            <p class="lead">Directions:</p>
            <p>
              <ol>
              <?php
                $directions = explode("\n", $recipe->directions);
                for($i = 0; $i < count($directions); $i++)
                {
                  echo "<li>$directions[$i]</li>";
                }
              ?>
              </ol>
            </p>

            <div id="footer">
              <p class="muted credit text-right">A working prototype for assignement 2, <a href="http://www.uib.no/course/INFO261">Interaction Design</a>.<br><i><a href="http://www.twitter.com/Puyover">Santiago Sánchez Sobrino</a>, 2013.</i></p>
            </div>
          </div>

          <div class="span2"></div>
        </div>
      </div>
    </div>
  </body>
</html>