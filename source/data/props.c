
// water skills
#define amplitude skill1
#define speed skill2
#define modulation skill3

// action for visual water plane
// uses: amplitude, speed, modulation
action props_toxic_water(){
	
	set(my, PASSABLE | NOFILTER | TRANSLUCENT);
	reset(my, DYNAMIC);
	my->ambient = 100;
	my->alpha = 50;
	
	if(my->amplitude == 0){ my->amplitude = 1000; }
	if(my->speed == 0){ my->speed = 25; }
	if(my->modulation == 0){ my->modulation = 300; }
	
	my->material = mtl_water;
	my->skill41 = floatv(my->amplitude);	
	my->skill42 = floatv(my->speed);
	my->skill43 = floatv(my->modulation);
}