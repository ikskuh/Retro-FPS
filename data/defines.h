#ifndef _DEFINES_H_
	#define _DEFINES_H_
	
	#define false 0
	#define true 1

	// default trace/move flags
	#define TRACE_FLAGS (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT)
	#define MOVE_FLAGS (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT)

	// fog colors
	#define COLOR_WORLD_FOG vector(128, 128, 128)
	#define COLOR_WATER_FOG vector(11, 23, 31)
	#define COLOR_LAVA_FOG vector(0, 128, 255)

	// fog settings (start, end)
	#define WORLD_FOG_END 2000
	#define WORLD_FOG_START 100
	#define WATER_FOG_END 300
	#define WATER_FOG_START -50
	#define LAVA_FOG_END 100
	#define LAVA_FOG_START -50

	// collusion groups/push values
	#define PUSH_GROUP 2
	#define SWITCH_ITEM_GROUP 3
	#define PLAYER_GROUP 4
	#define SHOOTABLE_GROUP 5
	#define OBSTACLE_GROUP 6

	#define LEVEL_GROUP 10
	
	// object skills
	#define obj_main_struct skill40
	#define obj_physics_struct skill41
	#define obj_animator_struct skill42
	#define obj_weapon_struct skill43
	
	#define obj_type skill44
	#define TYPE_NONE 0
	#define TYPE_PLAYER 1
	#define TYPE_NPC 2
	#define TYPE_ELEVATOR 3
	#define TYPE_PLATFORM 4
	
	#define obj_health skill45
	#define obj_armor skill46
	#define obj_move_npc skill47

	// game logic stuff
	var game_running = false; // 1 - when game is running, otherwise - 0 (used to stop main game loop)
	var game_level_is_loaded = false; // true - if level is loaded, otherwise - false
	var game_ticks = 0; // alternative to native total_ticks

#endif