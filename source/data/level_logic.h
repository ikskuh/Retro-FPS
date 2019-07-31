#ifndef _LEVEL_LOGIC_H_
	#define _LEVEL_LOGIC_H_
	
	#define MAX_LEVELS 100
	
	var level_id = 0; // current level id, increased at the end of each level
	STRING *level_str[MAX_LEVELS]; // list of strings, containing each level's name
	
	void set_level_names();
	
	void set_level_settings();
	
	void level_reset();
	
	void level_load_();
	
	void level_restart();

	#include "level_logic.c"
#endif