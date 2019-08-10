
// randomize given vector within the given range
void vec_randomize(var *vec, var range)
{
	vec[0] = random(1) - 0.5;
	vec[1] = random(1) - 0.5;
	vec[2] = random(1) - 0.5;
	vec_normalize(vec, random(range));
}

// slow down given particle
void particle_slow_down(PARTICLE *p, var value)
{
	VECTOR temp_pos;
	vec_set(&temp_pos, &p->vel_x);

	vec_normalize(&temp_pos, minv(vec_length(&temp_pos), value * time_step));
	vec_inverse(&temp_pos);

	vec_add(&p->vel_x, &temp_pos);
}

// fading event function for bubble particle
void bubbles_fade_event(PARTICLE *p)
{
	particle_slow_down(p, 0.5);
	p->size += 0.05 * time_step;
	if (p->z > p->skill_x - 16)
	{
		p->lifespan = 0;
	}
	if (region_check(reg_water_str, &p->x, &p->x) == 0)
	{
		p->lifespan = 0;
	}
}

// bubbles particle
void bubbles_particle(PARTICLE *p)
{
	p->skill_x = p->vel_x;
	p->vel_x = 0;
	p->alpha = 0;

	if (p->z < p->skill_x - 16)
	{
		p->bmap = bubbles_tga;
		p->size = 2 + random(4);
		p->alpha = 25 + random(10);
	}

	VECTOR temp_pos;
	vec_randomize(&temp_pos, 6 + random(4));
	p->vel_z -= 5;
	vec_add(&p->vel_x, &temp_pos);

	p->gravity = -0.75;
	p->lifespan = 200;
	p->flags |= (MOVE | TRANSLUCENT | NOFILTER);
	p->event = bubbles_fade_event;
}

// spawn given amount of bubbles at the given position with the given range
void bubbles_spawn(VECTOR *pos, var num, VECTOR *range, var water_zone_height)
{
	var i = 0;
	for (i = 0; i < num; i++)
	{
		VECTOR temp_pos;
		temp_pos.x = pos->x + range->x - random(range->x * 2);
		temp_pos.y = pos->y + range->y - random(range->y * 2);
		temp_pos.z = pos->z - range->z + (range->z * 2 - random(range->z * 4));
		effect(bubbles_particle, 1, &temp_pos, vector(water_zone_height, 0, 0));
	}
}

// bullet impact fade event
void bullet_impact_fade_event(PARTICLE *p)
{
	particle_slow_down(p, 0.5);
}

// impact particle effects
void bullet_impact_particle(PARTICLE *p)
{
	VECTOR temp_vec;
	vec_randomize(&temp_vec, (2 + random(2)));
	vec_add(&p->vel_x, &temp_vec);
	vec_fill(&p->blue, random(96));
	p->bmap = particle_png;
	p->size = 1 + random(1);
	p->gravity = 0.75;
	p->alpha = 70 + random(30);
	p->lifespan = 8 + random(1);
	p->flags |= (MOVE | TRANSLUCENT | NOFILTER | LIGHT);
	p->event = bullet_impact_fade_event;
}