#ifndef _LEVEL_LOGIC_H_
	#define _LEVEL_LOGIC_H_
	
	// This header includes everything related to level setup and loading
	
	// initialize all levels name (in string list)
	void set_level_names();
	
	// set level settings (sun, fog, clipping etc)
	void set_level_settings();
	
	// reset variables per level
	void level_reset();
	
	// load level (level_str with level_id)
	void level_load_();
	
	// restart level
	void level_restart();

	#include "level_logic.c"
#endif