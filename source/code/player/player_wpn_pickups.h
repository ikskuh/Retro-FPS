#ifndef _PLAYER_WPN_PICKUPS_H_
#define _PLAYER_WPN_PICKUPS_H_

// visual weapons sprite (pickable)
void wpn_visual_func();

// add ammo to the given weapon
void wpn_add_ammo(PLAYER *hero, var num, var am, var mx);

// weapon pickup event function
void wpn_event_function();

// init pickable weapon
void wpn_init(var num);

#endif