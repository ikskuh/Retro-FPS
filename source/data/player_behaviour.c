
void player_toggle_run(){
	
	if(player_allow_movement == false || player_health <= 0){ return; }
	if(game_level_is_loaded == false){ return; }
	
	player_always_run += 1;
	player_always_run %= 2;
}

void player_sounds(CCT_PHYSICS *physics){
	
	// if we are dead
	if(my->obj_state == DEATH){
		
		if(player_death_snd_switch == 0){
			
			if(snd_playing(player_snd_handle)){ snd_stop(player_snd_handle); }
			var rnd = integer(random(3));
			if(rnd == 0){ player_snd_handle = snd_play(player_death_01_ogg, player_snd_volume, 0); }
			if(rnd == 1){ player_snd_handle = snd_play(player_death_02_ogg, player_snd_volume, 0); }
			if(rnd == 2){ player_snd_handle = snd_play(player_death_03_ogg, player_snd_volume, 0); }
			
			player_death_snd_switch = 1;
		}
		
		// if camera got under the water
		// we need to stop all previous sounds, and play death to underwater sound effect
		if(region_check(reg_toxic_aqua_str, &camera->x, &camera->x) && snd_playing(player_snd_handle) && player_death_snd_switch == 1){
			
			if(snd_playing(player_snd_handle)){ snd_stop(player_snd_handle); }
			player_snd_handle = snd_play(player_death_to_underwater_ogg, player_snd_volume, 0);
			player_death_snd_switch = -1;
		}
	}
	else{
		
		// we are alive, so this means, that we can make some sounds ... 
		// handle underwater and above water pain sounds differently
		if(physics->aqua_state == UNDERWATER){
			
			// damage from all stuff is the same
			if(snd_playing(player_snd_handle)){ snd_stop(player_snd_handle); }
			var rnd = integer(random(2));
			if(rnd == 0){ player_snd_handle = snd_play(player_drown_01_ogg, player_snd_volume, 0); }
			if(rnd == 1){ player_snd_handle = snd_play(player_drown_02_ogg, player_snd_volume, 0); }
		}
		else{
			
			// if we are above the water line... 
			// damage from toxic water ?
			if(my->obj_pain_type == TYPE_AQUA){
				
				if(snd_playing(player_snd_handle)){ snd_stop(player_snd_handle); }
				var rnd = integer(random(2));
				if(rnd == 0){ player_snd_handle = snd_play(player_burn_01_ogg, player_snd_volume, 0); }
				if(rnd == 1){ player_snd_handle = snd_play(player_burn_02_ogg, player_snd_volume, 0); }
			}
		}
	}
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
	
	// we are officially dead now
	my->obj_state = DEATH;
	
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
	on_r = level_restart;
	
	// reset some variables
	player_death_snd_switch = 0;
	
	while(my){
		
		// for testing
		if(key_q){ my->obj_health = -1; }
		DEBUG_VAR(my->obj_health, 10);
		
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
				
				// handle state machine
				ent_state_machine(my, physics);
				
				// gravity + X and Y movement
				ent_movement(my, physics);
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
			
			// allow to reset level, as soon as movement has stoped and camera has lowered
			if(mouse_left && player_cam->cam_height <= -13){ break; }
		}
		
		// detect toxic water and handle sound effects
		ent_aqua(my, physics);
		
		// handle sound effects
		player_sounds(physics);
		
		// save our fake origin position
		vec_set(&physics->origin, vector(my->x, my->y, my->z + 16));
		
		// attach and update camera
		camera_update(my, physics);
		
		wait(1);
	}
	
	level_restart();
}