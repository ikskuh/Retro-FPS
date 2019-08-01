
// get input from player
void camera_get_input(){
	
	hero->cam->angle.pan = cycle(hero->cam->angle.pan - mickey.x / 6.5 * mouse_speed_factor, 0, 360);
	hero->cam->angle.tilt = clamp(hero->cam->angle.tilt - mickey.y / 6.5 * mouse_speed_factor, -90, 90);
	hero->cam->angle.roll = 0;
}

// animate camera
void camera_bobbing(CCT *cct){
	
	// if we are moving, calculate bobbing effects
	if(cct->is_moving == 1 && cct->moving_speed > 0.5){
		
		// camera bob effect
		hero->cam->bob_speed = cct->moving_speed * hero->cam->bob_speed_factor;
		hero->cam->bob = (hero->cam->bob + hero->cam->bob_speed * time_step) % 360;
		
		// weapon bob effect
		weapon_bob_y = sin(hero->cam->bob) * 0.5;
		weapon_bob_z = cos(hero->cam->bob * 2) * 0.25;
	}
	else{
		
		weapon_bob_y -= (weapon_bob_y - 0) * 0.1 * time_step;
		weapon_bob_z -= (weapon_bob_z - 0) * 0.1 * time_step;
	}
}

// reset bobbing effect
void camera_bobbing_reset(CCT *cct){
	
	// reset bobbing effect
	if(hero->cam->bob < 180){ hero->cam->bob -= (hero->cam->bob - 0) * 0.5 * time_step; }
	if(hero->cam->bob > 180){ hero->cam->bob -= (hero->cam->bob - 182) * 0.5 * time_step; }
	
	// reset weapon bobbing
	weapon_bob_y -= (weapon_bob_y - 0) * 0.1 * time_step;
	weapon_bob_z -= (weapon_bob_z - 0) * 0.1 * time_step;
}

// update everything related to the camera
void camera_update(ENTITY *ent, CCT *cct){
	
	// alive?
	if(ent->obj_health > 0){
		
		// allowed to move?
		if(ent->obj_allow_move == 1){
			
			// get input from player
			camera_get_input();
			
			// if we are swimming
			if(cct->water_state >= IN_WATER){
				
				camera_bobbing(cct);
				
				// reset Z bobbing offset for camera
				// this will affect only camera, but not the weapon
				hero->cam->bob_z_offset -= (hero->cam->bob_z_offset - 0) * 0.1 * time_step;
				
				// reset landing effect
				hero->cam->land_effect -= (hero->cam->land_effect - 0) * 0.25 * time_step;
				
				// sway a little bit up and down while we are underwater
				hero->cam->water_z = 0 - 1 * sinv(8 * game_ticks);
				hero->cam->water_z_lerp -= (hero->cam->water_z_lerp - hero->cam->water_z) * 0.25 * time_step;
			}
			else{
				
				if(cct->is_grounded == true){
					
					camera_bobbing(cct);
				}
				else{
					
					camera_bobbing_reset(cct);
				}
				
				// reset water sway effect
				hero->cam->water_z = 0;
				hero->cam->water_z_lerp -= (hero->cam->water_z_lerp - hero->cam->water_z) * time_step;
				
				// handle landing effect
				hero->cam->land_effect -= (hero->cam->land_effect - cct->land_timer) * time_step;
				weapon_z_land = hero->cam->land_effect * 0.15;
			}
		}
	}
	else{
		
		// lower camera and tilt it a little bit upwards
		hero->cam->height -= (hero->cam->height - (-14)) * 0.5 * time_step;
		hero->cam->angle.tilt -= (hero->cam->angle.tilt - 5) * 0.5 * time_step;				
		
		// reset bobbing effect
		if(hero->cam->bob < 180){ hero->cam->bob -= (hero->cam->bob - 0) * 0.5 * time_step; }
		if(hero->cam->bob > 180){ hero->cam->bob -= (hero->cam->bob - 182) * 0.5 * time_step; }
		
		// reset landing and water effect
		hero->cam->water_z = 0;
		hero->cam->water_z_lerp = 0;
		hero->cam->land_effect = 0;
	}
	
	// handle different fog for IN and OUT of aqua
	set_camera_fog(cct);
	camera_recoil(ent);
	camera_explosion_shake();
	
	// camera FOV
	camera->arc = hero->cam->fov;
	camera->arc = clamp(camera->arc, 40, hero->cam->fov);		
	
	// set camera angles
	camera->pan = hero->cam->angle.pan + hero->cam->recoil_angle.pan;
	camera->tilt = hero->cam->angle.tilt + hero->cam->recoil_angle.tilt - hero->cam->land_effect;
	camera->roll = hero->cam->angle.roll;
	
	// handle third person camera mode
	if(camera_mode == THIRD_PERSON){
		
		hero->cam->third_person_dist -= 0.3 * mickey.z;
		hero->cam->third_person_dist = clamp(hero->cam->third_person_dist, -512, -64);
		hero->cam->third_person_focus = fcos(camera->tilt, hero->cam->third_person_dist);
		
		camera->x = ent->x + fcos((camera->pan), hero->cam->third_person_focus);
		camera->y = ent->y + fsin((camera->pan), hero->cam->third_person_focus);
		camera->z = cct->origin.z + fsin(camera->tilt, hero->cam->third_person_dist);		
		
		if(is(ent, INVISIBLE)){ reset(ent, INVISIBLE); }
	}
	
	// all non third person modes lead to default first person camera
	if(camera_mode != THIRD_PERSON){
		
		var cam_bobbing = fsin(hero->cam->bob * 2, hero->cam->bob_z_offset);
		var cam_z_height = hero->cam->height - hero->cam->land_effect - cam_bobbing + hero->cam->water_z_lerp;
		
		vec_set(&hero->cam->position, vector(hero->cam->explo_vector.x, hero->cam->explo_vector.y, cam_z_height));
		vec_rotate(&hero->cam->position, vector(hero->cam->angle.pan, 0, 0));
		vec_add(&hero->cam->position, &cct->origin);
		vec_set(&camera->x, &hero->cam->position);
		
		if(!is(ent, INVISIBLE)){ set(ent, INVISIBLE); }
	}
}

