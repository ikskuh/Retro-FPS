#ifndef _PLAYER_BEHAVIOUR_H_
	#define _PLAYER_BEHAVIOUR_H_
	
	var player_always_run = false; // true is we are allowed to run always, otherwise - false
	var player_health = 0; // used for gui
	var player_armor = 0; // used for gui
	var player_allow_movement = false; // changes by player's obj_allow_move skill
	var player_death_snd_switch = 0; // used for playing death sound (both above/under water line)
	var player_snd_handle = 0; // handle for sound effects player by player (voice)
	var player_snd_volume = 65; // volume for all (voice) sound effects player by player
	
	void player_toggle_run();
	
	void player_sounds(CCT_PHYSICS *physics);
	
	void player_event();
	
	void player_dead();
	
	#include "player_behaviour.c"
#endif