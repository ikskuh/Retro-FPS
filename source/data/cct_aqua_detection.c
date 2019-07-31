
void ent_detect_aqua(ENTITY *ent, CCT_PHYSICS *physics){
	
	physics->aqua_reg_total = 0;
	physics->aqua_depth_factor = 1;
	
	if(physics->aqua_reg_total == 0){
		
		physics->aqua_origin_diff_z = 0;
		physics->aqua_z_height = 0;
	}
	
	var get_toxic_aqua = region_get(reg_toxic_aqua_str, physics->aqua_reg_total, &physics->aqua_reg_min, &physics->aqua_reg_max);
	
	while(get_toxic_aqua != 0){
		
		physics->aqua_reg_total += 1;
		
		if(ent->x < physics->aqua_reg_min.x || ent->x > physics->aqua_reg_max.x){ continue; }
		if(ent->y < physics->aqua_reg_min.y || ent->y > physics->aqua_reg_max.y){ continue; }
		if((physics->aqua_reg_min.z - ent->z) > 0 || (ent->z - physics->aqua_reg_max.z) > 0){ continue; }
		
		physics->aqua_origin_diff_z = physics->aqua_reg_max.z - ent->z;
		physics->aqua_z_height = physics->aqua_reg_max.z;
		physics->aqua_depth_factor = clamp(1 - (physics->aqua_origin_diff_z / 25), 0.1, 1);
	}
}