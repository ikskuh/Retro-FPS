
void player_event(){
	
}

action player_controller(){
	
	CCT_PHYSICS *physics = register_cct(my);
	
	my->obj_health = 100;
	my->obj_armor = 0;
	my->obj_type = TYPE_PLAYER;
	
	while(my){
		
		cct_gravity_trace(my, physics);
		
		wait(1);
	}
}