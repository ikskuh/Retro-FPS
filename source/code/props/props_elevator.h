#ifndef _PROPS_ELEVATOR_H
#define _PROPS_ELEVATOR_H

// TOGGLEABLE
// if set, then each interaction will (toggle) move plaftorm up and down
// if not set, then elevator will go through whole states (open, delay, close) by itself

// USE_SWITCH
// if set, elevator can be triggered only by switch (with the same ID)

// USE_TRIGGER
// if set, elevator can be triggered only by trigger zone (with the same ID)

// function to update given elevator
void elevator_update(ENTITY *ent);

// event function for the elevator
void elevator_event_function();

#endif