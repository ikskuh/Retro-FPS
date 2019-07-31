
void player_toggle_run(){
	
	if(player_allow_movement == false || player_health <= 0){ return; }
	if(game_level_is_loaded == false){ return; }
	
	player_always_run += 1;
	player_always_run %= 2;
}

void player_event(){
	
	if(event_type == EVENT_PUSH){
		
	}
	
	if(event_type == EVENT_SHOOT){
		
	}
	
	if(event_type == EVENT_SCAN){
		
	}
}

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

action player_controller(){
	
	CCT_PHYSICS *physics = register_cct(my);
	register_camera(my);
	
	c_setminmax(my);
	my->max_z = physics->bbox_z;
	my->min_z = -0.1;
	
	my->obj_health = 100;
	my->obj_armor = 0;
	my->obj_type = TYPE_PLAYER;
	my->obj_allow_move = true;
	
	my->group = PLAYER_GROUP;
	my->push = PLAYER_GROUP;
	
	my->emask |= (ENABLE_PUSH | ENABLE_SHOOT | ENABLE_SCAN);
	my->event = player_event;
	
	player_always_run = false;
	on_caps = player_toggle_run;
	
	while(my){
		
		// for testing
		if(key_q){ my->obj_health = -1; }
		
		// update our health, armor etc
		// used globally - f.e. for gui
		player_health = my->obj_health;
		player_armor = my->obj_armor;
		player_allow_movement = my->obj_allow_move;
		
		// perform gravity trace
		cct_gravity_trace(my, physics);
		
		// shake camera, if we are on moving props
		if(physics->ground_info == MOVING){ /*cam_explo_power = 0.35;*/ }
		
		// alive?
		if(my->obj_health > 0){
			
			// allowed to move?
			if(my->obj_allow_move == 1){
				
				// movement input from player
				physics->force.x = physics->movement_speed * (key_w - key_s) * physics->aqua_depth_factor;
				physics->force.y = physics->movement_speed * (key_a - key_d) * physics->aqua_depth_factor;
				
				// input for running and jumping
				physics->run = key_shift;
				physics->jump = key_space;
				
				// gravity + X and Y movement
				ent_movement(my, physics);
				
				// detect toxic water
				ent_detect_aqua(my, physics);
			}
			else{
				
				// stop all movement, cause we aren't allowed to move !
				ent_stop_movement(physics);
			}
		}
		else{
			
			// player does still move after death
			// well.. at least his corpse does
			// gravity + pushing forces
			ent_movement(my, physics);
			
			// handle all stuff related to death
			// f.e. disable lightrange, stop sounds etc
			player_dead();
		}
		
		// save our fake origin position
		vec_set(&physics->origin, vector(my->x, my->y, my->z + 16));
		
		// attach and update camera
		camera_update(my, physics);
		
		wait(1);
	}
}