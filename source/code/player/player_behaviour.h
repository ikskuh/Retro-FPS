#ifndef _PLAYER_BEHAVIOUR_H_
#define _PLAYER_BEHAVIOUR_H_

// toggle auto run mode on and off
void player_toggle_run();

// handle player's pain sounds
void player_pain_sound(ENTITY *ent);

// handles player's death sounds
void player_death_sound(ENTITY *ent, CCT *cct, PLAYER *hero);

// handles all events that player receives
// such as EVENT_PUSH, EVENT_SCAN, EVENT_SHOOT etc
void player_event_function();

// handles all stuff related to after death life
void player_dead(ENTITY *ent);

// handle all player's interactions
void player_interact(ENTITY *ent, CCT *cct);

// player's update function
void player_update(ENTITY *ent);

#endif