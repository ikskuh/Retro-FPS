
void register_camera(ENTITY *ent){
	
	if(!ent){
		
		diag("\nERROR! Can't register camera, given entity doesn't exist");
		return;
	}
	
	player_cam = sys_malloc(sizeof(CAMERA));
	
	vec_set(&player_cam->cam_ang, &my->pan);
	player_cam->cam_height = 10;
	player_cam->cam_fov = 90;
	player_cam->cam_3d_dist = -128;
	player_cam->cam_bob_z_offset = 1.5;
	player_cam->cam_bob_speed_factor = 2;
}

void delete_camera(){
	
	sys_free(player_cam);
}

void camera_get_input(){
	
	player_cam->cam_ang.pan = cycle(player_cam->cam_ang.pan - mickey.x / 6.5 * mouse_speed_factor, 0, 360);
	player_cam->cam_ang.tilt = clamp(player_cam->cam_ang.tilt - mickey.y / 6.5 * mouse_speed_factor, -90, 90);
	player_cam->cam_ang.roll = 0;
}

void camera_update(ENTITY *ent, CCT_PHYSICS *physics){
	
	// alive?
	if(ent->obj_health > 0){
		
		// allowed to move?
		if(ent->obj_allow_move == 1){
			
			// get input from player
			camera_get_input();
			
			// first, decide whether the actor is standing on the floor or not
			if(physics->soil_contact == true){
				
				// landed
				if(physics->falling_timer > player_cam->cam_land_timer_limit){
					
					player_cam->cam_land_z_effect = -minv(physics->falling_timer * 6, 12);
				}
				
				// if we are moving, calculate bobbing effects
				if(physics->is_moving == 1 && physics->moving_speed > 0.5){
					
					// camera bob effect
					player_cam->cam_bob_speed = physics->moving_speed * player_cam->cam_bob_speed_factor;
					player_cam->cam_bob = (player_cam->cam_bob + player_cam->cam_bob_speed * time_step) % 360;
					
					// weapon bob effect
					weapon_bob_y = sin(player_cam->cam_bob) * 0.5;
					weapon_bob_z = cos(player_cam->cam_bob * 2) * 0.25;
				}
				else{
					
					weapon_bob_y -= (weapon_bob_y - 0) * 0.1 * time_step;
					weapon_bob_z -= (weapon_bob_z - 0) * 0.1 * time_step;
				}
			}			
			else{
				
				// reset bobbing effect
				if(player_cam->cam_bob < 180){ player_cam->cam_bob -= (player_cam->cam_bob - 0) * 0.5 * time_step; }
				if(player_cam->cam_bob > 180){ player_cam->cam_bob -= (player_cam->cam_bob - 182) * 0.5 * time_step; }
				
				// reset weapon bobbing
				weapon_bob_y -= (weapon_bob_y - 0) * 0.1 * time_step;
				weapon_bob_z -= (weapon_bob_z - 0) * 0.1 * time_step;
			}
		}
	}
	else{
		
		// lower camera and tilt it a little bit upwards
		player_cam->cam_height -= (player_cam->cam_height - (-14)) * 0.5 * time_step;
		player_cam->cam_ang.tilt -= (player_cam->cam_ang.tilt - 5) * 0.5 * time_step;				
		
		// reset bobbing effect
		if(player_cam->cam_bob < 180){ player_cam->cam_bob -= (player_cam->cam_bob - 0) * 0.5 * time_step; }
		if(player_cam->cam_bob > 180){ player_cam->cam_bob -= (player_cam->cam_bob - 182) * 0.5 * time_step; }
		
		// reset landing effect
		player_cam->cam_land_z_effect = 0;
		player_cam->cam_land_z_lerp = 0;
	}
	
	// update landing effect
	if(player_cam->cam_land_z_effect < 0){
		
		player_cam->cam_land_z_effect -= (player_cam->cam_land_z_effect - 0) * 0.25 * time_step;
	}
	
	player_cam->cam_land_z_lerp -= (player_cam->cam_land_z_lerp - player_cam->cam_land_z_effect) * time_step;
	weapon_z_land = player_cam->cam_land_z_lerp * 0.15;
	
	// handle different fog for IN and OUT of aqua
	set_camera_fog();
	
	camera_recoil(ent);
	
	camera_explosion_shake();
	
	// camera FOV
	camera->arc = player_cam->cam_fov;
	camera->arc = clamp(camera->arc, 40, player_cam->cam_fov);		
	
	// set camera angles
	camera->pan = player_cam->cam_ang.pan + player_cam->recoil_ang.pan;
	camera->tilt = player_cam->cam_ang.tilt + player_cam->recoil_ang.tilt + (player_cam->cam_fluid_damage_roll * 0.5) + player_cam->cam_land_z_lerp;
	camera->roll = player_cam->cam_ang.roll + player_cam->cam_fluid_damage_roll;
	
	// handle third person camera mode
	if(camera_mode == THIRD_PERSON){
		
		player_cam->cam_3d_dist -= 0.3 * mickey.z;
		player_cam->cam_3d_dist = clamp(player_cam->cam_3d_dist, -512, -64);
		player_cam->cam_3d_focus = fcos(camera->tilt, player_cam->cam_3d_dist);
		
		camera->x = ent->x + fcos((camera->pan), player_cam->cam_3d_focus);
		camera->y = ent->y + fsin((camera->pan), player_cam->cam_3d_focus);
		camera->z = physics->origin.z + fsin(camera->tilt, player_cam->cam_3d_dist);		
		
		if(is(ent, INVISIBLE)){ reset(ent, INVISIBLE); }
	}
	
	// all non third person modes lead to default first person camera
	if(camera_mode != THIRD_PERSON){
		
		var cam_bobbing = fsin(player_cam->cam_bob * 2, player_cam->cam_bob_z_offset);
		var cam_z_height = player_cam->cam_height + player_cam->cam_land_z_lerp - cam_bobbing;
		
		vec_set(&player_cam->cam_pos, vector(player_cam->explo_pos.x, player_cam->explo_pos.y, cam_z_height));
		vec_rotate(&player_cam->cam_pos, vector(player_cam->cam_ang.pan, 0, 0));
		vec_add(&player_cam->cam_pos, &physics->origin);
		vec_set(&camera->x, &player_cam->cam_pos);
		
		if(!is(ent, INVISIBLE)){ set(ent, INVISIBLE); }
	}
}

