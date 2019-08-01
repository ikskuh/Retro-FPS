
// perform gravity trace for the given entity with the given cct
void ent_gravity_trace(ENTITY *ent, CCT *cct){
	
	if(!ent){
		diag("\nERROR! Can't create gravity trace for given entity, it doesn't exist!");
		return;
	}
	
	// start and end positions of the gravity trace
	VECTOR from, to, target_vec;
	vec_set(&from, vector(ent->x, ent->y, ent->z + cct_grav_trace_start_offset));
	vec_set(&to, &from);
	to.z -= cct_grav_trace_end_offset + cct->foot_height;
	
	// perform gravity trace
	vec_set(&ent->min_x, vector(-(cct->bbox_x - 1), -(cct->bbox_y - 1), -0.1));
	vec_set(&ent->max_x, vector((cct->bbox_x - 1), (cct->bbox_y - 1), 0.1));
	c_ignore(PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_trace(&from, &to, TRACE_FLAGS | IGNORE_PUSH | SCAN_TEXTURE | USE_BOX);
	vec_set(&ent->min_x, vector(-cct->bbox_x, -cct->bbox_y, -0.1));
	vec_set(&ent->max_x, vector(cct->bbox_x, cct->bbox_y, cct->bbox_z));
	
	vec_fill(&cct->surface_normal, 0);
	vec_fill(&cct->surface_speed, 0);
	
	vec_set(&target_vec, &to);
	
	cct->ground_info = SOLID;
	
	if(HIT_TARGET){
		
		vec_set(&target_vec, &target);
		vec_set(&cct->surface_normal, &normal);
		
		// level block
		if(you == NULL){
			
			// we are on invisible slope ? 
			// (which is used for stairs, so no sliding on stairs)
			if(tex_flag1){ cct->ground_info = STAIRS; }
		}
		
		// models
		if(you != NULL){
			
			// if this entity can move us
			if(your->obj_move_npc == true){
				
				// then add it's XY speed to our movement
				// TODO !
				// here we need to get structure of YOU entity
				// then we need to take it's velocity and add it to our
				// surface speed !
				// cct->surface_speed.x = your->speed_x;
				// cct->surface_speed.y = your->speed_y;
				// Z speed is not necessary - this is done by the height adaption
			}
			
			if(your->obj_type == TYPE_ELEVATOR || your->obj_type == TYPE_PLATFORM){
				
				// TODO !
				// here we need to get structure of YOU entity
				// then we need to get it's current state and if it's MOVING
				// then we need to set our ground info to MOVING as well
				// this could be used f.e. to shake player's camera
				/*if(your->obj_state == 1 || your->obj_state == 2){
					cct->ground_info = MOVING;
				}*/
			}
		}
	}
	
	cct->soil_height = target_vec.z - cct->foot_height;
}