
void ent_detect_aqua(ENTITY *ent, CCT_PHYSICS *physics){
	
	physics->aqua_reg_total = 0;
	physics->aqua_depth_factor = 1;
	
	if(physics->aqua_reg_total == 0){
		
		physics->aqua_origin_diff_z = 0;
		physics->aqua_z_height = 0;
	}
	
	while(region_get(reg_toxic_aqua_str, physics->aqua_reg_total, &physics->aqua_reg_min, &physics->aqua_reg_max) != 0){
		
		physics->aqua_reg_total += 1;
		
		if(ent->x < physics->aqua_reg_min.x || ent->x > physics->aqua_reg_max.x){ continue; }
		if(ent->y < physics->aqua_reg_min.y || ent->y > physics->aqua_reg_max.y){ continue; }
		if((physics->aqua_reg_min.z - ent->z) > 0 || (ent->z - physics->aqua_reg_max.z) > 0){ continue; }
		
		physics->aqua_origin_diff_z = physics->aqua_reg_max.z - ent->z;
		physics->aqua_z_height = physics->aqua_reg_max.z;
		physics->aqua_depth_factor = clamp(1 - (physics->aqua_origin_diff_z / 15), 0.1, 1);	
	}
}

void ent_aqua_sounds(ENTITY *ent, CCT_PHYSICS *physics){
	
	if(physics->aqua_state == WADE && physics->aqua_snd_water_in == false){
		
		if(snd_playing(physics->aqua_snd_handle)){ snd_stop(physics->aqua_snd_handle); }
		
		// we need to handle sound playing differently for player and other npcs
		// since for npcs we need to play 3d sound and for player not
		if(ent->obj_type == TYPE_PLAYER){
			
			physics->aqua_snd_handle = snd_play(water_in_ogg, cct_player_water_volume, 0);
		}
		else{
			
			physics->aqua_snd_handle = ent_playsound(ent, water_in_ogg, cct_water_snd_volume);
		}
		
		physics->aqua_snd_water_in = true;
		physics->aqua_snd_water_out = false;
	}
	else if (physics->aqua_state == ABOVE && physics->aqua_snd_water_out == false){
		
		if(snd_playing(physics->aqua_snd_handle)){ snd_stop(physics->aqua_snd_handle); }
		if(ent->obj_type == TYPE_PLAYER){
			
			physics->aqua_snd_handle = snd_play(water_out_ogg, cct_player_water_volume, 0);
		}
		else{
			
			physics->aqua_snd_handle = ent_playsound(ent, water_out_ogg, cct_water_snd_volume);
		}
		
		physics->aqua_snd_water_out = true;
		physics->aqua_snd_water_in = false;
	}
}

void ent_aqua(ENTITY *ent, CCT_PHYSICS *physics){
	
	ent_detect_aqua(ent, physics);
	
	if(physics->aqua_origin_diff_z > 0 && physics->aqua_origin_diff_z < 16){
		physics->aqua_state = WADE;
	} 
	else{
		
		if(physics->aqua_origin_diff_z > 16){
			
			physics->aqua_state = UNDERWATER;
		} 
		else{
			
			physics->aqua_state = ABOVE;
		}
	}
	
	if(ent->obj_health > 0){
		
		ent_toxic_water_damage(ent, physics);
		ent_aqua_sounds(ent, physics);
	}
}