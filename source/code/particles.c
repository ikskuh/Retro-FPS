
// randomize given vector within the given range
void vec_randomize(var *vec, var range){
	
	vec[0] = random(1) - 0.5;
	vec[1] = random(1) - 0.5;
	vec[2] = random(1) - 0.5;
	vec_normalize(vec, random(range));
}

// slow down given particle
void particle_slow_down(PARTICLE *p, var value){
	
	VECTOR temp;
	vec_set(&temp, &p->vel_x);
	
	vec_normalize(&temp, minv(vec_length(&temp), value * time_step));
	vec_inverse(&temp);
	
	vec_add(&p->vel_x, &temp);
}

// fading event function for bubble particle
void bubbles_fade_event(PARTICLE *p){
	
	particle_slow_down(p, 0.5);
	p->size += 0.05 * time_step;
	if(p->z > p->skill_x - 16){ p->lifespan = 0; }
	if(region_check(reg_water_str, &p->x, &p->x) == 0){ p->lifespan = 0; }
}

// bubbles particle
void bubbles_particle(PARTICLE *p){
	
	p->skill_x = p->vel_x;
	p->vel_x = 0;
	p->alpha = 0;
	
	if(p->z < p->skill_x - 16){
		
		p->bmap = bubbles_tga;
		p->size = 2 + random(4); 
		p->alpha = 25 + random(10);
	}
	
	VECTOR temp;
	vec_randomize(&temp, 6 + random(4));
	p->vel_z -= 5;
	vec_add(&p->vel_x, &temp);
	
	p->gravity = -0.75;
	p->lifespan = 200;
	p->flags |= (MOVE | TRANSLUCENT | NOFILTER);
	p->event = bubbles_fade_event;
}