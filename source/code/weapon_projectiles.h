#ifndef _WEAPON_PROJECTILE_H_
#define _WEAPON_PROJECTILE_H_

// impact sprite animation
void bullet_impact_sprite();

// blood impact sprite animation
void bullet_blood_impact_sprite();

// ricochet sounds
// MAX 4 bullet impact sounds !
void bullet_ric_snd();

// bullet impact water sounds
// MAX 4 bullet impact sounds !
void bullet_water_impact_snd();

// bullet impact body sounds
// MAX 4 bullet impact sounds !
void bullet_body_impact_snd();

// handle bullet impacts
void bullet_impact_fx(ENTITY *ent, var is_alive, VECTOR *hit_vector, VECTOR *surf_angle);

// bullet function
void bullet_func();

// create bullets
void bullet_create(VECTOR *pos, VECTOR *angle, var owner_group, var owner_damage, var bullet_speed, var accur);

// function to update given bullet
void bullet_update(ENTITY *ent);

#endif