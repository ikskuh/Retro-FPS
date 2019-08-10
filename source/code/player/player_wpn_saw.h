#ifndef _PLAYER_WPN_SAW_H_
#define _PLAYER_WPN_SAW_H_

// play saw hit sound
void saw_hit_solid_sfx();

// play saw impact sound
void saw_impact_fx(var is_alive, VECTOR *hit_vector, VECTOR *surf_angle);

// stop saw sound
void player_saw_stop_snd(PLAYER *hero);

// initialize all saw parameters
void player_saw_init(PLAYER *hero);

// draw function for saw
void player_saw_draw(PLAYER *hero);

// idle function for saw
void player_saw_idle(PLAYER *hero);

// function for playing saw attack sound
void player_saw_snd_attack(PLAYER *hero);

// saw attack function
void player_saw_attack(PLAYER *hero);

// animate saw
void player_saw_animate(PLAYER *hero);

#endif