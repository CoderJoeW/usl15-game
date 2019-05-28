<?php

/**
 * @file
 * Template for bounce.
 *
 * Synced with CG: yes
 * Synced with 2114: yes
 * Ready for phpcbf: yes
 * Ready for MVC separation: no
 * Controller moved to callback include: no
 * View only in theme template: no
 * All db queries in controller: no
 * Minimal function calls in view: no
 * Removal of globals: no
 * Removal of game_defs include: N/A
 * .
 */

$game = check_plain(arg(0));
$arg2 = check_plain(arg(2));

if ($arg2 == 'facebook') {

  $phone_id = zg_get_fbid();
// echo $phone_id;
  echo <<< EOF
<form method=post action="/$game/home/$arg2">
  <input type="Submit" value="Continue to the game">
</form>
EOF;

  db_set_active('default');
  return;
}

db_set_active('default');
drupal_goto($game . '/home/' . $arg2);
