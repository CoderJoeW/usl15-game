<?php

/**
 * @file functions-equipment.inc
 * Misc Equipment functions for Ziquid games.
 *
 * Synced with CG: yes
 * Synced with 2114: no
 */

function game_required_equipment(\stdClass $game_user, $equipment_id, $req_quantity) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  $sql = 'select quantity from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $equipment_id, $game_user->id);
  $quantity = db_fetch_object($result);

  if ($quantity->quantity >= $req_quantity) {
    $not_yet = $a_start = $a_end = '';
  }
  else {
    $not_yet = 'not-yet';
    $a_start = '<a href="/' . $game . '/equipment_buy/' .
      $arg2 . '/' . $equipment_id . '/' .
      ($req_quantity - $quantity->quantity) . '">';
    $a_end = '</a>';
  }

  echo <<< EOF
    <div class="quest-required_stuff">Requires
      <div class="quest-required_equipment">
        $a_start
        <img class="$not_yet"
          src="/sites/default/files/images/equipment/$game-$equipment_id.png"
          width="48">
        $a_end
      </div>&nbsp;x$req_quantity
    </div>
EOF;
}

/**
 * Fetch the list of equipment to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 *
 * @return \stdClass[]
 *   The array of equipment objects.
 */
function game_fetch_visible_equip($game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'WHERE equipment.id > 0';
  }
  else {
    $sql_to_add = 'WHERE (((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
    )
  
    AND
  
    (
      fkey_values_id = 0
      OR fkey_values_id = %d
    ))
  
      AND required_level <= %d
      AND active = 1
      AND is_loot = 0
    )
  
    OR equipment_ownership.quantity > 0 ';
  }

  $data = [];
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  ' . $sql_to_add . '
  ORDER BY required_level ASC';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);

  while ($item = db_fetch_object($result)) {
    $data[] = $item;
  }
  return $data;
}

/**
 * Fetch the next equipment item to show.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @return \stdClass
 *   The next equipment item to show, if any.
 */
function game_fetch_next_equip($game_user) {
  if ($game_user->meta == 'admin') {
    $sql_to_add = 'AND equipment.id > 0';
  }
  else {
    $sql_to_add = 'AND active = 1
      AND is_loot = 0';
  }
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE ((
    fkey_neighborhoods_id = 0
    OR fkey_neighborhoods_id = %d
  )

  AND
  (
    fkey_values_id = 0
    OR fkey_values_id = %d
  ))

  AND required_level > %d
  ' . $sql_to_add . '
  ORDER BY required_level ASC LIMIT 1';
  $result = db_query($sql, $game_user->id, $game_user->fkey_neighborhoods_id,
    $game_user->fkey_values_id, $game_user->level);
  return db_fetch_object($result);
}

/**
 * Fetch the specified equipment item.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $equip_id
 *   The equip id.
 *
 * @return \stdClass
 *   The equipment item, if any.
 */
function game_fetch_equip_by_id($game_user, $equip_id) {
  $sql = 'SELECT equipment.*, equipment_ownership.quantity
  FROM equipment

  LEFT OUTER JOIN equipment_ownership ON equipment_ownership.fkey_equipment_id = equipment.id
  AND equipment_ownership.fkey_users_id = %d

  WHERE equipment.id = %d;';
  $result = db_query($sql, $game_user->id, $equip_id);
  return db_fetch_object($result);
}

/**
 * Show a piece of equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param \stdClass $item
 *   The equipment item.
 * @param string $ai_output
 *   AI output.
 * @param array $options
 *   Options that modify the display behavior.
 */
