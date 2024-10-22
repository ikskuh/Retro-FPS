#ifndef _DEFINES_H_
#define _DEFINES_H_

// macros
#define FOR_ENT_OF_TYPE(e, t) for ((e) = ent_next(NULL); (e) != NULL; (e) = ent_next((e))) if ((e)->OBJ_TYPE == (t))
#define ASSERT(_X) _assert(_X, "assertion failed!")

// max indexes
#define MAX_WEAPONS 7
#define MAX_LEVELS 100

// game states
#define GAME_LOADING 0
#define GAME_RUNNING 1

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

// player's state machine states
#define IDLE 0
#define WALK 1
#define RUN 2
#define JUMPED 3
#define IN_AIR 4
#define LANDED 5
#define PAIN 6
#define SHOOT 7
#define DEATH 8
#define SWIM 9
#define DIVE 10

// camera modes
#define FIRST_PERSON 0
#define THIRD_PERSON 1

// props states
#define IDLE 0
#define OPEN 1
#define CLOSE 2
#define DELAY 3
#define DEAD 4

// skills used by all props
#define ID skill1
#define OFFSET_X_ skill2
#define OFFSET_Y_ skill3
#define OFFSET_Z_ skill4

#define TOGGLEABLE FLAG3
#define USE_SWITCH FLAG4
#define USE_TRIGGER FLAG5
#define RED_KEY FLAG6
#define YELLOW_KEY FLAG7
#define BLUE_KEY FLAG8

// water skills
#define AMPLITUDE skill1
#define WATER_SPEED skill2
#define MODULATION skill3

// cct ground info defines
#define SOLID 0
#define STAIRS 1
#define MOVING 2

// water detection states
#define OUT_OF_WATER 0
#define ON_WATER 1
#define IN_WATER 2
#define HEAD_IN_WATER 3

// player's weapon states
#define PLAYER_WPN_WAIT 0
#define PLAYER_WPN_DRAW 1
#define PLAYER_WPN_IDLE 2
#define PLAYER_WPN_SHOOT 3
#define PLAYER_WPN_HIDE 4

// player's weapons ID
#define PLAYER_SAW 0
#define PLAYER_PISTOL 1
#define PLAYER_SHOTGUN 2
#define PLAYER_SSHOTGUN 3
#define PLAYER_CHAINGUN 4
#define PLAYER_ROCKETLAUNCHER 5
#define PLAYER_LASERGUN 6

// skills used by projectiles (and barrels)
#define ORIGIN_X skill1
#define ORIGIN_Y skill2
#define ORIGIN_Z skill3
#define DIR_X skill4
#define DIR_Y skill5
#define DIR_Z skill6
#define VEL_X skill7
#define VEL_Y skill8
#define VEL_Z skill9
#define OWNER_GROUP skill10

// projectiles states
#define PROJ_FLY 0
#define PROJ_HIT_ALIVE 1
#define PROJ_HIT_STATIC 2
#define PROJ_DELETE 3

// skills used by casing
#define CASING_VEL_X skill1
#define CASING_VEL_Y skill2
#define CASING_VEL_Z skill3
#define CASING_DIST skill4
#define CASING_HIT_COUNT skill5
#define CASING_IS_SHELL skill6

// skills used by all items
#define ITEM_HEALTH_SMALL 1
#define ITEM_HEALTH_BIG 2
#define ITEM_ARMOR_SMALL 3
#define ITEM_ARMOR_BIG 4
#define ITEM_AMMO_SMALL 5
#define ITEM_AMMO_BIG 6
#define ITEM_PISTOL_AMMO 7
#define ITEM_ROCKET_BIG 8
#define ITEM_ROCKET_SMALL 9
#define ITEM_SHELLS_BIG 10
#define ITEM_SHELLS_SMALL 11
#define ITEM_SUIT 12
#define ITEM_KEY_RED 13
#define ITEM_KEY_YELLOW 14
#define ITEM_KEY_BLUE 15

#define ID skill1
#define ANIM_FRAMES skill2
#define ANIM_TOTAL_FRAMES skill3
#define ANIMATED FLAG8

