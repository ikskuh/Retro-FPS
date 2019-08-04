#ifndef _PROPS_SWITCH_H
	#define _PROPS_SWITCH_H
	
	// toggleable (set by default)
	// if set, then we are able to use this switch more than once
	// if not set, then switch will be used only once
	// NOTE if you have more than one switches with the same id
	// and one of them has this flag not set, then it will be considered
	// as USED when you'll press another switch with same ID
	
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