function game_show_equip($game_user, $item, &$ai_output, $options = []) {

  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  static $party_title;
  $classes = [];

  if (empty($party_title)) {
    $sql = 'select party_title from `values` where id = %d;';
    $result = db_query($sql, $game_user->fkey_values_id);
    $data = db_fetch_object($result);
    $party_title = preg_replace('/^The /', '', $data->party_title);
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $header = ($options['equipment-succeeded'] == 'success') ?
    '<div class="land-succeeded">Success</div>' : '';

  switch ($options['equipment-buy-succeeded']) {

    case 'failed no-money':
      $offer = game_luck_money_offer($game_user);
      $header = '<div class="title">' .

        // FIXME remove special characters from text here.
        t('∆ @value <span>Insufficient</span> ∆',
          ['@value' => $game_user->values]) . '
        </div>
        <div class="try-an-election-wrapper">
          <div class="try-an-election">
            <a href="/' . $game . '/elders_do_fill/' . $arg2 .
        '/money?destination=/' . $game . '/equipment/' . $arg2 . '">' .
        t('Receive @offer @values (1&nbsp;@luck',
          [
            '@offer' => $offer,
            '@values' => $game_user->values,
            '@luck' => $luck,
          ]) . ')
            </a>
          </div>
        </div>';

      $failed = 'failed';
      break;

    case 'failed not-required-level':
      $header = '<div class="title">' .
        t('∆ Player Level <span>Insufficient</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed hit-quantity-limit':
      $header = '<div class="title">' .
        t('Limit reached!') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-enough-income':
      $header = '<div class="title">' .
        t('Not enough hourly income!') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-required-hood':
      $header = '<div class="title">' .
        t('∆ Location <span>Incorrect</span> ∆') .
        '</div>';
      $failed = 'failed';
      game_competency_gain($game_user, 'lost');
      break;

    case 'failed not-required-party':
      $header = '<div class="title">' .
        t('∆ @party <span>Incorrect</span> ∆',
          ['@party' => $party]) .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed not-active':
    case 'failed is-loot':
      $header = '<div class="title">' .
        t('∆ Item <span>Unavailable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;

    case 'failed cant-sell':
      $header = '<div class="title">' .
        t('∆ Item <span>Unsalable</span> ∆') .
        '</div>';
      $failed = 'failed';
      break;
  }

  $description = str_replace('%party', "<em>$party_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) {
    $quantity = '<em>' . t('None') . '</em>';
  }

  $equipment_price = $item->price + ($item->quantity *
      $item->price_increase);

  if (!$item->is_loot) {
    $ai_output .= " $item->id=$equipment_price";
  }

  if ((($equipment_price % 1000) == 0) && $equipment_price > 0) {
    $equipment_price = ($equipment_price / 1000) . 'K';
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  $can_buy = $can_sell = TRUE;
  $cant_buy_reason = $cant_sell_reason = '';

  if ($item->fkey_neighborhoods_id != 0 &&
    $item->fkey_neighborhoods_id != $game_user->fkey_neighborhoods_id) {
    $can_buy = FALSE;
    $sql = 'select name from neighborhoods where id = %d;';
    $result = db_query($sql, $item->fkey_neighborhoods_id);
    $hood = db_fetch_object($result);
    $cant_buy_reason = '(' . $hood->name . ' only)';
  }

  if ($item->fkey_values_id != 0 &&
    $item->fkey_values_id != $game_user->fkey_values_id) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Wrong ' . $party . ')';
  }

  if ($item->required_level > $game_user->level) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Not Enough ' . $experience . ')';
  }

  if (!$item->active) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Inactive)';
  }

  if ($item->is_loot != 0) {
    $can_buy = FALSE;
    $cant_buy_reason = '(Loot Only)';
  }

  if ($item->can_sell != 1) {
    $can_sell = FALSE;
    $cant_sell_reason = '(Ever)';
  }

  if ($item->quantity < 1) {
    $can_sell = FALSE;
  }

  $inactive = ($item->active) ? '' : t('(Inactive)');

  if ($can_buy) {
    $icon_button = <<< EOF
<a href="/$game/equipment_buy/$arg2/$item->id/1">
  <img src="/sites/default/files/images/equipment/$game-$item->id.png?2" width="96"
    border="0">
</a>
EOF;
    $name_link = <<< EOF
<a href="/$game/equipment_buy/$arg2/$item->id/1">
  <span class="type">
    $inactive
  </span>
  $item->name
</a>
EOF;
    $buy_button = <<< EOF
<div class="land-buy-button">
  <a href="/$game/equipment_buy/$arg2/$item->id/1">
    Buy
  </a>
</div>
EOF;
  }
  else {
    // Can't buy.
    $icon_button = <<< EOF
  <img src="/sites/default/files/images/equipment/$game-$item->id.png?2" width="96"
    border="0">
EOF;
    $name_link = <<< EOF
<span class="type">
  $inactive
</span>
$item->name
EOF;
    $buy_button = <<< EOF
<div class="land-buy-button not-yet">
  Can't Buy $cant_buy_reason
</div>
EOF;
  }

  if ($can_sell) {
    $sell_button = <<< EOF
<div class="land-sell-button">
  <a href="/$game/equipment_sell/$arg2/$item->id/1">
    Sell
  </a>
</div>
EOF;
  }
  else {
    $sell_button = <<< EOF
<div class="land-sell-button not-yet">
  Can't Sell $cant_sell_reason
</div>
EOF;
  }

  // grab each action for the equipment
  $acts_arr = [];
  $sql = 'select * from actions where fkey_equipment_id = %d;';
  $result = db_query($sql, $item->id);
  while ($action = db_fetch_object($result)) {
    $acts_arr[] = $action;
  }

  if ($item->initiative_bonus != 0) {
    $classes[] = 'equip-ini';
  }

  if ($item->endurance_bonus != 0) {
    $classes[] = 'equip-end';
  }

  if ($item->elocution_bonus != 0) {
    $classes[] = 'equip-elo';
  }

  if ($item->speed_increase != 0) {
    $classes[] = 'equip-move';
  }

  if ($can_buy) {
    $classes[] = 'equip-buy';
  }

  if ($can_sell) {
    $classes[] = 'equip-sal';
  }

  if ($item->upkeep != 0) {
    $classes[] = 'equip-upk';
  }

  if ($item->energy_bonus != 0 || $item->energy_increase != 0) {
    $classes[] = 'equip-eng';
  }

  if (count($acts_arr) > 0) {
    $classes[] = 'equip-act';
  }

  $class_str = implode(' ', $classes);

  if ($soon) {
    echo <<< EOF
<div class="land $soon $failed $class_str">
  <div class="land-icon">
      <img src="/sites/default/files/images/equipment/$game-soon.png?1" width=96
        border="0">
  </div>
  <div class="land-details">
    <div class="land-name">
        <span class="type">$inactive</span>
        $item->name
    </div>
    <div class="land-description">$description</div>
EOF;
  }