// collusion groups/push values
#define PUSH_GROUP 2
#define WATER_GROUP 3
#define SWITCH_ITEM_GROUP 4
#define PATHFIND_GROUP 5
#define PLAYER_GROUP 6
#define ENEMY_GROUP 7
#define SHOOTABLE_GROUP 8
#define OBSTACLE_GROUP 9
#define LEVEL_GROUP 10

// object skills
#define OBJ_STRUCT skill50
#define OBJ_CCT_STRUCT skill51
#define OBJ_TYPE skill52
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
#define TYPE_PLAYER_WEAPON 10
#define TYPE_BULLET 11
#define TYPE_ROCKET 12
#define TYPE_CASING 13
#define TYPE_WATER_PLANE 14
#define TYPE_BARREL 15
#define TYPE_SAW_TRACE 16
#define TYPE_WORLD_SAW 17
#define TYPE_WORLD_PISTOL 18
#define TYPE_WORLD_SHOTGUN 19
#define TYPE_WORLD_SSHOTGUN 20
#define TYPE_WORLD_CHAINGUN 21
#define TYPE_WORLD_ROCKETLAUNCHER 22
#define TYPE_WORLD_LASERGUN 23
#define TYPE_PICKABLE_ITEM 24

#define OBJ_HEALTH skill53
#define OBJ_ARMOR skill54
#define OBJ_MOVE_CCT skill55
#define OBJ_ALLOW_MOVE skill56
#define OBJ_STATE skill57
#define OBJ_DEATH_TYPE skill58
#define OBJ_PAIN_TYPE skill59
#define TYPE_UNDEFINED 0
#define TYPE_SHOOT 1
#define TYPE_MELEE 2
#define TYPE_EXPLO 3
#define TYPE_SMASHED 4
#define TYPE_OUT_OF_AIR 5
#define TYPE_FALL_DAMAGE 6

#define OBJ_SND_HANDLE skill60
#define OBJ_TIMER skill61
#define OBJ_CHECK skill62

// trace/scan states
#define OBJ_C_INDICATOR skill63
#define NONE 0
#define INTERACT 1
#define SHOOT 2

// custom flags for "more than 8"
#define OBJ_FLAGS skill64
#define FLAG_DELETE_LATER 1

#define OBJ_TAKE_DAMAGE skill65
#define OBJ_EXPLO_DAMAGE skill66
#define OBJ_EXPLO_RANGE skill67
#define OBJ_ANIM_STRUCT skill68

// sky entity
ENTITY *sky_ent;

// water region
STRING *reg_water_str = "water_reg";

// level logic stuff
var level_id = 0;            // current level id, increased at the end of each level
var level_is_loaded = false; // true - if level is loaded, otherwise - false
var level_secrets_found = 0; // +1 with each new found secret zone (per level)
var level_secrets_total = 0; // +1 with each created secret zone (per level)

// game logic stuff
var game_state = GAME_LOADING; // represents current game state
var game_is_running = false;   // 1 - when game is running, otherwise - 0 (used to stop main game loop)
var game_ticks = 0;            // alternative to native total_ticks

// music
var music_handle = 0;  // used for storing music stream
var music_volume = 35; // volume for level's music

// bboxes for checks
var biggest_bbox_x_side = 16; // consider this as a bixest bbox X size (16+16=32)
var biggest_bbox_y_side = 16; // same as above, but for Y
var biggest_bbox_z_side = 32; // same as above, but for Z