void set_camera_fog(){
	
	// if we are in toxic water ?
	if(region_check(reg_toxic_aqua_str, &camera->x, &camera->x)){
		
		fog_color = 2;
		vec_set(&d3d_fogcolor2.blue, COLOR_TOXIC_WATER_FOG);
		vec_set(&sky_color.blue, &d3d_fogcolor2.blue);
		
		camera->fog_start = TOXIC_WATER_FOG_START;
		camera->fog_end = TOXIC_WATER_FOG_END;		
	}
	else{
		
		fog_color = 4;
		vec_set(&d3d_fogcolor4.blue, COLOR_WORLD_FOG);
		vec_set(&sky_color.blue, &d3d_fogcolor4.blue);
		
		camera->fog_start = WORLD_FOG_START;
		camera->fog_end = WORLD_FOG_END;
	}
}

void camera_recoil(ENTITY *ent){
	
	// recoil power
	if(weapon_do_recoil == 1 && ent->obj_health > 0){
		
		if(random(1) > 0.5){ player_cam->recoil_ang.pan -= random(weapon_recoil_rate); }
		else{ player_cam->recoil_ang.pan += random(weapon_recoil_rate); }			
		player_cam->recoil_ang.tilt += (weapon_recoil_rate + random(weapon_recoil_rate));
	}
	else{
		
		player_cam->recoil_ang.pan -= (player_cam->recoil_ang.pan - 0) * 0.75 * time_step;
		player_cam->recoil_ang.tilt -= (player_cam->recoil_ang.tilt - 0) * 0.75 * time_step;
	}
	
	player_cam->recoil_ang.tilt = clamp(player_cam->recoil_ang.tilt, 0, 35);
}

void camera_explosion_shake(){
	
	player_cam->cam_explo_power -= (player_cam->cam_explo_power - 0) * 0.1 * time_step;
	
	if(player_cam->cam_explo_power > 0.1){
		
		if(random(1) > 0.5){ player_cam->explo_pos.x = random(player_cam->cam_explo_power); }
		else{ player_cam->explo_pos.x = -random(player_cam->cam_explo_power); }
		if(random(1) > 0.5){ player_cam->explo_pos.y = random(player_cam->cam_explo_power); }
		else{ player_cam->explo_pos.y = -random(player_cam->cam_explo_power); }
	}
	else{
		
		player_cam->explo_pos.x -= (player_cam->explo_pos.x - 0) * 0.5 * time_step;
		player_cam->explo_pos.y -= (player_cam->explo_pos.y - 0) * 0.5 * time_step;
	}
}

void camera_aqua_damage(){
	
	player_cam->cam_fluid_damage_roll -= (player_cam->cam_fluid_damage_roll - 0) * time_step;
}