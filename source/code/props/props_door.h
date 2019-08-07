#ifndef _PROPS_DOOR_H_
#define _PROPS_DOOR_H_

// NOTE - props_door_a and props_door_b differ only by sound effects

// RED_KEY OR YELLOW_KEY OR BLUE_KEY
// if any set, then door will require the key with the set color to be opened!

// TOGGLEABLE
// if set, then each interaction will (toggle) open/close the door
// if notset, then door will go through whole states (open, delay, close) by itself

// USE_SWITCH
// if set, door can be triggered only by switch (with the same ID)

// USE_TRIGGER
// if set, door can be triggered only by trigger zone (with the same ID)

// function to update given door
void door_update(ENTITY *ent);

// check if player has needed key
var door_player_has_key();

// event function for the door
void door_event_function();

// initialize door settings
void door_setup(var snd);

#endif