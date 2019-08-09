#ifndef _PLAYER_H_
#define _PLAYER_H_

// This is header of player's structure, it contains all the stuff needed for player
//
// This header also includes such headers as
// - player_weapons.h (includes everything used for player's weapon system)
// - player_camera.h (includes everything used for player's camera)
// - player_state_machine.h (includes everything used to setup player's state machine)
// - player_behaviour.h (everything needed for player action)

// weapon structure
typedef struct WEAPON
{
	var collected;
	var semiauto;

	var ammo;
	var ammo_per_shoot;
	var fire_rate;
	var recoil_strength;
	var damage;
	var accuracy;

	var animate;
	var anim_speed;
	var anim_total_frames;

	void *fnc;
	ENTITY *ent;
	SOUND *snd;
} WEAPON;

// player's camera structure
typedef struct CAMERA
{
	VECTOR position;
	ANGLE angle;

	ANGLE recoil_angle;
	VECTOR explo_vector;

	var height;
	var fov;

	var third_person_dist;
	var third_person_focus;

	var bob;
	var bob_z_offset;
	var bob_speed;
	var bob_speed_factor;

	var land_effect;
	var explo_power;
	var underwater_snd_handle;
	var water_z;
	var water_z_lerp;

} CAMERA;

// player's main structure
typedef struct PLAYER
{
	var death_snd_switch;
	var weapon_snd_switch;
	var weapon_draw_counter;
	CAMERA cam;
	WEAPON weapon[MAX_WEAPONS];

	// input
	var wpn_slot_1;
	var wpn_slot_2;
	var wpn_slot_3;
	var wpn_slot_4;
	var wpn_slot_5;
	var wpn_slot_6;
	var wpn_slot_7;
	var wpn_slot_8;
	var wpn_slot_9;
	var wpn_shoot;
} PLAYER;

// register player's structure
PLAYER *register_player_struct(ENTITY *ent);

// initialize player's structure to default values
var initialize_player_struct(ENTITY *ent, PLAYER *hero);

// get player's structure from the given entity
PLAYER *get_player_struct(ENTITY *ent);

// free player structure from memory from given entity
var delete_player_struct(ENTITY *ent);

// free given player structure from the memory
void delete_player_struct(PLAYER *hero);

#endif