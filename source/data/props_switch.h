#ifndef _PROPS_SWITCH_H
	#define _PROPS_SWITCH_H
	
	// use_once
	// if set, switch will be pressable only once !
	// note, that if switch with the same ID was triggered
	// switch with this flag set will be considered as USED
	// means, that you won't be able to use it again
	
	// enable all switches with the given id
	void switch_enable_by_id(var num);

	// disable all switches with the given id
	void switch_disable_by_id(var num);

	// function used to trigger props with the same id
	void switch_trigger_props();

	// switch's event function
	void switch_event();
	
	#include "props_switch.c"
#endif