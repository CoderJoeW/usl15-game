<?php

/**
 * @file elders_tpl.php
 * Stlouis elders
 *
 * Synced with CG: no
 * Synced with 2114: no
 * Ready for phpcbf: no
 */

global $game, $phone_id;
include drupal_get_path('module', $game) . '/game_defs.inc';
$game_user = $fetch_user();
$fetch_header($game_user);
$get_value = '_' . $game . '_get_value';

if (substr($phone_id, 0, 3) == 'ai-') {

  // Useful for freshening stats.
  echo "<!--\n<ai \"elders\"/>\n-->";
  db_set_active('default');
  return;
}

$offer = game_luck_money_offer($game_user);

echo <<< EOF
<div class="title">Visit the $elders</div>
<div class="subtitle">You have $game_user->luck&nbsp;$luck</div>
<div class="elders-menu">
EOF;

if ($game_user->level >= 6) {

  // Only allow users to change parties if they can join one.

  // AT LEAST 10 LUCK!
  if ($game_user->luck > 9) {

    echo <<< EOF
<div class="menu-option"><a href="/$game/choose_name/$arg2">Change your
character's name (10&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/choose_clan/$arg2/0">Join a
different $party_lower (5&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  elseif ($game_user->luck > 4) {

    // AT LEAST 5 LUCK!
    echo <<< EOF
<div class="menu-option not-yet">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option"><a href="/$game/choose_clan/$arg2/0">Join a
different political party (5&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  elseif ($game_user->luck > 2) {

    // AT LEAST THREE LUCK!
    echo <<< EOF
<div class="menu-option not-yet">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet">Join a different political party (5&nbsp;$luck)</div>
<div class="menu-option"><a href="/$game/elders_ask_reset_skills/$arg2">Reset
your skill points (3&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

 }
 elseif ($game_user->luck > 0) {

    // AT LEAST ONE LUCK!
    echo <<< EOF
<div class="menu-option not-yet">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet">Join a different political party (5&nbsp;$luck)</div>
<div class="menu-option not-yet">Reset your skill points (3&nbsp;$luck)</div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/action">Refill
your Action (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/energy">Refill
your Energy (1&nbsp;$luck)</a></div>
<div class="menu-option"><a href="/$game/elders_do_fill/$arg2/money">Receive
$offer $game_user->values (1&nbsp;$luck)</a></div>
EOF;

  }
  else {

    // NO LUCK!
    echo <<< EOF
<div class="menu-option not-yet">Change your character's name (10&nbsp;$luck)</div>
<div class="menu-option not-yet">Join a different political party (5&nbsp;$luck)</div>
<div class="menu-option not-yet">Reset your skill points (3&nbsp;$luck)</div>
<div class="menu-option not-yet">Refill your Action (1&nbsp;$luck)</div>
<div class="menu-option not-yet">Refill your Energy (1&nbsp;$luck)</div>
<div class="menu-option not-yet">Receive $offer $game_user->values (1&nbsp;$luck)</div>
EOF;

  }

}

echo <<< EOF
<div class="menu-option"><a href="/$game/elders_set_password/$arg2">Set a
password for your account (Free)</a></div>
<div class="menu-option"><a href="/$game/elders_ask_reset/$arg2">Reset
your character (Free)</a></div>
<!--<div class="menu-option"><a href="/$game/elders_preferences/$arg2">Game
Preferences</a></div>-->
<div class="menu-option"><a href="/$game/elders_ask_purchase/$arg2">Purchase
more $luck</a></div>
EOF;

if ((($game == 'stlouis') &&
  ($get_value($game_user->id, 'allow_2114', 0) == 1)) ||
  ($game_user->level >= 125)) {

  $new_host = ($_SERVER['HTTP_HOST'] == 'codero1.cheek.com') ?
    'stl2114.game.ziquid.com' : 'codero1.cheek.com';

  echo <<< EOF
<div class="menu-option"><a href="http://$new_host/$game/home/$arg2">Switch
games</a></div>
EOF;

}

echo '</div><br/><br/>';

db_set_active('default');