// cct stuff
var cct_gravity = 4;                      // gravity strength
var cct_gravity_max = 90;                 // gravity strength max
var cct_gravity_fluid = 1;                // gravity strength in fluid
var cct_gnd_fric = 0.5;                   // ground friction
var cct_air_fric = 0.1;                   // air friction
var cct_water_fric = 0.75;                // water friction
var cct_grav_trace_start_offset = 4;      // offset for start position of the gravity trace
var cct_grav_trace_end_offset = 24;       // length of the gravity trace
var cct_def_foot_step_height = 8;         // default footstep height
var cct_ground_z_offset = 2;              // offset from ground
var cct_slope_fac = 0.5;                  // walkable slope angle
var cct_reduce_input_in_air_factor = 0.2; // reduce input forces when we are in air
var cct_underwater_no_air_damage = 10;    // damage thaken underwater, when out of air
var cct_fall_damage_limit = 9;            // if falling limit is greater than this value - take damage
var cct_fall_damage_const = 50;           // used for falling damage calculations
var cct_interact_distance = 64;           // interaction distance (both front and downwards)
var cct_allow_cheap_edge_climb = false;   // if true, player will be able to climb edges :D
var cct_snd_volume = 450;                 // cct world sounds volume

// player's stuff
var player_has_red_key = false;    // true if player has red key, otherwise - false
var player_has_yellow_key = false; // same as above for yellow key
var player_has_blue_key = false;   // same as above for blue key
var player_has_suit = false;       // true if player gots suit item
var player_suit_counter = 0;       // timer for suit use
var player_suit_def_time = 15;     // suit could be used for 15 seconds
var player_snd_volume = 75;        // volume for player's voice sounds
var player_pick_snd_handle = 0;    // handle for player's pickup sound effects
var player_allow_movement = false; // true - if player is allowed to move, otherwise - false
var player_always_run = false;     // true - if player is allowed to run always, otherwise - false
var player_health = 0;             // stores player's health for gui
var player_armor = 0;              // stores player's armor for gui
var player_ammo = 0;               // stores player's ammo for gui

// player's camera stuff
var mouse_speed_factor = 1;        // speed factor for mouse, can be changed from settings menu
var camera_mode = 0;               // 1 - third person camera, otherwise first person
var camera_underwater_volume = 65; // volume for underwater sound effect

// props stuff
var props_switch_snd_volume = 450;        // volume for all switch sound effect
var props_elevator_snd_volume = 450;      // volume for elevator non looping sound effects
var props_elevator_snd_loop_volume = 150; // volume for elevator looping sound effects
var props_door_snd_volume = 450;          // volume for all door sound effects
var props_platform_snd_volume = 450;      // volume for platform non looping sound effects
var props_platform_snd_loop_volume = 150; // volume for platform looping sound effects
var props_secret_wall_snd_volume = 450;   // volume for all secret wall related sound effects
var props_secret_zone_snd_volume = 50;    // volume for 'found secret' sound effect

// water stuff
var water_small_impact_volume = 48; // volume water small impact sound effect
var water_hard_impact_volume = 256; // volume water small impact sound effect

// weapon stuff
var weapon_id = PLAYER_PISTOL;     // currently equiped weapon's ID
var weapon_bob_y = 0;              // weapon's sway effect (left-right)
var weapon_bob_z = 0;              // weapon's sway effect (up-down)
var weapon_z_land = 0;             // weapon's landing effect
var weapon_do_recoil = false;      // true - if we need to have recoil effect, otherwise - false
var weapon_draw_volume = 45;       // volume for player's weapon draw sound effect
var weapon_idle_volume = 35;       // volume for player's weapon idle sound effect (saw)
var weapon_shoot_volume = 65;      // volume for player's weapon shoot sound effect
var weapon_draw_time = 0.25;       // default drawing time for weapons
var weapon_in_use = false;         // true - if player's has equiped weapon, otherwise - false
var weapon_fire_key_busy = false;  // true - if fire button is busy, otherwise - false
var weapon_ric_volume = 96;        // volume for ricochet sounds effects
var weapon_casing_snd_volume = 64; // weapon casing impact sound effect volume
var weapon_casing_snd_counter = 0; // counts amount of existing casings

