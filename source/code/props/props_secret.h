#ifndef _PROPS_SECRET_H_
	#define _PROPS_SECRET_H_
	
	// This header handles everything related to secret rooms
	// Secret walls and zones can be triggered only ONCE !

	// update secret wall
	void secret_wall_update(ENTITY *ent);

	// event function for secret wall
	void secret_wall_event();
	
	// event function for secret zone
	void secret_zone_event();

#endif