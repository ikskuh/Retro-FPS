#ifndef _PROPS_HELPER_H
	#define _PROPS_HELPER_H
	
	// set's unused coordinates, to the origin ones
	void props_offset_trim(ENTITY *ent);

	// check if entity needs to be smashed or not (f.e. by door on close etc)
	void props_vs_npc_check(ENTITY *prop, ENTITY *npc, var scale);

	// check if props almost closed
	void props_almost_closed_check(ENTITY *ent, PROPS *props);

	// check if props almost opened
	void props_almost_opened_check(ENTITY *ent);

	#include "props_helper.c"
#endif