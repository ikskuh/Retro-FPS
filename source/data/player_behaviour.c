
#include "player_behaviour.h"

// toggle auto run mode on and off
void player_toggle_run(){
	
	if(hero->allow_movement == false || hero->health <= 0){ return; }
	if(game_level_is_loaded == false){ return; }
	
	hero->always_run += 1;
	hero->always_run %= 2;
}

// handles all voice sounds made by player
void player_sounds(ENTITY * ent, CCT *cct){
	
	if(ent->obj_state == DEATH){
		
		// smashed ?
		if(ent->obj_death_type == TYPE_SMASHED){
			
			if(hero->death_snd_switch == 0){
				
				if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
				ent->obj_snd_handle = snd_play(player_smashed_ogg, player_snd_volume, 0);
				
				hero->death_snd_switch = 1;
			}
		}
		else{
			
			if(hero->death_snd_switch == 0){
				
				if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
				var rnd = integer(random(2));
				if(rnd == 0){ ent->obj_snd_handle = snd_play(player_death_ogg, player_snd_volume, 0); }
				if(rnd == 1){ ent->obj_snd_handle = snd_play(player_x_death_ogg, player_snd_volume, 0); }
				
				hero->death_snd_switch = 1;
			}
			
			if(cct->water_state >= IN_WATER && hero->death_snd_switch == 1){
				
				if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
				ent->obj_snd_handle = snd_play(player_death_to_water_ogg, player_snd_volume, 0);
				hero->death_snd_switch = -1;
			}
		}
	}
}

// handles all events that player receives
// such as EVENT_PUSH, EVENT_SCAN, EVENT_SHOOT etc
void player_event(){
	
	if(event_type == EVENT_PUSH){
		
		// smashed by props ?
		if(you->obj_type == TYPE_ELEVATOR || you->obj_type == TYPE_DOOR || you->obj_type == TYPE_PLATFORM || you->obj_type == TYPE_SECRET_WALL){
			
			// get props structure
			PROPS *props = get_props(you);
			
			// get player's cct structure
			CCT *cct = get_cct(my);
			
			// push us away
			vec_set(&cct->push_force, vector(props->diff.x, props->diff.y, 0));
			
			// if we can perform check ?
			// means that door is almost closed (f.e.)
			if(you->obj_check == true){
				
				// perform check (if player is smashed or not)
				props_vs_npc_check(you, my, my->scale_x);
			}
		}
	}
	
	if(event_type == EVENT_SHOOT){
		
	}
	
	if(event_type == EVENT_SCAN){
		
	}
}

// handles all stuff related to after death life
void player_dead(ENTITY * ent){
	
	// reset lightrange
	ent->lightrange = 0;
	
	// no events from here on
	ent->emask &= ~(ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
	ent->event = NULL;
}

// handle all player's interactions
void player_interact(ENTITY * ent, CCT *cct){
	
	// when alive
	if(ent->obj_health > 0){
		
		// handle interaction traces
		ent_interact(ent, cct);
	}
}

void player_update(ENTITY * ent)
{
	CCT * cct = get_cct(ent);
	
	// for testing
	if(key_q){ ent->obj_health = -1; }
	DEBUG_VAR(ent->obj_health, 10);
	DEBUG_VAR(cct->air_underwater, 30);
	DEBUG_VAR(cct->falling_timer, 50);
	DEBUG_VAR(cct->soil_contact, 70);
	DEBUG_VAR(cct->is_grounded, 90);
	
	// update our health, armor etc
	// used globally - f.e. for gui
	hero->health = ent->obj_health;
	hero->armor = ent->obj_armor;
	hero->allow_movement = ent->obj_allow_move;
	cct->always_run = hero->always_run;
	
	// perform gravity trace
	ent_gravity_trace(ent, cct);
	
	// shake camera, if we are on moving props
	if(cct->ground_info == MOVING){ hero->cam->explo_power = 0.35; }
	
	// alive?
	
	if(ent->obj_health > 0){
		
		// allowed to move ?
		if(ent->obj_allow_move == 1){
			
			// set input keys and update every frame
			cct->forward = key_w;
			cct->backward = key_s;
			cct->straif_left = key_a;
			cct->straif_right = key_d;
			cct->run = key_shift;
			cct->jump = key_space;
			cct->dive = (key_ctrl || key_c);
			cct->interact = (key_e || mouse_right);
		}
		else{
			
			// stop all movement, cause we aren't allowed to move !
			ent_stop_movement(cct);
		}
	}
	else{
		
		// handle all stuff related to death
		// f.e. disable lightrange, stop sounds etc
		player_dead(ent);
		
		// count down
		ent->obj_timer -= time_frame / 16;
		
		// allow to reset level, as soon as movement has stoped and camera has lowered
		if(mouse_left && ent->obj_timer <= 0){
			
			// make sure that it's intentional
			ent->obj_state = -1;
			level_restart();
			return;
		}
	}
	
	// handle movement
	ent_movement(ent, cct);
	
	// handle state machine
	player_handle_state_machine(ent, cct);
	
	// handle sound effects
	player_sounds(ent, cct);
	
	// save our fake origin position
	vec_set(&cct->origin, vector(ent->x, ent->y, ent->z + 16));
	
	// attach and update camera
	camera_update(ent, cct);
	
	// interact with the world
	player_interact(ent, cct);
}

// main player's action, this one is used in WED
action player_controller()
{
	register_player_struct(my);
	CCT *cct = register_cct(my);

	set(my, TRANSLUCENT);
	c_setminmax(my);
	my->max_z = cct->bbox_z;
	my->min_z = -0.1;
	
	my->obj_health = 100;
	my->obj_armor = 0;
	my->obj_type = TYPE_PLAYER;
	my->obj_allow_move = true;
	my->obj_timer = 1; // 1 second delay, before retry
	
	my->group = PLAYER_GROUP;
	my->push = PLAYER_GROUP;
	
	my->emask |= (ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
	my->event = player_event;
}