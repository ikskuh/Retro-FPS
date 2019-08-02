
// register and initializes new cct (character controller)
// and save it's pointer into given entities obj_cct_struct skill
CCT *register_cct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't register CCT, given entity doesn't exist");
		return;
	}
	
	CCT *cct = sys_malloc(sizeof(CCT));
	init_cct(cct);
	
	ent->obj_cct_struct = cct;
	return cct;
}

// initialize all major variables for the given cct
// this function called inside of register_cct right after creating new cct
void init_cct(CCT *cct){
	
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
	
	cct->movement_speed = 4.5;
	cct->swim_speed = 2;
	cct->falling_damage_limit = cct_fall_damage_limit;
	cct->land_timer_limit = 4.5;
	
	cct->always_run = 0;
	
	cct->walk_speed_factor = 1;
	cct->run_speed_factor = 2;
	cct->water_depth_factor = 1;
	cct->water_out_switch = true;
	cct->air_underwater = 100;
	cct->air_underwater_timer = 2;
	cct->air_underwater_timer_limit = 2;
}

// returns pointer to the cct from given entity's obj_cct_struct skill
CCT *get_cct(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't get CCT, given entity doesn't exist");
		return;
	}
	
	CCT *cct = ent->obj_cct_struct;
	return cct;
}

// removes cct from the memory
// pointer is taken from given entity's obj_cct_struct skill
void delete_cct(ENTITY *ent){
	
	CCT *cct = get_cct(ent);
	sys_free(cct);
}

// remove given cct from the memory
void delete_cct(CCT *cct){
	
	sys_free(cct);
}