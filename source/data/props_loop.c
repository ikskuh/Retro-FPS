
// update all props in one loop
void props_update(){
	
	proc_mode = PROC_EARLY;
	
	ENTITY *props_ent = NULL;
	
	while(game_level_is_loaded == true){
		
		wait(1);
		
		for(props_ent = ent_next(NULL); props_ent; props_ent = ent_next(props_ent)){
			
			// elevator ? 
			if(props_ent->obj_type == TYPE_ELEVATOR){
				
				elevator_update(props_ent);
			}
			
			// door ?
			if(props_ent->obj_type == TYPE_DOOR){
				
				door_update(props_ent);
			}
			
		}
	}
}