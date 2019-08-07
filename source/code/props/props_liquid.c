
// event function for water plane
// animate skin, if we aren't using shaders!
void water_event_function()
{
	if (event_type == EVENT_FRAME)
	{
		my->AMPLITUDE += my->WATER_SPEED * time_step;
		my->AMPLITUDE %= my->MODULATION;
		my->skin = my->AMPLITUDE + 1;
	}
}

// action for visual water plane
// uses: AMPLITUDE, WATER_SPEED, MODULATION
action props_water()
{
	set(my, PASSABLE | NOFILTER);
	reset(my, DYNAMIC);
	my->ambient = 100;

#ifndef FREE_VERSION
	if (my->AMPLITUDE == 0)
	{
		my->AMPLITUDE = 1000;
	}
	if (my->WATER_SPEED == 0)
	{
		my->WATER_SPEED = 25;
	}
	if (my->MODULATION == 0)
	{
		my->MODULATION = 300;
	}

	my->material = mtl_water;
	my->skill41 = floatv(my->AMPLITUDE);
	my->skill42 = floatv(my->WATER_SPEED);
	my->skill43 = floatv(my->MODULATION);
#else
	if (my->WATER_SPEED == 0)
	{
		my->WATER_SPEED = 0.15;
	}
	my->AMPLITUDE = 0;
	my->MODULATION = ent_status(my, 8); // get total amount of skins
	my->event = water_event_function;
	my->emask |= ENABLE_FRAME;
#endif
}