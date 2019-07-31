
CCT_PHYSICS *register_cct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't register CCT, given entity doesn't exist");
		return;
	}
	
	CCT_PHYSICS *temp_cct = sys_malloc(sizeof(CCT_PHYSICS));
	init_cct(temp_cct);
	
	ent->obj_physics_struct = temp_cct;
	return temp_cct;
}

void init_cct(CCT_PHYSICS *cct){
	
	if(!cct){
		
		diag("\nERROR! Can't init cct, because it doesn't exist");
		return;
	}
	
	cct->bbox_x = 6;
	cct->bbox_y = 6;
	cct->bbox_z = 30;
	
	cct->foot_height = -cct_ground_z_offset;
	cct->foot_check_length = 1;
	cct->foot_step_height = 8;
	cct->soil_allow_sliding = 1;
	
	cct->jump_allowed = false;
	cct->jump_height = 16;
	
	cct->movement_speed = 5.5;
	cct->land_timer_limit = 4.5;
	cct->aqua_depth_factor = 1;
	cct->aqua_snd_water_out = true;
	cct->aqua_toxic_damage_timer_limit = 0.35;
	cct->aqua_toxic_damage_def = 25;
	
	cct->always_run = 0;
	
	cct->walk_speed_factor = 1;
	cct->run_speed_factor = 2;
}

CCT_PHYSICS *get_cct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't get CCT, given entity doesn't exist");
		return;
	}
	
	CCT_PHYSICS *temp_cct = ent->obj_physics_struct;
	return temp_cct;
}

void delete_cct(ENTITY *ent){
	
	CCT_PHYSICS *temp_cct = get_cct(ent);
	sys_free(temp_cct);
}

