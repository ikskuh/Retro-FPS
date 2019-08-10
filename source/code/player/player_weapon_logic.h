#ifndef _PLAYER_WEAPONS_H_
#define _PLAYER_WEAPONS_H_

// pointer to weapon's shooting function
var weapon_function(PLAYER *hero);

// find better weapon
void player_weapon_find_better(PLAYER *hero, var num);

// reset weapons input
void player_weapon_reset_input(PLAYER *hero);

// reset weapon by id
void player_weapon_reset_by_id(PLAYER *hero, var num);

// reset all weapons
void player_weapons_reset_all(PLAYER *hero);

// activate weapon by id
void player_weapon_activate_by_id(PLAYER *hero, var num);

// function used for all weapon sprites
void player_weapon_fnc();

// setup basic weapon settings at start
void player_weapons_initialize(ENTITY *ent, PLAYER *hero);

// update weapon's pos/angle
void player_weapon_view_pos(PLAYER *hero);

// handle all weapon animations
void player_weapon_animate(PLAYER *hero);

// switch to weapon by id
void player_switch_weapon_by_id(PLAYER *hero, var num);

// switch weapons
void player_switch_weapons(PLAYER *hero);

// main function to update all weapons
void player_weapons_update(ENTITY *ent, PLAYER *hero);

#endif