else {
    echo <<< EOF
<div class="land $failed $class_str">
  $header
  <div class="land-icon">
    $icon_button
  </div>
  <div class="land-details">
    <div class="land-name">
      $name_link
    </div>
    <div class="land-description">$description</div>
EOF;
  }

  echo <<< EOF
    <div class="land-owned">Owned: $quantity $quantity_limit</div>
EOF;
  $reqs = <<< EOF
    <div class="land-cost">Cost: $equipment_price $game_user->values</div>
EOF;

  if ($game_user->level < $item->required_level) {
    $reqs .= <<< EOF
    <div class="land-required_level">Player: Level $item->required_level</div>
EOF;
  }

  if (!empty($reqs)) {
    echo <<< EOF
    <!--<div class="land-requisites">// Requisites //</div>-->
    $reqs
EOF;
  }

  if ($item->energy_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">
    Energy: +$item->energy_bonus immediate energy bonus
  </div>
EOF;
  }

  if ($item->energy_increase > 0) {
    echo <<< EOF
  <div class="land-payout">
    Energy: +$item->energy_increase every 5 minutes
  </div>
EOF;
  }

  if ($item->initiative_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">
    $initiative: +$item->initiative_bonus
  </div>
EOF;
  }
else if ($item->initiative_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">
    $initiative: $item->initiative_bonus
  </div>
EOF;
  }

  if ($item->endurance_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">
    $endurance: +$item->endurance_bonus
  </div>
EOF;
  }
