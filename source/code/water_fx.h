#ifndef _WATER_FX_H_
#define _WATER_FX_H_

// water splash function
void water_splash_effect();

// water ripple function
void water_ripple_effect();

// get impact pos on water plane
void water_get_pos_on_surface(ENTITY *ent, VECTOR *pos);

// create ripple effect on water
void water_create_ripple_effect(VECTOR *pos, var scale);

// create small water impact effect
void water_create_small_impact(VECTOR *pos, var scale);

// create medium water impact effect
void water_create_medium_impact(VECTOR *pos, var scale);

// create big water impact effect
void water_create_big_impact(VECTOR *pos, var scale);

// cct water impact
void water_cct_impact(VECTOR *pos, var scale);

// water hard impact sound
void water_hard_impact_snd(ENTITY *ent);

// water small impact sound
void water_small_impact_snd(ENTITY *ent);

#endif