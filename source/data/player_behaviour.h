#ifndef _PLAYER_BEHAVIOUR_H_
	#define _PLAYER_BEHAVIOUR_H_
	
	var player_always_run = false; // true is we are allowed to run always, otherwise - false
	var player_health = 0; // used for gui
	var player_armor = 0; // used for gui
	var player_allow_movement = false; // changes by player's obj_allow_move skill
	
	void player_toggle_run();
	
	void player_event();
	
	void player_dead();
	
	action player_controller();

	#include "player_behaviour.c"
#endif