// change camera's fog, if we are inside of the water region
void set_camera_fog(CCT *cct){
	
	// if our head in water ?
	if(cct->water_state == HEAD_IN_WATER){
		
		fog_color = 2;
		vec_set(&d3d_fogcolor2.blue, COLOR_WATER_FOG);
		vec_set(&sky_color.blue, &d3d_fogcolor2.blue);
		
		camera->fog_start = WATER_FOG_START;
		camera->fog_end = WATER_FOG_END;
		
		if(pp_underwater_is_on == false){
			
			if(!snd_playing(hero->cam->underwater_snd_handle)){
				
				hero->cam->underwater_snd_handle = snd_loop(underwater_ogg, camera_underwater_volume, 0);
			}
			
			pp_add(pp_mtl_underwater, camera, camera->bmap);
			pp_add(pp_mtl_colorintensity, camera, camera->bmap);
			pp_colorintensity_set_value(0.5, 0.75, 0.25);
			
			pp_underwater_is_on = true;
		}		
	}
	else{
		
		fog_color = 4;
		vec_set(&d3d_fogcolor4.blue, COLOR_WORLD_FOG);
		vec_set(&sky_color.blue, &d3d_fogcolor4.blue);
		
		camera->fog_start = WORLD_FOG_START;
		camera->fog_end = WORLD_FOG_END;
		
		if(snd_playing(hero->cam->underwater_snd_handle)){
			
			snd_stop(hero->cam->underwater_snd_handle);
		}
		
		if(pp_underwater_is_on == true){
			
			pp_remove(pp_mtl_underwater, camera, camera);
			pp_remove(pp_mtl_colorintensity, camera, camera);
			pp_underwater_is_on = false;
		}
	}
}

// handle recoil effect
void camera_recoil(ENTITY *ent){
	
	// recoil power
	if(weapon_do_recoil == 1 && ent->obj_health > 0){
		
		if(random(1) > 0.5){ hero->cam->recoil_angle.pan -= random(weapon_recoil_rate); }
		else{ hero->cam->recoil_angle.pan += random(weapon_recoil_rate); }			
		hero->cam->recoil_angle.tilt += (weapon_recoil_rate + random(weapon_recoil_rate));
	}
	else{
		
		hero->cam->recoil_angle.pan -= (hero->cam->recoil_angle.pan - 0) * 0.75 * time_step;
		hero->cam->recoil_angle.tilt -= (hero->cam->recoil_angle.tilt - 0) * 0.75 * time_step;
	}
	
	hero->cam->recoil_angle.tilt = clamp(hero->cam->recoil_angle.tilt, 0, 35);
}

// handle camera shaking effect (used for explosions and moving on platforms/elevators)
void camera_explosion_shake(){
	
	hero->cam->explo_power -= (hero->cam->explo_power - 0) * 0.1 * time_step;
	
	if(hero->cam->explo_power > 0.1){
		
		if(random(1) > 0.5){ hero->cam->explo_vector.x = random(hero->cam->explo_power); }
		else{ hero->cam->explo_vector.x = -random(hero->cam->explo_power); }
		if(random(1) > 0.5){ hero->cam->explo_vector.y = random(hero->cam->explo_power); }
		else{ hero->cam->explo_vector.y = -random(hero->cam->explo_power); }
	}
	else{
		
		hero->cam->explo_vector.x -= (hero->cam->explo_vector.x - 0) * 0.5 * time_step;
		hero->cam->explo_vector.y -= (hero->cam->explo_vector.y - 0) * 0.5 * time_step;
	}
}