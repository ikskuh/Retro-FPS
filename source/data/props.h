#ifndef _PROPS_H
	#define _PROPS_H
	
	// This header handles everything related to level props
	//
	// This header also includes such headers as
	// - props_helper.h (helper functions for all props)
	// - props_liquid.h (everything needed for water, lava etc)
	// - props_switch.h (everything needed for creating switches)
	// - props_trigger.h (everything needed for creating trigger zones)
	// - props_secret.h (everything needed fro creating secret rooms)
	// - props_elevator.h (everything needed for elevators)
	// - props_door.h (everything needed for doors)
	// - props_platforms.h (everything needed for creating platforms that follow path)
	// - props_loop.h (update all props in one loop)
	
	// skills used by all props
	#define id skill1
	#define offset_x_ skill2
	#define offset_y_ skill3
	#define offset_z_ skill4
	
	#define toggleable FLAG3
	#define use_switch FLAG4
	#define use_trigger FLAG5
	#define red_key FLAG6
	#define yellow_key FLAG7
	#define blue_key FLAG8
	
	// states
	#define IDLE 0
	#define OPEN 1
	#define CLOSE 2
	#define DELAY 3
	#define DEAD 4
	
	// props main structure
	typedef struct {
		
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
	
	#include "props.c"
	#include "props_helper.h"
	#include "props_liquid.h"
	#include "props_switch.h"
	#include "props_trigger.h"
	#include "props_secret.h"
	#include "props_elevator.h"
	#include "props_door.h"
	#include "props_platform.h"
	
	#include "props_loop.h"
#endif