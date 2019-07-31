
void ent_vertical_movement(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(ent->z > physics->soil_height + (5 + 20 * physics->is_grounded) * time_step || physics->force.z > 0){
		
		physics->is_grounded = false;
		physics->force.z = maxv(physics->force.z - cct_gravity * time_step, -cct_gravity_max);
	}
	else{
		
		c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		c_move(ent, nullvector, vector(0, 0, physics->soil_height - ent->z), MOVE_FLAGS | IGNORE_YOU | GLIDE);
		physics->is_grounded = true;
		physics->force.z = 0;		
	}
	
	if(physics->is_grounded == true){
		
		if(physics->jump == true){
			
			if(physics->jump_allowed == true){
				
				physics->force.z = physics->jump_height;
				physics->jump_allowed = false;
				
			}
		}
		else{ physics->jump_allowed = true; }
		
	}
	
	if(physics->force.z){
		
		c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
		c_move(ent, nullvector, vector(0, 0, maxv(physics->force.z * time_step, physics->soil_height - ent->z)), MOVE_FLAGS | IGNORE_YOU | GLIDE);
		
		// hit the ceiling ?
		// then disable jumping forces !
		if(HIT_TARGET && normal.z < -0.5){ physics->force.z = minv(physics->force.z, 0); }
	}
}

void ent_horizontal_movement(ENTITY *ent, CCT_PHYSICS *physics){
	
	// if object is dead, stop all input forces
	if(ent->obj_health <= 0){
		
		physics->force.x = 0;
		physics->force.y = 0;
	}
	
	// limit max movement speed (XY)
	// means DON'T ALLOW to cheat, by holding both movement keys
	// f.e. forward and left (or right)
	if(vec_length(vector(physics->force.x, physics->force.y, 0)) > physics->movement_speed){
		
		var len = sqrt(physics->force.x * physics->force.x + physics->force.y * physics->force.y);
		physics->force.x *= ((physics->movement_speed) / len);
		physics->force.y *= ((physics->movement_speed) / len);
	}
	
	// handle running and 'always running'
	if(physics->always_run == true){
		
		if(physics->run == true){
			
			physics->force.x *= physics->walk_speed_factor;
			physics->force.y *= physics->walk_speed_factor;
		}
		else{
			
			physics->force.x *= physics->run_speed_factor;
			physics->force.y *= physics->run_speed_factor;
		}
	}
	else{
		
		if(physics->run == true){
			
			physics->force.x *= physics->run_speed_factor;
			physics->force.y *= physics->run_speed_factor;
		}
		else{
			
			physics->force.x *= physics->walk_speed_factor;
			physics->force.y *= physics->walk_speed_factor;
		}
	}
	
	// if this is a player ?
	// and he is alive ...
	if(ent->obj_type == TYPE_PLAYER && ent->obj_health > 0){
		
		// rotate input with camera PAN angle (input)
		vec_rotate(&physics->force, vector(camera->pan, 0, 0));
	}
	
	// first, decide whether the actor is standing on the floor or not
	if(physics->is_grounded == true){
		
		// handle landing state
		ent_set_land_state(ent, physics);
		
		// reset falling time
		physics->falling_timer = 0;
		
		// ground friction
		physics->friction = cct_gnd_fric;
		
		// slide down on slopes
		ent_slide_on_slopes(ent, physics);
	}
	else{
		
		// airborne - reduce all relative forces
		// to prevent him from jumping or further moving in the air
		physics->friction = cct_air_fric;
		
		physics->force.x *= cct_reduce_input_in_air_factor; // don't set the force completely to zero, otherwise
		physics->force.y *= cct_reduce_input_in_air_factor; // player could be stuck on top of a non-wmb entity
		
		// reset absolute forces
		vec_fill(&physics->abs_force, 0);
		
		// add falling time, only falling if moving downward
		if((physics->force.z <= 0)){ physics->falling_timer += time_step; }
	}
	
	// foot check here
	ent_foot_check(ent, physics);
	
	// accelerate the entity relative speed by the force
	var fric = maxv((1 - time_step * physics->friction), 0);
	physics->velocity.x = (time_step * physics->force.x) + (fric * physics->velocity.x);
	physics->velocity.y = (time_step * physics->force.y) + (fric * physics->velocity.y);
	physics->velocity.z = 0;
	
	// calculate relative distances to move
	physics->dist.x = physics->velocity.x * time_step;
	physics->dist.y = physics->velocity.y * time_step;
	physics->dist.z = 0;
	
	// calculate absolute distance to move
	physics->abs_dist.x = physics->abs_force.x * time_step;
	physics->abs_dist.y = physics->abs_force.y * time_step;
	physics->abs_dist.z = physics->abs_force.z * time_step;
	
	// add the speed given by the ground elasticity and the jumping force
	if(physics->is_grounded == true){
		
		// if the actor is standing on a moving platform, add it's horizontal displacement
		physics->abs_dist.x += physics->surface_speed.x;
		physics->abs_dist.y += physics->surface_speed.y;
	}
	
	// push forces from props
	if(vec_length(vector(physics->push_velocity.x, physics->push_velocity.y, 0)) > 0){
		
		physics->abs_dist.x += physics->push_velocity.x;
		physics->abs_dist.y += physics->push_velocity.y;
		
		vec_fill(&physics->push_velocity, 0);
	}
	
	// relative distance
	move_friction = 0;
	c_ignore(PLAYER_GROUP, PATHFIND_GROUP, 0);
	physics->moving_distance = c_move(ent, nullvector, &physics->dist, MOVE_FLAGS | IGNORE_PUSH | IGNORE_YOU | GLIDE);
	
	if(physics->moving_distance > 0.5){
		
		physics->moving_speed = physics->moving_distance / time_step;
		physics->is_moving = true;
	}
	else{
		
		physics->moving_speed = 0;
		physics->is_moving = false;
	}
	
	// absolute distance
	c_ignore(PUSH_GROUP, PLAYER_GROUP, SWITCH_ITEM_GROUP, PATHFIND_GROUP, 0);
	c_move(my, nullvector, &physics->abs_dist, MOVE_FLAGS | IGNORE_YOU | GLIDE);
}

