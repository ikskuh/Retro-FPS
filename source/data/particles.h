#ifndef _PARTICLES_H_
	#define _PARTICLES_H_
	
	// This header handles everything related to particles
	
	// randomize given vector within the given range
	void vec_randomize(var *vec, var range);

	// slow down given particle
	void particle_slow_down(PARTICLE *p, var value);
	
	// fading event function for bubble particle
	void bubbles_fade_event(PARTICLE *p);

	// bubbles particle
	void bubbles_particle(PARTICLE *p);

	#include "particles.c"
#endif