else if ($item->endurance_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">
    $endurance: $item->endurance_bonus
  </div>
EOF;
  }

  if ($item->elocution_bonus > 0) {
    echo <<< EOF
  <div class="land-payout">
    $elocution: +$item->elocution_bonus
  </div>
EOF;

    if (!$item->is_loot) {
      $ai_output .= "/+elo:$item->elocution_bonus";
    }

  }
else if ($item->elocution_bonus < 0) {
    echo <<< EOF
  <div class="land-payout negative">
    $elocution: $item->elocution_bonus
  </div>
EOF;

    if (!$item->is_loot) {
      $ai_output .= "/+elo:$item->elocution_bonus";
    }
  }

  if ($item->speed_increase > 0) {
    echo <<< EOF
  <div class="land-payout">
    Speed Increase: $item->speed_increase fewer Action
    needed to move to a new $hood_lower
  </div>
EOF;

    if ((!$item->is_loot) && ($item->quantity == 0)) {
      $ai_output .= "/+spd:$item->speed_increase";
    }

  }

  if ($item->upkeep > 0) {
    echo <<< EOF
  <div class="land-payout negative">
    Upkeep: $item->upkeep every 60 minutes
  </div>
EOF;
  }

  if ($item->chance_of_loss > 0) {
    $lifetime = floor(100 / $item->chance_of_loss);
    $use = ($lifetime == 1) ? 'use' : 'uses';
    echo <<< EOF
  <div class="land-payout negative">
    Expected Lifetime: $lifetime $use
  </div>
EOF;
  }

  foreach ($acts_arr as $action) {
//    firep($action, 'action associated with equipment');

    $cost = "Cost: $action->cost Action";
    if ($action->values_cost > 0)
      $cost .= ", $action->values_cost $game_user->values";

    $name = str_replace('%value', $game_user->values, $action->name);

    echo '<div class="land-action">Action: ' . $name . '</div>';
    echo '<div class="land-description">' . $action->description . '</div>';
    echo '<div class="land-action-cost">' . $cost . '</div>';

    if ($action->influence_change < 0) {
      $inf_change = -$action->influence_change;
      echo <<< EOF
        <div class="land-payout negative">Effect: Target's
          {$experience_lower} is reduced by $inf_change</div>
EOF;
    }

    if (($action->rating_change < 0.10) && ($action->rating_change != 0.0)) {
      $rat_change = abs($action->rating_change);

      if ($action->rating_change < 0.0) {
        echo <<< EOF
    <div class="land-payout negative">Effect: $target approval rating is
      reduced by $rat_change%</div>
EOF;
      }
      else {
        echo <<< EOF
    <div class="land-payout">Effect: $target approval rating is
      increased by $rat_change%</div>
EOF;
      }

    }

    if ($action->rating_change >= 0.10) {
      $rat_change = $action->rating_change;
      echo <<< EOF
    <div class="land-payout">Effect: Your approval rating is
      increased by $rat_change%</div>
EOF;
    }

    if ($action->neighborhood_rating_change < 0.0) {
      $rat_change = -$action->neighborhood_rating_change;
      echo <<< EOF
  <div class="land-payout negative">Effect: $hood $beauty_lower
    rating is reduced by $rat_change</div>
EOF;
    }

    if ($action->neighborhood_rating_change > 0.0) {
      $rat_change = $action->neighborhood_rating_change;
      echo <<< EOF
  <div class="land-payout">Effect: $hood $beauty_lower rating is
    increased by $rat_change</div>
EOF;
    }

    if ($action->values_change < 0) {
      $val_change = -$action->values_change;
      echo <<< EOF
        <div class="land-payout negative">Effect: Target's $game_user->values is
          reduced by $val_change</div>
EOF;
    }
  }

  echo '</div>';

  // BUY buttons.
  if ($soon) {
    echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button not-yet">
      Can't Buy
    </div>
    <div class="land-sell-button not-yet">
      Can't Sell
    </div>
  </div>
EOF;
  }
  else {

    // Attempt to buy.
    if ($options['equipment-buy-succeeded']) {
      echo <<< EOF
<div class="land-button-wrapper">
  <form action="/$game/equipment_buy/$arg2/$item->id/use-quantity">
    <div class="quantity">
      <select name="quantity">
EOF;
      foreach ([1, 5, 10, 25, 50, 100] as $option) {
        if ($option == $options['orig-quantity']) {
          echo '<option selected="selected" value="' . $option . '">' .
            $option . '</option>';
        }
        else {
          echo '<option value="' . $option . '">' . $option . '</option>';
        }
      }
      echo <<< EOF
      </select>
    </div>
    <input class="land-buy-button" type="submit" Value="Buy"/>
  </form>
</div>
EOF;

    }

    // Normal show.
    else {
      echo <<< EOF
  <div class="land-button-wrapper">
    $buy_button
    $sell_button
  </div>
EOF;
    }

  }

  echo '</div>';
}