void cct_gravity_trace(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(!ent){
		diag("\nERROR! Can't create gravity trace for given entity, it doesn't exist!");
		return;
	}
	
	// start and end positions of the gravity trace
	VECTOR from, to, target_vec;
	vec_set(&from, vector(ent->x, ent->y, ent->z + cct_grav_trace_start_offset));
	vec_set(&to, &from);
	to.z -= cct_grav_trace_end_offset + physics->foot_height;
	
	// perform gravity trace
	vec_set(&ent->min_x, vector(-(physics->bbox_x - 1), -(physics->bbox_y - 1), -0.1));
	vec_set(&ent->max_x, vector((physics->bbox_x - 1), (physics->bbox_y - 1), 0.1));
	c_ignore(PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(&from, &to, TRACE_FLAGS | IGNORE_PUSH | SCAN_TEXTURE | USE_BOX);
	vec_set(&ent->min_x, vector(-physics->bbox_x, -physics->bbox_y, -0.1));
	vec_set(&ent->max_x, vector(physics->bbox_x, physics->bbox_y, physics->bbox_z));
	
	vec_fill(&physics->surface_normal, 0);
	vec_fill(&physics->surface_speed, 0);
	
	vec_set(&target_vec, &to);
	
	physics->ground_info = SOLID;
	
	if(HIT_TARGET){
		
		vec_set(&target_vec, &target);
		vec_set(&physics->surface_normal, &normal);
		
		// level block
		if(you == NULL){
			
			// we are on invisible slope ? 
			// (which is used for stairs, so no sliding on stairs)
			if(tex_flag1){ physics->ground_info = STAIRS; }
		}
		
		// models
		if(you != NULL){
			
			// if this entity can move us
			if(your->obj_move_npc == true){
				
				// then add it's XY speed to our movement
				// TODO !
				// here we need to get physics structure of YOU entity
				// then we need to take it's velocity and add it to our
				// surface speed !
				// physics->surface_speed.x = your->speed_x;
				// physics->surface_speed.y = your->speed_y;
				// Z speed is not necessary - this is done by the height adaption
			}
			
			if(your->obj_type == TYPE_ELEVATOR || your->obj_type == TYPE_PLATFORM){
				
				// TODO !
				// here we need to get physics structure of YOU entity
				// then we need to get it's current state and if it's MOVING
				// then we need to set our ground info to MOVING as well
				// this could be used f.e. to shake player's camera
				/*if(your->obj_state == 1 || your->obj_state == 2){
					physics->ground_info = MOVING;
				}*/
			}
		}
	}
	
	physics->soil_height = target_vec.z - physics->foot_height;
}

void ent_foot_push(VECTOR *input, VECTOR *surf_normal, var strength){
	
	input->x += surf_normal->x * (strength - vec_dot(&surf_normal->x, &input->x)) * time_step;
	input->y += surf_normal->y * (strength - vec_dot(&surf_normal->x, &input->x)) * time_step;
}

void ent_foot_push_from_normals(ENTITY *ent, CCT_PHYSICS *physics, VECTOR *hit_pos, VECTOR *surf_normal){
	
	var strength = 1;
	var ent_to_hit_distance = vec_dist(vector(ent->x, ent->y, 0), vector(hit_pos->x, hit_pos->y, 0));
	var ent_bbox_size = ent->max_x;
	
	if(ent_to_hit_distance < ent_bbox_size && physics->is_grounded == false){
		
		ent_foot_push(&physics->force, surf_normal, strength);
		ent_foot_push(&physics->speed, surf_normal, strength);
		ent_foot_push(&physics->dist, surf_normal, strength);
		ent_foot_push(&physics->push_force, surf_normal, strength);	
	}
}

void ent_foot_stop_velocity(var is_x_force, ENTITY *ent, CCT_PHYSICS *physics, VECTOR *hit_pos){
	
	if(is_x_force == true){
		
		if(hit_pos->x < ent->x){
			
			physics->force.x = maxv(physics->force.x, 0);
			physics->speed.x = maxv(physics->speed.x, 0);
			physics->dist.x = maxv(physics->dist.x, 0);
			physics->push_force.x = maxv(physics->push_force.x, 0);
		}
		else{
			
			physics->force.x = minv(physics->force.x, 0);
			physics->speed.x = minv(physics->speed.x, 0);
			physics->dist.x = minv(physics->dist.x, 0);
			physics->push_force.x = minv(physics->push_force.x, 0);
		}
	}
	
	if(is_x_force == false){
		
		if(hit_pos->y < ent->y){
			
			physics->force.y = maxv(physics->force.y, 0);
			physics->speed.y = maxv(physics->speed.y, 0);
			physics->dist.y = maxv(physics->dist.y, 0);
			physics->push_force.y = maxv(physics->push_force.y, 0);
		}
		else{
			
			physics->force.y = minv(physics->force.y, 0);
			physics->speed.y = minv(physics->speed.y, 0);
			physics->dist.y = minv(physics->dist.y, 0);
			physics->push_force.y = minv(physics->push_force.y, 0);
		}
	}
}

void ent_foot_check(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(!ent){
		
		diag("\nERROR! Can't perform foot check, entity doesn't exist!");
		return;
	}
	
	if(!physics){
		
		diag("\nERROR! Can't perform foot check, cct (physics) doesn't exist!");
		return;
	}
	
	VECTOR check_vec;
	
	// x coordinates
	vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
	check_vec.x += sign(physics->force.x) + (physics->foot_check_length * sign(physics->force.x));
	
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, TRACE_FLAGS | IGNORE_YOU | USE_BOX);
	
	if(HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > physics->foot_step_height){
		
		ent_foot_stop_velocity(true, ent, physics, &hit->x);
		ent_foot_push_from_normals(ent, physics, &target, &normal);
	}
	
	// y coordinates
	vec_set(&check_vec, vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset));
	check_vec.y += sign(physics->force.y) + (physics->foot_check_length * sign(physics->force.y));
	
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(vector(ent->x, ent->y, ent->z - cct_grav_trace_start_offset), &check_vec, TRACE_FLAGS | IGNORE_YOU | USE_BOX);
	
	if(HIT_TARGET && abs(normal.z) < cct_slope_fac && (hit->z - (ent->z + ent->min_z)) > physics->foot_step_height){
		
		ent_foot_stop_velocity(false, ent, physics, &hit->x);
		ent_foot_push_from_normals(ent, physics, &target, &normal);
	}
	
	// different step height in air and on ground
	if(physics->is_grounded == true){
		
		physics->foot_step_height = cct_def_foot_step_height;
	}
	else{
		
		physics->foot_step_height = 0;
	}
}

