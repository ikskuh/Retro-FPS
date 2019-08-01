
// toggle auto run mode on and off
void player_toggle_run(){
	
	if(hero->allow_movement == false || hero->health <= 0){ return; }
	if(game_level_is_loaded == false){ return; }
	
	hero->always_run += 1;
	hero->always_run %= 2;
}

// handles all voice sounds made by player
void player_sounds(CCT *cct){
	
}

// handles all events that player receives
// such as EVENT_PUSH, EVENT_SCAN, EVENT_SHOOT etc
void player_event(){
	
	if(event_type == EVENT_PUSH){
		
	}
	
	if(event_type == EVENT_SHOOT){
		
	}
	
	if(event_type == EVENT_SCAN){
		
	}
}

// handles all stuff related to after death life
void player_dead(){
	
	// reset lightrange
	my->lightrange = 0;
	
	// no events from here on
	my->emask &= ~(ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
	my->event = NULL;
	
	// we aren't allowed to move
	// because we are dead x_x
	my->obj_allow_move = 0;
}

// main player's action, this one is used in WED
action player_controller(){

	CCT *cct = register_cct(my);
	register_player_struct(my);
	
	c_setminmax(my);
	my->max_z = cct->bbox_z;
	my->min_z = -0.1;
	
	my->obj_health = 100;
	my->obj_armor = 0;
	my->obj_type = TYPE_PLAYER;
	my->obj_allow_move = true;
	
	my->group = PLAYER_GROUP;
	my->push = PLAYER_GROUP;
	
	my->emask |= (ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
	my->event = player_event;
	
	on_caps = player_toggle_run;
	on_r = level_restart;
	
	while(my){
		
		// for testing
		if(key_q){ my->obj_health = -1; }
		DEBUG_VAR(my->obj_health, 10);
		
		// update our health, armor etc
		// used globally - f.e. for gui
		hero->health = my->obj_health;
		hero->armor = my->obj_armor;
		hero->allow_movement = my->obj_allow_move;
		
		// perform gravity trace
		ent_gravity_trace(my, cct);
		
		// shake camera, if we are on moving props
		if(cct->ground_info == MOVING){ /*cam_explo_power = 0.35;*/ }
		
		// alive?
		if(my->obj_health > 0){
			
			// allowed to move?
			if(my->obj_allow_move == 1){
				
				// movement input from player
				cct->force.x = cct->movement_speed * (key_w - key_s) * cct->water_depth_factor;
				cct->force.y = cct->movement_speed * (key_a - key_d) * cct->water_depth_factor;
				
				// input for running and jumping
				cct->run = key_shift;
				cct->jump = key_space;
				
				// gravity + X and Y movement
				ent_movement(my, cct);
			}
			else{
				
				// stop all movement, cause we aren't allowed to move !
				ent_stop_movement(cct);
			}
		}
		else{
			
			// player does still move after death
			// well.. at least his corpse does
			// gravity + pushing forces
			ent_movement(my, cct);
			
			// handle all stuff related to death
			// f.e. disable lightrange, stop sounds etc
			player_dead();
			
			// allow to reset level, as soon as movement has stoped and camera has lowered
			if(mouse_left && hero->cam->height <= -13){
				
				// make sure that it's intentional
				my->obj_state = -1;
				break;
			}
		}
		
		// handle sound effects
		player_sounds(cct);
		
		// save our fake origin position
		vec_set(&cct->origin, vector(my->x, my->y, my->z + 16));
		
		// attach and update camera
		camera_update(my, cct);
		
		wait(1);
	}
	
	// if we wated to restart, then do so!
	if(my->obj_state == -1){
		
		level_restart();
	}
}