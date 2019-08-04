
// set's unused coordinates, to the origin ones
void props_offset_trim(ENTITY *ent){
	
	if(ent->offset_x_ == 0){ ent->offset_x_ = ent->x; }
	if(ent->offset_y_ == 0){ ent->offset_y_ = ent->y; }
	if(ent->offset_z_ == 0){ ent->offset_z_ = ent->z; }	
}

// check if entity needs to be smashed or not (f.e. by door on close etc)
void props_vs_npc_check(ENTITY *prop, ENTITY *npc, var scale){
	
	if(!prop){
		
		diag("\nERROR! Can't perform props vs npc check, props doesn't exist!");
		return;
	}
	
	if(!npc){
		
		diag("\nERROR! Can't perform props vs npc check, npc doesn't exist!");
		return;
	}
	
	// we need to check for collision ?
	if(prop->obj_check == 1){
		
		// if props smashed npc, then npc should die !!!
		if(is_cct_in_rect(npc, prop, scale) == true){
			
			npc->obj_health = -100;
			npc->obj_death_type = TYPE_SMASHED;
		}
		prop->obj_check = 0;
	}
}

// check (for smashing cct) if props almost closed
void props_almost_closed_check(ENTITY *ent, PROPS *props){
	
	var x_distance = abs(ent->x - props->origin.x);
	var y_distance = abs(ent->y - props->origin.y);
	var z_distance = abs(ent->z - props->origin.z);
	
	if(x_distance <= biggest_bbox_x_side && y_distance <= biggest_bbox_x_side && z_distance <= biggest_bbox_z_side){
		
		ent->obj_check = true;
	}
}

// check (for smashing cct) if props almost opened
void props_almost_opened_check(ENTITY *ent){
	
	var x_distance = abs(ent->x - ent->offset_x_);
	var y_distance = abs(ent->y - ent->offset_y_);
	var z_distance = abs(ent->z - ent->offset_z_);
	
	if(x_distance <= biggest_bbox_x_side && y_distance <= biggest_bbox_x_side && z_distance <= biggest_bbox_z_side){
		
		ent->obj_check = true;
	}
}