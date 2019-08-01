#ifndef _CCT_WATER_DETECTION_H_
	#define _CCT_WATER_DETECTION_H_
	
	// This header contains everything related to water detection
	
	// detect water regions and change our water movement factor, if we are in water
	void ent_detect_water(ENTITY *ent, CCT *cct);
	
	// set water state for the given cct
	void ent_set_water_state(CCT *cct);
	
	// find surface to hop onto
	void ent_detect_water_edge(ENTITY *ent, CCT *cct);

	// detect if entity is inside of the water region, or not
	void ent_detect_water_state(ENTITY *ent, CCT *cct);

	#include "cct_water_detection.c"
#endif