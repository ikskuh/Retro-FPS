
// handle all gravity movement when out of the water
void ent_vertical_movement(ENTITY *ent, CCT *cct){
	
	if(ent->z > cct->soil_height + (5 + 20 * cct->is_grounded) * time_step || cct->force.z > 0){
		
		cct->is_grounded = false;
		cct->force.z = maxv(cct->force.z - cct_gravity * time_step, -cct_gravity_max);
	}
	else{
		
		c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		c_move(ent, nullvector, vector(0, 0, cct->soil_height - ent->z), MOVE_FLAGS | IGNORE_YOU | GLIDE);
		cct->is_grounded = true;
		cct->force.z = 0;		
	}
	
	if(cct->is_grounded == true){
		
		if(cct->jump == true){
			
			if(cct->jump_allowed == true){
				
				cct->force.z = cct->jump_height;
				cct->jump_allowed = false;
				
			}
		}
		else{ cct->jump_allowed = true; }
		
	}
	
	if(cct->force.z){
		
		c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		c_move(ent, nullvector, vector(0, 0, maxv(cct->force.z * time_step, cct->soil_height - ent->z)), MOVE_FLAGS | IGNORE_YOU | GLIDE);
		
		// hit the ceiling ?
		// then disable jumping forces !
		if(HIT_TARGET && normal.z < -0.5){ cct->force.z = minv(cct->force.z, 0); }
	}
}

// handle all horizontal (XY) movement when out of the water
void ent_horizontal_movement(ENTITY *ent, CCT *cct){
	
	// if object is dead, stop all input forces
	if(ent->obj_health <= 0){
		
		cct->force.x = 0;
		cct->force.y = 0;
	}
	
	// limit max movement speed (XY)
	// means DON'T ALLOW to cheat, by holding both movement keys
	// f.e. forward and left (or right)
	if(vec_length(vector(cct->force.x, cct->force.y, 0)) > cct->movement_speed){
		
		var len = sqrt(cct->force.x * cct->force.x + cct->force.y * cct->force.y);
		cct->force.x *= ((cct->movement_speed) / len);
		cct->force.y *= ((cct->movement_speed) / len);
	}
	
	// handle running and 'always running'
	if(cct->always_run == true){
		
		if(cct->run == true){
			
			cct->force.x *= cct->walk_speed_factor;
			cct->force.y *= cct->walk_speed_factor;
		}
		else{
			
			cct->force.x *= cct->run_speed_factor;
			cct->force.y *= cct->run_speed_factor;
		}
	}
	else{
		
		if(cct->run == true){
			
			cct->force.x *= cct->run_speed_factor;
			cct->force.y *= cct->run_speed_factor;
		}
		else{
			
			cct->force.x *= cct->walk_speed_factor;
			cct->force.y *= cct->walk_speed_factor;
		}
	}
	
	// if this is a player ?
	// and he is alive ...
	if(ent->obj_type == TYPE_PLAYER && ent->obj_health > 0){
		
		// rotate input with camera PAN angle (input)
		vec_rotate(&cct->force, vector(camera->pan, 0, 0));
	}
	
	// first, decide whether the actor is standing on the floor or not
	if(cct->is_grounded == true){
		
		// reset falling time
		cct->falling_timer = 0;
		
		// ground friction
		cct->friction = cct_gnd_fric;
		
		// slide down on slopes
		ent_slide_on_slopes(ent, cct);
	}
	else{
		
		// airborne - reduce all relative forces
		// to prevent him from jumping or further moving in the air
		cct->friction = cct_air_fric;
		
		cct->force.x *= cct_reduce_input_in_air_factor; // don't set the force completely to zero, otherwise
		cct->force.y *= cct_reduce_input_in_air_factor; // player could be stuck on top of a non-wmb entity
		
		// reset absolute forces
		vec_fill(&cct->abs_force, 0);
		
		// add falling time, only falling if moving downward
		if((cct->force.z <= 0)){ cct->falling_timer += time_step; }
	}
	
	// foot check here
	ent_foot_check(ent, cct);
	
	// accelerate the entity relative speed by the force
	var fric = maxv((1 - time_step * cct->friction), 0);
	cct->velocity.x = (time_step * cct->force.x) + (fric * cct->velocity.x);
	cct->velocity.y = (time_step * cct->force.y) + (fric * cct->velocity.y);
	cct->velocity.z = 0;
	
	// calculate relative distances to move
	cct->dist.x = cct->velocity.x * time_step;
	cct->dist.y = cct->velocity.y * time_step;
	cct->dist.z = 0;
	
	// calculate absolute distance to move
	cct->abs_dist.x = cct->abs_force.x * time_step;
	cct->abs_dist.y = cct->abs_force.y * time_step;
	cct->abs_dist.z = cct->abs_force.z * time_step;
	
	// add the speed given by the ground elasticity and the jumping force
	if(cct->is_grounded == true){
		
		// if the actor is standing on a moving platform, add it's horizontal displacement
		cct->abs_dist.x += cct->surface_speed.x;
		cct->abs_dist.y += cct->surface_speed.y;
	}
	
	// push forces from props
	if(vec_length(vector(cct->push_velocity.x, cct->push_velocity.y, 0)) > 0){
		
		cct->abs_dist.x += cct->push_velocity.x;
		cct->abs_dist.y += cct->push_velocity.y;
		
		vec_fill(&cct->push_velocity, 0);
	}
	
	// relative distance
	move_friction = 0;
	c_ignore(PLAYER_GROUP, PATHFIND_GROUP, 0);
	cct->moving_distance = c_move(ent, nullvector, &cct->dist, MOVE_FLAGS | IGNORE_PUSH | IGNORE_YOU | GLIDE);
	
	if(cct->moving_distance > 0.5){
		
		cct->moving_speed = cct->moving_distance / time_step;
		cct->is_moving = true;
	}
	else{
		
		cct->moving_speed = 0;
		cct->is_moving = false;
	}
	
	// absolute distance
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_move(ent, nullvector, &cct->abs_dist, MOVE_FLAGS | IGNORE_YOU | GLIDE);
}

