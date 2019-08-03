#ifndef _PROPS_H
	#define _PROPS_H
	
	// This header handles everything related to level props
	//
	// This header also includes such headers as
	// - props_liquid.h (everything needed for water, lava etc)
	// - props_switch.h (everything needed for creating switches)
	// - props_elevator.h (everything needed for elevators)
	
	// skills used by all props
	#define id skill1
	#define offset_x_ skill2
	#define offset_y_ skill3
	#define offset_z_ skill4
	
	#define once FLAG3
	#define switch FLAG4
	#define red_key FLAG5
	#define yellow_key FLAG6
	#define blue_key FLAG7
	
	// props main structure
	typedef struct {
		
		var delay;
		
	} PROPS;
	
	// register and initializes new props (f.e. doors, platforms, elevators)
	// and save it's pointer into given entity's obj_struct skill
	PROPS *register_props(ENTITY *ent);
	
	// initialize all major variables for the given props
	// this function called inside of register_props right after creating new props
	void init_props(PROPS *props);
	
	// returns pointer of the props from given entity's obj_struct skill
	PROPS *get_props(ENTITY *ent);
	
	// removes props from the memory
	// pointer is taken from given entity's obj_struct skill
	void delete_props(ENTITY *ent);
	
	// remove given props from the memory
	void delete_props(PROPS *props);
	
	#include "props.c"
	#include "props_liquid.h"
	#include "props_switch.h"
	#include "props_elevator.h"
#endif