void ent_movement(ENTITY *ent, CCT_PHYSICS *physics){
	
	// gravity
	ent_vertical_movement(my, physics);
	
	// x-y movement
	ent_horizontal_movement(my, physics);
}

void ent_slide_on_slopes(ENTITY *ent, CCT_PHYSICS *physics){
	
	// we aren't standing on stairs and we are allowed to slide down ?
	if(physics->ground_info != STAIRS && physics->soil_allow_sliding == true){
		
		// reset absolute forces slowly (don't stop right after the slope ends)
		physics->abs_force.x -= (physics->abs_force.x - 0) * 0.25 * time_step;
		physics->abs_force.y -= (physics->abs_force.y - 0) * 0.25 * time_step;
		physics->abs_force.z = 0;
	}
	else{
		
		// reset absolute forces
		vec_fill(&physics->abs_force, 0);
	}
	
	// if slope is too steep
	if(physics->surface_normal.z < 0.9){
		
		// reduce ahead force because player force it is now deflected upwards
		physics->force.x *= physics->surface_normal.z;
		physics->force.y *= physics->surface_normal.z;
		
		// only if we are on slopes
		if(physics->ground_info != STAIRS && physics->soil_allow_sliding == true){
			
			// gravity draws him down the slope
			physics->abs_force.x = physics->surface_normal.x * cct_gravity * 2 * cct_slope_fac;
			physics->abs_force.y = physics->surface_normal.y * cct_gravity * 2 * cct_slope_fac;	
		}
	}
}

void ent_stop_movement(CCT_PHYSICS *physics){
	
	vec_fill(&physics->force, 0);
	vec_fill(&physics->abs_force, 0);
	vec_fill(&physics->dist, 0);
	vec_fill(&physics->abs_dist, 0);
	vec_fill(&physics->speed, 0);
	vec_fill(&physics->velocity, 0);
	vec_fill(&physics->push_force, 0);
	vec_fill(&physics->push_velocity, 0);

	physics->is_moving = 0;
	physics->moving_distance = 0;
	physics->moving_speed = 0;	
}