// handle all the movement
void ent_movement(ENTITY *ent, CCT *cct){
	
	// gravity
	ent_vertical_movement(ent, cct);
	
	// x-y movement
	ent_horizontal_movement(ent, cct);
}

// slide on steep slopes, if we are allowed to
void ent_slide_on_slopes(ENTITY *ent, CCT *cct){
	
	// we aren't standing on stairs and we are allowed to slide down ?
	if(cct->ground_info != STAIRS && cct->soil_allow_sliding == true){
		
		// reset absolute forces slowly (don't stop right after the slope ends)
		cct->abs_force.x -= (cct->abs_force.x - 0) * 0.25 * time_step;
		cct->abs_force.y -= (cct->abs_force.y - 0) * 0.25 * time_step;
		cct->abs_force.z = 0;
	}
	else{
		
		// reset absolute forces
		vec_fill(&cct->abs_force, 0);
	}
	
	// if slope is too steep
	if(cct->surface_normal.z < 0.9){
		
		// reduce ahead force because player force it is now deflected upwards
		cct->force.x *= cct->surface_normal.z;
		cct->force.y *= cct->surface_normal.z;
		
		// only if we are on slopes
		if(cct->ground_info != STAIRS && cct->soil_allow_sliding == true){
			
			// gravity draws him down the slope
			cct->abs_force.x = cct->surface_normal.x * cct_gravity * 2 * cct_slope_fac;
			cct->abs_force.y = cct->surface_normal.y * cct_gravity * 2 * cct_slope_fac;	
		}
	}
}

// stop all movement for the given cct
void ent_stop_movement(CCT *cct){
	
	vec_fill(&cct->force, 0);
	vec_fill(&cct->abs_force, 0);
	vec_fill(&cct->dist, 0);
	vec_fill(&cct->abs_dist, 0);
	vec_fill(&cct->speed, 0);
	vec_fill(&cct->velocity, 0);
	vec_fill(&cct->push_force, 0);
	vec_fill(&cct->push_velocity, 0);

	cct->is_moving = 0;
	cct->moving_distance = 0;
	cct->moving_speed = 0;	
}