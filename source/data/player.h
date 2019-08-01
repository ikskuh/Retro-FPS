#ifndef _PLAYER_H_
	#define _PLAYER_H_
	
	// This is header of player's structure, it contains all the stuff needed for player
	//
	// This header also includes such headers as
	// - player_weapons.h (all functions used for player's weapon system)
	// - player_camera.h (all functions used for player's camera)
	// - player_behaviour.h (all functions needed for player action)
	
	// camera modes
	#define FIRST_PERSON 0
	#define THIRD_PERSON 1
	
	// player's camera stuff
	var mouse_speed_factor = 1; // speed factor for mouse, can be changed from settings menu
	var camera_mode = 0; // 1 - third person camera, otherwise first person
	var camera_underwater_volume = 65; // volume for underwater sound effect
	
	// player's camera structure
	typedef struct {
		
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
	typedef struct {
		
		var health;
		var armor;
		var always_run;
		var allow_movement;
		
		CAMERA *cam;
		
	} PLAYER;
	
	// player's structure
	PLAYER *hero;
	
	// register and initializes new player's structure
	// and save it's pointer into given entities obj_struct skill
	void register_player_struct(ENTITY *ent);
	
	// initialize all major variables for the given player's structure
	// this function called inside of register_player_struct right after creating new one
	void init_player_struct(ENTITY *ent, PLAYER *temp);
	
	// returns pointer of the player structure from given entity's obj_struct skill
	PLAYER *get_player_struct(ENTITY *ent);
	
	// removes player from the memory
	// pointer is taken from given entity's obj_struct skill
	void delete_player_struct(ENTITY *ent);
	
	// remove given player from the memory
	void delete_player_struct(PLAYER *temp);
	
	#include "player.c"
	#include "player_weapon.h"
	#include "player_camera.h"
	#include "player_behaviour.h"
#endif