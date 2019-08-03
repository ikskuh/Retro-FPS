
void set_level_names(){
	
	// set names for each level
	// sadly, but by hand
	level_str[0] = "map.wmb";
}

void set_level_settings(){
	
	set(camera, NOFLAG1);
	
	sun_light = 0;
	sun_angle.pan = 270;
	sun_angle.tilt = -25;
	
	vec_set(&d3d_lodfactor, vector(12.5, 25, 50));
	
	camera->clip_near = 0.1;
	camera->clip_far = 4500;
	
	camera->fog_start = WORLD_FOG_START;
	camera->fog_end = WORLD_FOG_END;
	
	fog_color = 4;
	vec_set(&d3d_fogcolor4.blue, COLOR_WORLD_FOG);
	vec_set(&d3d_fogcolor2.blue, COLOR_WATER_FOG);
	vec_set(&sky_color.blue, &d3d_fogcolor4.blue);
}

void level_reset(){
	
}

void level_load_(){
	
	if(proc_status(level_load_) > 0 || !is(camera, SHOW)){ return; }
	
	reset(camera, SHOW);
	
	snd_stopall(4);
	game_level_is_loaded = false;
	level_reset();
	wait_for(level_reset);
	
	freeze_mode = true;
	level_load(level_str[level_id]);
	wait(3);
	freeze_mode = false;
	
	level_ent->group = LEVEL_GROUP;
	level_ent->push = LEVEL_GROUP;
	
	set_level_settings();
	game_level_is_loaded = true;
	
	// update props
	props_update();
	
	set(camera, SHOW);
}

void level_restart(){
	
	level_load_();
}