function game_equipment_gain(&$game_user, $id, $quantity, $eq_price = 0) {
  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  // BUG!  How do these get here?
  if ($id == 0) {
    mail('joseph@ziquid.com', $game_user->name . ' trying to get equipment 0!',
      $game_user->phone_id);
    return [FALSE, 'bad-id', 0];
  }

  // FIXME: refactor to use game_fetch_equip_by_id().
  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Check for enough income to cover upkeep.
  if ($game_user->income <
    $game_user->expenses + ($eq->upkeep * $quantity)) {
    return [FALSE, 'not-enough-income', ($eq->upkeep * $quantity)];
  }

  // Eo entry already there.
  if (!empty($eo)) {

    if (($eq->quantity_limit > 0) &&
      (($eo->quantity + $quantity) > $eq->quantity_limit)) {
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];
    }

    $sql = 'update equipment_ownership set quantity = quantity + %d
      where fkey_equipment_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
  }
  else {

    if (($eq->quantity_limit > 0) && ($quantity > $eq->quantity_limit)) {
      return [FALSE, 'quantity-exceeded', $eq->quantity_limit];
    }

    $sql = 'insert into equipment_ownership
      (fkey_users_id, fkey_equipment_id, quantity) values
      (%d, %d, %d);';
    db_query($sql, $game_user->id, $id, $quantity);
  }

  // Pay for the equipment, if needed.
  // FIXME: don't process if eq_price is 0.
  $sql = 'update users set money = money - %d where id = %d;';
  db_query($sql, $eq_price, $game_user->id);

  // Give energy bonus, if needed.
  if ($eq->energy_bonus > 0) {

    // Start the energy clock, if needed.
    if ($game_user->energy == $game_user->energy_max) {
      $sql = 'update users set energy_next_gain = "%s" where id = %d;';
      db_query($sql, date('Y-m-d H:i:s', REQUEST_TIME + 300),
        $game_user->id);
    }

    $sql = 'update users set energy = energy + %d where id = %d;';
    db_query($sql, $eq->energy_bonus * $quantity, $game_user->id);
  }

  // Reprocess user object.
  game_recalc_income($game_user);
  $game_user = $fetch_user();
  return [TRUE, 'success', 0];
}

/**
 * Lose (or Sell) one or more pieces of equipment.
 *
 * @param \stdClass $game_user
 *   The game user object.
 * @param int $id
 *   The equipment ID.
 * @param int $quantity
 *   Number of pieces to lose/sell
 * @param int $eq_price
 *   Amount of compensation to receive for sale.
 *
 * @return bool
 *   Whether the sale succeeded or not.
 */
function game_equipment_lose(&$game_user, $id, $quantity, $eq_price = 0) {
  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  // FIXME: refactor to use game_fetch_equip_by_id().
  $sql = 'select * from equipment where id = %d;';
  $result = db_query($sql, $id);
  $eq = db_fetch_object($result);

  $sql = 'select * from equipment_ownership
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  $result = db_query($sql, $id, $game_user->id);
  $eo = db_fetch_object($result);

  // Nothing there?
  if (empty($eo)) {
    return FALSE;
  }

  // Not enough?
  if ($eo->quantity < $quantity) {
    return FALSE;
  }

  $sql = 'update equipment_ownership set quantity = quantity - %d
    where fkey_equipment_id = %d and fkey_users_id = %d;';
  db_query($sql, $quantity, $id, $game_user->id);

  // Receive money for the equipment, if needed.
  // FIXME: don't process if money is 0.
  $sql = 'update users set money = money + %d where id = %d;';
  db_query($sql, $eq_price, $game_user->id);

  // Reprocess user object.
  game_recalc_income($game_user);
  $game_user = $fetch_user();
  return TRUE;
}