// pickable weapon's stuff
var wpn_pistol_add_ammo = 25;    // default ammo addition when pistol is picked up
var wpn_pistol_max_ammo = 200;   // max ammo for pistol
var wpn_shotgun_add_ammo = 25;   // default ammo addition when shotgun is picked up
var wpn_shotgun_max_ammo = 100;  // max ammo for shotgun
var wpn_chaingun_add_ammo = 50;  // default ammo addition when chaingun is picked up
var wpn_chaingun_max_ammo = 200; // max ammo for chaingun
var wpn_rocketl_add_ammo = 3;    // default ammo addition when rocket launcher is picked up
var wpn_rocketl_max_ammo = 50;   // max ammo for rocket launcher
var wpn_lasergun_add_ammo = 25;  // default ammo addition when laser is picked up
var wpn_lasergun_max_ammo = 100; // max ammo for lasergun
var wpn_pickup_snd_volume = 45;  // pickup sfx volume

var item_pickup_snd_volume = 65;  // volume for pickup sfx
var item_health_small_add = 1;    // each small health adds 1 hp
var item_health_small_max = 200;  // and can increase health up to 200 hp
var item_health_big_add = 25;     // big health pack adds 25 hp
var item_health_big_max = 100;    // and can increase health up to 100 hp
var item_armor_small_add = 1;     // each small armor adds 1 point to armor
var item_armor_small_max = 200;   // and can increase armor up to 200 points
var item_armor_big_add = 25;      // each big armor adds 25 point to armor
var item_armor_big_max = 100;     // and can increase armor up to 200 points
var item_small_pistol_add = 15;   // each small pistol clip/ammo adds 15
var item_big_pistol_add = 25;     // each big pistol ammo adds 25
var item_small_shotgun_add = 12;  // each small shell ammo adds 12
var item_big_shotgun_add = 25;    // each big shell ammo adds 25
var item_small_chaingun_add = 25; // each small chaingun ammo adds 25
var item_big_chaingun_add = 50;   // each big chaingun ammo adds 50
var item_small_rocketl_add = 1;   // each small rocket launcher ammo adds 1
var item_big_rocketl_add = 5;     // each big rocket launcher ammo adds 5
var item_small_lasergun_add = 10; // each small lasergun ammo adds 10
var item_big_lasergun_add = 25;   // each big lasergun ammo adds 25

// explosion stuff
var explo_underwater_snd_volume = 1024; // volume for underwater explosion sound effect
var explo_snd_volume = 1024;            // volume for all explosion sound effects
var explo_default_range = 128;          // default range for the explosion damage !

// chainsaw hit sounds
var player_saw_hit_snd_handle = 0;  // handle for player's chainsaw hit sound effect
var player_saw_hit_snd_volume = 55; // volume of player's chainsaw hit sound effect

// shader pipeline
VIEW *TempView;
var pp_underwater_is_on = false; // true - if underwater effect is on, otherwise - false
var pp_blur = 0;                 // amount of screen blur
var pp_saturation = 50;          // screen saturation
var pp_sepia = 5;                // sepia strength
var pp_darken = 5;               // screen darkness (greater - darker)
var pp_contrast = 125;           // screen contrast (smaller - darker)
var pp_sharpen_strength = 3;     // screen sharpen effect's strength
var mtl_brightness = 0.9;        // level of brightness for all level objects (ignores water)

// delay functions
void delay(var time);
void ent_delay(ENTITY *ent, var time);

// math functions
var math_round(num);
var math_check_divide(value, divide);

// assert func
void _assert(int v, char const *msg);

// safe remove entity (at the end of the frame)
void ent_delete_later(ENTITY *ent);

// safe remove entity
void ent_delete(ENTITY *ent);

// alternative for c_move
var ent_move(ENTITY *ent, VECTOR *reldist, VECTOR *absdist, var mode);

// alternative for c_trace
var ent_trace(ENTITY *ent, VECTOR *from, VECTOR *to, var mode);

// alternative for c_scan
var ent_scan(ENTITY *ent, VECTOR *pos, ANGLE *ang, VECTOR *sector, var mode);

// rotate entity towards the camera
void ent_rotate_to_camera(ENTITY *ent);

// place entity on the ground
// this function uses c_trace instead of ent_trace
// since it's called in action functions where my already exists !
void ent_place_on_ground(ENTITY *ent);

// used by animator for 8 directional sprites
var ang2(var p);

#endif