void ent_state_machine(ENTITY *ent, CCT_PHYSICS *physics){
	
	// not in pain, or death or shooting state
	if(ent->obj_state != PAIN && ent->obj_state != DEATH && ent->obj_state != SHOOT){
		
		// player jumped ?
		if(physics->is_grounded == false && physics->jump_allowed == false){
			
			ent->obj_state = JUMPED;
			
			// if in air for way too long, then switch to IN AIR state
			if(physics->falling_timer > 0.5){
				
				ent->obj_state = IN_AIR;
			}
		}
		else{
			
			// on ground
			if(physics->is_grounded == true){
				
				if(ent->obj_state == LANDED){
					
					physics->land_timer -= (physics->land_timer - 0) * 0.5 * time_step;
					
					if(physics->land_timer <= 0.5){
						
						ent->obj_state = IDLE;
						physics->land_timer = 0;
					}
				}
				else{
					
					// if we are moving
					if(physics->is_moving == true){
						
						// handle WALK and RUN states
						// according to "always_run"
						if(physics->always_run == true){
							
							if(physics->run == true){
								
								ent->obj_state = WALK;
							}
							else{
								
								ent->obj_state = RUN;
							}
						}
						else{
							
							if(physics->run == true){
								
								ent->obj_state = RUN;
							}
							else{
								
								ent->obj_state = WALK;
							}
						}
					}
					else{
						
						ent->obj_state = IDLE;
					}
				}
			}
		}
	}
}

void ent_set_land_state(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(ent->obj_state != PAIN && ent->obj_state != DEATH && ent->obj_state != SHOOT){
		
		if(physics->falling_timer > physics->land_timer_limit){
			
			if(physics->aqua_state == WADE){
				
				if(snd_playing(physics->aqua_snd_handle)){ snd_stop(physics->aqua_snd_handle); }
				
				// we need to handle sound playing differently for player and other npcs
				// since for npcs we need to play 3d sound and for player not
				if(ent->obj_type == TYPE_PLAYER){
					
					snd_play(land_in_water_ogg, cct_player_land_volume, 0);
				}
				else{
					
					ent_playsound(ent, land_in_water_ogg, cct_land_volume);
				}
			}
			else if (physics->aqua_state == ABOVE){
				
				// we can play normal land sound here
			}
			
			ent->obj_state = LANDED;
			physics->land_timer = minv(physics->falling_timer * 6, 12);
		}
	}
}

void ent_toxic_water_damage(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(physics->aqua_state != ABOVE){
		
		physics->aqua_toxic_damage_timer += time_frame / 16;
		
		if(physics->aqua_toxic_damage_timer >= physics->aqua_toxic_damage_timer_limit){
			
			ent->obj_health -= physics->aqua_toxic_damage_def + random(5);
			ent->obj_death_type = TYPE_AQUA;
			ent->obj_pain_type = TYPE_AQUA;
			physics->aqua_toxic_damage_timer %= physics->aqua_toxic_damage_timer_limit;
		}
	}
	else{
		
		physics->aqua_toxic_damage_timer = 0;
	}
}

var is_cct_in_rect(ENTITY *cct, ENTITY *rect, var scale){
	
	if(!cct){
		
		diag("\nERROR! Can't perform cct_vs_rect check! CCT entity doesn't exist!");
		return;
	}
	
	if(!rect){
		
		diag("\nERROR! Can't perform cct_vs_rect check! Rect entity doesn't exist!");
		return;	
	}
	
	VECTOR pos;
	vec_diff(&pos, &cct->x, &rect->x);
	vec_rotateback(&pos, &rect->pan);
	vec_add(&pos, &rect->x);
	
	var delta_x = pos.x - maxv(rect->x + (rect->min_x * scale), minv(pos.x, rect->x + (rect->max_x * scale)));
	var delta_y = pos.y - maxv(rect->y + (rect->min_y * scale), minv(pos.y, rect->y + (rect->max_y * scale)));
	var rect_delta = (delta_x * delta_x) + (delta_y * delta_y);
	
	var cct_top = cct->z + (cct->max_z * scale);
	var cct_bottom = cct->z + (cct->min_z * scale);
	var cct_z_hit = false;
	
	if((rect->z + rect->min_z) - cct_top > 0 || cct_bottom - (rect->z + rect->max_z) > 0){
		
		cct_z_hit = true; 
	}
	
	if(rect_delta < (cct->max_x * cct->max_x) && cct_z_hit == false){
		
		return(true);
	}
	
	return(false);
}