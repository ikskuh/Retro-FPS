
// detect water regions and change our water movement factor, if we are in water
void ent_detect_water(ENTITY *ent, CCT *cct){
	
	cct->water_reg_total = 0;
	cct->water_depth_factor = 1;
	
	if(cct->water_reg_total == 0){
		
		cct->water_origin_diff_z = 0;
		cct->water_z_height = 0;
	}
	
	while(region_get(reg_water_str, cct->water_reg_total, &cct->water_reg_min, &cct->water_reg_max) != 0){
		
		cct->water_reg_total += 1;
		
		if(ent->x < cct->water_reg_min.x || ent->x > cct->water_reg_max.x){ continue; }
		if(ent->y < cct->water_reg_min.y || ent->y > cct->water_reg_max.y){ continue; }
		if((cct->water_reg_min.z - ent->z) > 0 || (ent->z - cct->water_reg_max.z) > 0){ continue; }
		
		cct->water_origin_diff_z = cct->water_reg_max.z - ent->z;
		cct->water_z_height = cct->water_reg_max.z;
		cct->water_depth_factor = clamp(1 - (cct->water_origin_diff_z / 25), 0.1, 1);	
	}
}

// set water state for the given cct
void ent_set_water_state(CCT *cct){
	
	// if we are in water
	if(cct->water_origin_diff_z > 0 && cct->water_origin_diff_z < (cct->bbox_z / 2)){
		
		cct->water_state = ON_WATER;
	} 
	else{
		
		if(cct->water_origin_diff_z > (cct->bbox_z / 2)){
			
			cct->water_state = IN_WATER;
		} 
		else{
			
			cct->water_state = OUT_OF_WATER;
		}
	}
}

