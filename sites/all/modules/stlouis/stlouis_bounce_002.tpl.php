<?php
  $game = check_plain(arg(0));
  $arg2 = check_plain(arg(2));
  db_set_active('default');
?>
<style>
  body {
    margin: 6px auto;
    width: 100%;
  }
  p {
    margin: 6px;
  }
</style>

  <p>Uprising: St. Louis, Community Edition</p>
  <p>March 6, 2018 Beta release 0.2.0</p>
  <h4>Please note this release is for testing only</h4>
  <h4>Some things may change</h4>
<form method=post action="/<?php echo $game; ?>/home/<?php echo $arg2;?>">
  <input type="Submit" value="Continue to the game"/>
</form>