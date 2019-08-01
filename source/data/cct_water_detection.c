
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
	
	var trace_length = 4;
	
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
	cct->water_out_trace_top.z += cct->bbox_z + 4;
	
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
}