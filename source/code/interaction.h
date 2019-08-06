#ifndef _INTERACTION_H_
#define _INTERACTION_H_

#include "defines.h"

#define INTERACTION_SUPPRESSION_TIME 0.5 // half second suppression

// enable interactions for this entity
void interaction_enable(ENTITY * ent);

// disable interactions for this entity
void interaction_disable(ENTITY * ent);

// checks if interaction is enabled for this entity
bool interaction_is_enabled(ENTITY * ent);

// resets the interaction state to "not interacted"
void interaction_reset(ENTITY * ent);

// sets the interaction to "i was interacted with!"
void interaction_do(ENTITY * ent, ENTITY * source);

// returns true if the entity had an interaction since the last check.
// will reset the interaction bit automatically
bool interaction_was_triggered(ENTITY * ent);

// returns the last entity that triggered an interaction
ENTITY * interaction_get_source(ENTITY * ent);

#endif // _INTERACTION_H_