function game_equipment_use(&$game_user, $id, $quantity) {
  global $game;
  include drupal_get_path('module', $game) . '/game_defs.inc';

  if (game_equipment_lose($game_user, $id, $quantity, 0)) {
    $sql = 'update equipment_ownership set quantity_used = quantity_used + %d
    where fkey_equipment_id = %d and fkey_users_id = %d;';
    db_query($sql, $quantity, $id, $game_user->id);
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function show_equipment($game_user, $item, $options = []) {

  global $game, $phone_id;
  include drupal_get_path('module', $game) . '/game_defs.inc';
  $arg2 = check_plain(arg(2));
  static $party_title;

  $description = str_replace('%clan', "<em>$party_title</em>",
    $item->description);

  $quantity = $item->quantity;
  if (empty($quantity)) $quantity = '<em>None</em>';

  $equipment_price = $item->price + ($item->quantity *
    $item->price_increase);

  if (!$item->is_loot) $ai_output .= " $item->id=$equipment_price";

  if (($equipment_price % 1000) == 0)
    $equipment_price = ($equipment_price / 1000) . 'K';

  if ($equipment_price === '0K') {
    $equipment_price = 'Free';
  }
  else {
    $equipment_price .= ' ' . $game_user->values;
  }

  if ($item->quantity_limit > 0) {
    $quantity_limit = '<em>(Limited to ' . $item->quantity_limit . ')</em>';
  }
  else {
    $quantity_limit = '';
  }

  if (strpos($_SERVER['REQUEST_URI'], 'equipment') !== FALSE) {
    $link = 'equipment';
  }
  else {
    $link = 'weapons';
  }

  $soon = ($options['soon']) ? 'soon' : '';
  $clan = ($options['clan']) ? 'clan' : '';

  if ($soon) {
    $icon = "$game-soon.png";
    $a_start = $a_end = '';
  }
  else {
    $icon = "$game-$item->id.png";
    $a_start = "<a href=\"/$game/{$link}_buy/$arg2/$item->id/1\">";
    $a_end = '</a>';
  }

  // Cannot buy.
  if ($clan | $item->is_loot) {
    $a_start = $a_end = '';
  }

  echo <<< EOF
<div class="land">
  <div class="land-icon">
    $a_start
      <img src="/sites/default/files/images/equipment/$icon"
        border="0" width="96"/>
    $a_end
  </div>
  <div class="land-details">
    <div class="land-name">
      $a_start
        $item->name
      $a_end
    </div>
    <div class="land-description">
      $description
    </div>
EOF;

  if (empty($soon)) echo <<< EOF
    <div class="land-owned">
      Owned: $quantity $quantity_limit
    </div>
EOF;

  echo <<< EOF
    <div class="land-requisites">
      // Requisites //
    </div>
EOF;

  if ($item->required_level > $game_user->level) echo <<< EOF
    <div class="land-required_level">
      Player: Level $item->required_level
    </div>
EOF;

  echo <<< EOF
    <div class="land-cost">
      Cost: $equipment_price
    </div>
EOF;

  if ($item->energy_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_bonus immediate energy bonus
    </div>
EOF;

  }

  if ($item->energy_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Energy: +$item->energy_increase every 5 minutes
    </div>
EOF;

  }

  if ($item->initiative_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $initiative: +$item->initiative_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $endurance: +$item->endurance_bonus
    </div>
EOF;

  }

  if ($item->endurance_bonus < 0) {

    echo <<< EOF
    <div class="land-payout negative">
      $endurance: $item->endurance_bonus
    </div>
EOF;

  }

  if ($item->elocution_bonus > 0) {

    echo <<< EOF
    <div class="land-payout">
      $elocution: +$item->elocution_bonus
    </div>
EOF;

  if (!$item->is_loot) $ai_output .= "/+elo:$item->elocution_bonus";

  }

  if ($item->speed_increase > 0) {

    echo <<< EOF
    <div class="land-payout">
      Speed Increase: $item->speed_increase fewer Action
        needed to move to a new $hood_lower
    </div>
EOF;

  if ((!$item->is_loot) && ($item->quantity == 0))
    $ai_output .= "/+spd:$item->speed_increase";

  }

  if ($item->upkeep > 0) {

    echo <<< EOF
    <div class="land-payout negative">
      Upkeep: $item->upkeep every 60 minutes
    </div>
EOF;

  }

  if ($item->chance_of_loss > 0) {

    $lifetime = floor(100 / $item->chance_of_loss);
    $use = ($lifetime == 1) ? 'use' : 'uses';
    echo <<< EOF
    <div class="land-payout negative">
      Expected Lifetime: $lifetime $use
    </div>
EOF;

  }

  // Grab each action for the equipment.
  $data2 = [];
  $sql = 'select * from actions where fkey_equipment_id = %d;';
  $result = db_query($sql, $item->id);

  while ($action = db_fetch_object($result)) $data2[] = $action;

  foreach ($data2 as $action) {
firep($action);

    $cost = "Cost: $action->cost Action";
    if ($action->values_cost > 0)
      $cost .= ", $action->values_cost $game_user->values";

    $name = str_replace('%value', $game_user->values, $action->name);

    echo <<< EOF
    <div class="land-action">
      Action: $name
    </div>
    <div class="land-description">
      $action->description
    </div>
    <div class="land-action-cost">
      $cost
    </div>
EOF;

    if ($action->influence_change < 0) {

      $inf_change = -$action->influence_change;

      echo <<< EOF
    <div class="land-payout negative">
       Effect: Target's {$experience_lower} is reduced by $inf_change
    </div>
EOF;

    }

    if (($action->rating_change < 0.10) && ($action->rating_change != 0.0)) {

      $rat_change = abs($action->rating_change);

      if ($action->rating_change < 0.0) {

        echo <<< EOF
    <div class="land-payout negative">
      Effect: $target approval rating is reduced by $rat_change%
    </div>
EOF;

      }
      else {

        echo <<< EOF
    <div class="land-payout">
      Effect: $target approval rating is increased by $rat_change%
    </div>
EOF;

      }

    }

    if ($action->rating_change >= 0.10) {

      $rat_change = $action->rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Your approval rating is increased by $rat_change%
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change < 0.0) {

      $rat_change = -$action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Neighborhood $beauty_lower rating is reduced by $rat_change
    </div>
EOF;

    }

    if ($action->neighborhood_rating_change > 0.0) {

      $rat_change = $action->neighborhood_rating_change;

      echo <<< EOF
    <div class="land-payout">
      Effect: Neighborhood $beauty_lower rating is increased by $rat_change
    </div>
EOF;

    }

    if ($action->values_change < 0) {

      $val_change = -$action->values_change;

      echo <<< EOF
    <div class="land-payout negative">
      Effect: Target's $game_user->values is reduced by $val_change
    </div>
EOF;

    }

  }

  echo '</div>';

  if (empty($soon) && empty($clan)) {

    if (!$item->is_loot) {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button">
      <a href="/$game/{$link}_buy/$arg2/$item->id/1">
        Buy
      </a>
    </div>
EOF;

    }
    else {

      echo <<< EOF
  <div class="land-button-wrapper">
    <div class="land-buy-button not-yet">
      Can't Buy
    </div>
EOF;

    }

    if ($item->can_sell) {

      echo <<< EOF
    <div class="land-sell-button">
      <a href="/$game/{$link}_sell/$arg2/$item->id/1">
        Sell
      </a>
    </div>
  </div>
EOF;

    }
    else {

      echo <<< EOF
    <div class="land-sell-button not-yet">
      Can't Sell
    </div>
  </div>
EOF;

    }
  }

  echo '</div>';
}
