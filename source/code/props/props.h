#ifndef _PROPS_H
#define _PROPS_H

// This header handles everything related to level props
//
// Other props related headers
// - props_helper.h (helper functions for all props)
// - props_liquid.h (everything needed for water, lava etc)
// - props_switch.h (everything needed for creating switches)
// - props_trigger.h (everything needed for creating trigger zones)
// - props_secret.h (everything needed fro creating secret rooms)
// - props_elevator.h (everything needed for elevators)
// - props_door.h (everything needed for doors)
// - props_platforms.h (everything needed for creating platforms that follow path)
// - props_loop.h (update all props in one loop)

// props main structure
typedef struct
{
	VECTOR diff;
	VECTOR origin;
	var movement_speed;
	var delay_time;
	var old_state;
	var dir;
	var dist;
	var path_id;
	var total_nodes;
} PROPS;

// register and initializes new props (f.e. doors, platforms, elevators)
// and save it's pointer into given entity's obj_struct skill
PROPS *register_props(ENTITY *ent);

// initialize all major variables for the given props
// this function called inside of register_props right after creating new props
void init_props(ENTITY *ent, PROPS *props);

// returns pointer of the props from given entity's obj_struct skill
PROPS *get_props(ENTITY *ent);

// removes props from the memory
// pointer is taken from given entity's obj_struct skill
void delete_props(ENTITY *ent);

// remove given props from the memory
void delete_props(PROPS *props);

#endif