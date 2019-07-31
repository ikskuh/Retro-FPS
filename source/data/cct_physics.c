
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
	c_ignore(PLAYER_GROUP, SWITCH_ITEM_GROUP, 0);
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