// find surface to hop onto
void ent_detect_water_edge(ENTITY *ent, CCT *cct){
	
	// only if we are in water !
	// otherwise, player will be able to climb edges :D
	if(cct->water_state < IN_WATER && cct_allow_cheap_edge_climb == false){ return; }
	
	var trace_length = cct->bbox_x;
	
	vec_set(&cct->water_out_trace_mid, vector(cct->bbox_x + trace_length, 0, 0));
	
	// if player ?
	if(ent->obj_type == TYPE_PLAYER){
		
		// rotate positions with camera PAN angle
		vec_rotate(&cct->water_out_trace_mid, vector(camera->pan, 0, 0));
	}
	else{
		
		// for npcs rotate it with their bbox PAN angle
		vec_rotate(&cct->water_out_trace_mid, vector(ent->pan, 0, 0));
	}
	
	vec_add(&cct->water_out_trace_mid, &ent->x);
	vec_set(&cct->water_out_trace_top, &cct->water_out_trace_mid);
	cct->water_out_trace_top.z += cct->bbox_z;
	
	c_ignore(PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(&ent->x, &cct->water_out_trace_mid, TRACE_FLAGS | IGNORE_PUSH);
	cct->water_mid_trace_result = false;
	if(HIT_TARGET){ cct->water_mid_trace_result = true; }
	
	c_ignore(PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(vector(ent->x, ent->y, cct->water_out_trace_top.z), &cct->water_out_trace_top, TRACE_FLAGS | IGNORE_PUSH);
	cct->water_top_trace_result = false;
	if(HIT_TARGET){ cct->water_top_trace_result = true; }
	
	// logic is pretty simple, if top trace doesn't hit anything, while the middle one does
	// then it means that we can try to hop onto it
	cct->jump_out_of_water = false;
	if(cct->water_mid_trace_result == true && cct->water_top_trace_result == false){
		
		cct->jump_out_of_water = true;
	}
	
	// and for npcs, we need to force them to jump up
	if(ent->obj_type != TYPE_PLAYER){
		
		cct->jump = true;
	}
}

// detect if entity is inside of the water region, or not
void ent_detect_water_state(ENTITY *ent, CCT *cct){
	
	// perform only if wasn't smashed !
	if(ent->obj_death_type == TYPE_SMASHED){ return; }
	
	// detect water region
	ent_detect_water(ent, cct);
	
	// detect surface to hop onto
	ent_detect_water_edge(ent, cct);
	
	// if player ?
	if(ent->obj_type == TYPE_PLAYER){
		
		// had to make this dirty trick with camera
		// to avoid 'no fog' thing when just entered water
		VECTOR temp_camera;
		vec_set(&temp_camera, vector(camera->x, camera->y, camera->z - 0.5));
		
		// camera is inside of the water ?
		// then our head is in water !
		if(region_check(reg_water_str, &temp_camera, &temp_camera)){
			
			cct->water_state = HEAD_IN_WATER;
		}
		else{
			
			// handle all other states as usual
			ent_set_water_state(cct);
		}
		
	}
	else{
		
		// handle all other states as usual
		ent_set_water_state(cct);
	}
	
	// handle water sounds
	ent_water_interaction_sounds(ent, cct);
	
	// handle underwater air (breathing), only if we are alive
	if(ent->obj_health > 0){
		
		// handle breathing underwater
		ent_air_underwater(ent, cct);
	}
}

// handle sounds when player get in or out of the water
void ent_water_interaction_sounds(ENTITY *ent, CCT *cct){
	
	// handle surfacing sounds
	ent_water_surface_sound(ent, cct);
	
	// if we are swimming
	if(cct->water_state >= IN_WATER){
		
		// if npc ?
		if(ent->obj_type != TYPE_PLAYER){
			
			// stop all voice sounds when in water
			if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
		}
		
		// we jumped into the water?
		if(cct->falling_timer > 1 && cct->water_in_switch == false){
			
			// play water splash sound
			if(ent->obj_type == TYPE_PLAYER){
				
				snd_play(water_in_ogg, player_snd_volume, 0);			
			}
			else{
				
				ent_playsound(ent, water_in_ogg, cct_snd_volume);
			}
			
			// add some bubbles under the water
			// add water splash fx on water plane
			var bubble = 0;
			for(bubble = 0; bubble < 64; bubble++){
				
				VECTOR temp;
				temp.x = cct->origin.x + 32 - random(64);
				temp.y = cct->origin.y + 32 - random(64);
				temp.z = cct->origin.z - 32 + (64 - random(128));
				effect(bubbles_particle, 1, &temp, nullvector);
			}
			
			ent->obj_water_height = cct->water_z_height;
			cct->water_in_switch = true;
		}
		
		// we walked into the water slowly
		// so play in water slowly sound here
		if(cct->falling_timer < 1 && cct->water_in_switch == false){
			
			if(ent->obj_type == TYPE_PLAYER){
				
				snd_play(water_in_slow_ogg, player_snd_volume, 0);			
			}
			else{
				
				ent_playsound(ent, water_in_slow_ogg, cct_snd_volume);
			}
			
			cct->water_in_switch = true;
		}
	}
	else{
		
		// we are out of the water and standing on surface ?
		if(cct->water_out_switch == false && cct->is_grounded == true){
			
			// play water splash sound
			if(ent->obj_type == TYPE_PLAYER){
				
				snd_play(water_out_ogg, player_snd_volume, 0);			
			}
			else{
				
				ent_playsound(ent, water_out_ogg, cct_snd_volume);
			}
			
			cct->water_out_switch = true;
		}
	}	
}

// handle water sounds, when entity is surfacing
void ent_water_surface_sound(ENTITY *ent, CCT *cct){
	
	if(cct->water_z_is_moving == true && cct->water_state == HEAD_IN_WATER){
		
		if(cct->water_z_snd_switch == 0 && cct->swim_z_abs_dist > 0){
			
			var rnd = integer(random(2));
			if(ent->obj_type == TYPE_PLAYER){
				
				if(rnd == 0){ snd_play(player_swim_01_ogg, player_snd_volume, 0); }
				if(rnd == 1){ snd_play(player_swim_02_ogg, player_snd_volume, 0); }
			}
			else{
				
			}
			
			cct->water_z_snd_switch = 1;
		}
		
		if(cct->swim_z_abs_dist < 0 && cct->water_z_snd_switch == 1){ cct->water_z_snd_switch = 0; }
	}
	else{ cct->water_z_snd_switch = 0; }
}

// handle breathing underwater, and if out of air, then take damage
void ent_air_underwater(ENTITY *ent, CCT *cct){
	
	// if our head is under water ?
	if(cct->water_state == HEAD_IN_WATER){
		
		// decrease air
		cct->air_underwater = clamp(cct->air_underwater -= 0.5 * time_step, 0, 100);
		
		// if no air, take damage
		if(cct->air_underwater <= 0){
			
			cct->air_underwater_timer += time_frame / 16;
			
			if(cct->air_underwater_timer >= cct->air_underwater_timer_limit){
				
				VECTOR bubble_pos;
				vec_set(&bubble_pos, &ent->x);
				bubble_pos.z += cct->bbox_z;
				
				// play drown sound effects for player
				if(ent->obj_type == TYPE_PLAYER){
					
					if(snd_playing(my->obj_snd_handle)){ snd_stop(my->obj_snd_handle); }
					var rnd = integer(random(2));
					if(rnd == 0){ my->obj_snd_handle = snd_play(player_drown_01_ogg, player_snd_volume, 0); }
					if(rnd == 1){ my->obj_snd_handle = snd_play(player_drown_02_ogg, player_snd_volume, 0); }
				}
				
				int bubble = 0;
				for(bubble = 0; bubble < 16; bubble++){
					
					VECTOR temp;
					temp.x = bubble_pos.x + 16 - random(32);
					temp.y = bubble_pos.y + 16 - random(32);
					temp.z = bubble_pos.z + (8 - random(16));
					effect(bubbles_particle, 1, &temp, nullvector);
				}
				
				ent->obj_pain_type = TYPE_OUT_OF_AIR;
				ent->obj_health -= cct_underwater_no_air_damage + random(5);
				cct->air_underwater_timer %= cct->air_underwater_timer_limit;
			}
		}
		
		// allow to play gasp sounds
		if(cct->air_underwater < 75 && cct->air_underwater_snd_gasp_switch == 1){
			
			// out of water!
			cct->air_underwater_snd_gasp_switch = 0;
		}
	}
	else{
		
		// increase air when out of water
		cct->air_underwater = clamp(cct->air_underwater += 2 * time_step, 0, 100);
		
		// play gasp sounds here, if this is player
		if(ent->obj_type == TYPE_PLAYER){
			
			if(cct->air_underwater < 75 && cct->air_underwater_snd_gasp_switch == 0){
				
				if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
				
				if(cct->air_underwater > 25){
					
					ent->obj_snd_handle = snd_play(player_gasp_01_ogg, player_snd_volume, 0);
				}
				else{
					
					ent->obj_snd_handle = snd_play(player_gasp_02_ogg, player_snd_volume, 0);
				}
				
				cct->air_underwater_snd_gasp_switch = 1;
			}
		}
	}
}