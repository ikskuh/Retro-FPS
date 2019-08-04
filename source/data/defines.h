#ifndef _DEFINES_H_
	#define _DEFINES_H_
	
	#include <acknex.h>

	#define ASSERT(_X) _assert(_X, #_X)
	void _assert(int v, char const * msg);

	// macros
	#define math_round(num) return(floor(num + 0.5));
	#define math_check_divide(value, divide) return(integer(divide * math_round( value / divide )))
	#define ent_delete(ent) ptr_remove(ent); ent = NULL
	
	typedef int bool;
	#define false 0
	#define true 1

	#define MAX_LEVELS 100

	// default trace/move flags
	#define TRACE_FLAGS (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT)
	#define MOVE_FLAGS (IGNORE_ME | IGNORE_PASSABLE | IGNORE_PASSENTS | IGNORE_MAPS | IGNORE_SPRITES | IGNORE_CONTENT)

	// fog colors
	#define COLOR_WORLD_FOG vector(128, 128, 128)
	#define COLOR_WATER_FOG vector(11, 23, 31)

	// fog settings (start, end)
	#define WORLD_FOG_END 2000
	#define WORLD_FOG_START 100
	#define WATER_FOG_END 300
	#define WATER_FOG_START -50

	// collusion groups/push values
	#define PUSH_GROUP 2
	#define SWITCH_ITEM_GROUP 3
	#define PATHFIND_GROUP 4
	#define PLAYER_GROUP 5
	#define ENEMY_GROUP 6
	#define SHOOTABLE_GROUP 7
	#define OBSTACLE_GROUP 8

	#define LEVEL_GROUP 10
	
	// object skills
	#define obj_struct skill40
	#define obj_cct_struct skill41
	#define obj_animator_struct skill42
	#define obj_weapon_struct skill43
	
	#define obj_type skill44
	#define TYPE_NONE 0
	#define TYPE_PLAYER 1
	#define TYPE_NPC 2
	#define TYPE_SWITCH 3
	#define TYPE_TRIGGER_ZONE 4
	#define TYPE_SECRET_WALL 5
	#define TYPE_SECRET_ZOME 6
	#define TYPE_ELEVATOR 7
	#define TYPE_DOOR 8
	#define TYPE_PLATFORM 9
	
	#define obj_health skill45
	#define obj_armor skill46
	#define obj_move_npc skill47
	#define obj_allow_move skill48
	#define obj_state skill49
	#define obj_death_type skill50
	#define obj_pain_type skill51
	#define TYPE_UNDEFINED 0
	#define TYPE_SHOOT 1
	#define TYPE_MELEE 2
	#define TYPE_EXPLO 3
	#define TYPE_SMASHED 4
	#define TYPE_OUT_OF_AIR 5
	#define TYPE_FALL_DAMAGE 6
	
	#define obj_snd_handle skill52
	#define obj_water_height skill53
	#define obj_timer skill54
	#define obj_check skill55
	
	// trace/scan states
	#define obj_c_indicator skill56
	#define NONE 0
	#define INTERACT 1
	#define SHOOT 2

	#define OBJ_FLAGS skill57 // custom flags for "more than 8"
	#define OBJ_IS_INTERACTIBLE 1 // the player can interact with this
	#define OBJ_HAD_INTERACTION 2 // the player interacted with this object
	
	#define OBJ_LAST_INTERACTION skill58 // stores total_ticks of last interaction
	#define OBJ_INTERACTION_SOURCE skill59 // stores handle to the last interactor

	// list of strings, containing each level's name
	STRING *level_str[MAX_LEVELS];
	
	// water region
	STRING *reg_water_str = "water_reg";
	
	// level logic stuff
	var level_id = 0; // current level id, increased at the end of each level
	var level_secrets_found = 0; // +1 with each new found secret zone (per level)
	var level_secrets_total = 0; // +1 with each created secret zone (per level)
	
	// game logic stuff
	var game_running = false; // 1 - when game is running, otherwise - 0 (used to stop main game loop)
	var game_level_is_loaded = false; // true - if level is loaded, otherwise - false
	var game_ticks = 0; // alternative to native total_ticks
	
	// bboxes for checks
	var biggest_bbox_x_side = 16; // consider this as a bixest bbox X size (16+16=32)
	var biggest_bbox_y_side = 16; // same as above, but for Y
	var biggest_bbox_z_side = 32; // same as above, but for Z
	
	// cct stuff
	var cct_snd_volume = 450;
	
	// player's stuff
	var player_has_red_key = false;
	var player_has_yellow_key = false;
	var player_has_blue_key = false;
	var player_snd_volume = 50;
	
	// props stuff
	var props_switch_snd_volume = 450;
	var props_elevator_snd_volume = 450;
	var props_elevator_snd_loop_volume = 150;
	var props_door_snd_volume = 450;
	var props_platform_snd_volume = 450;
	var props_platform_snd_loop_volume = 150;
	var props_secret_wall_snd_volume = 450;
	var props_secret_zone_snd_volume = 50;
	
	// checks if given cct is inside of the given entity
	// returns true if true, else if not
	var is_cct_in_rect(ENTITY *cct, ENTITY *rect, var scale);
#endif