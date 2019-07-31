#ifndef _CCT_AQUA_DETECTION_H_
	#define _CCT_AQUA_DETECTION_H_
	
	STRING *reg_toxic_aqua_str = "toxic_reg";
	
	void ent_detect_aqua(ENTITY *ent, CCT_PHYSICS *physics);
	
	#include "cct_aqua_detection.c"
#endif