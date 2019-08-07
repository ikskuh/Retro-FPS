#ifndef _PROPS_PLATFORM_H_
#define _PROPS_PLATFORM_H_

// NOTE that in order to find path, platform should be placed slightly above the starting node !
// otherwise, you'll get 'platform can't find any path' error

// TOGGLEABLE
// if set, then each interaction will (toggle) move plaftorm up and down
// if not set, then elevator will go through whole states (open, delay, close) by itself

// USE_SWITCH
// if set, elevator can be triggered only by switch (with the same ID)

// USE_TRIGGER
// if set, elevator can be triggered only by trigger zone (with the same ID)

// move platform
var platform_move(ENTITY *ent, PROPS *props);

// update this platform
void platform_update(ENTITY *ent);

// event function for platform
void platform_event_function();

#endif