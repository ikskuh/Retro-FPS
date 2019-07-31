#ifndef _PLAYER_CAMERA_H_
	#define _PLAYER_CAMERA_H_
	
	#define FIRST_PERSON 0
	#define THIRD_PERSON 1

	var mouse_speed_factor = 1; // speed factor for mouse, can be changed from settings menu
	var camera_mode = 0; // 1 - third person camera, otherwise first person
	var camera_underwater_volume = 65; // volume for underwater sound effect
	
	typedef struct {
		
		VECTOR cam_pos;
		ANGLE cam_ang;
		
		ANGLE recoil_ang;
		VECTOR explo_pos;
		
		var cam_height;
		var cam_fov;
		
		var cam_3d_dist;
		var cam_3d_focus;
		
		var cam_bob;
		var cam_bob_z_offset;
		var cam_bob_speed;
		var cam_bob_speed_factor;
		
		var cam_land_effect;
		
		var cam_explo_power;
		
		var cam_fluid_damage_roll;
		var cam_water_snd_handle;
		
	} CAMERA;
	
	CAMERA *player_cam;
	
	void register_camera(ENTITY *ent);
	
	void delete_camera();
	
	void camera_get_input();
	
	void camera_update(ENTITY *ent, CCT_PHYSICS *physics);
	
	void set_camera_fog();
	
	void camera_recoil(ENTITY *ent);
	
	void camera_explosion_shake();
	
	void cam_aqua_damage(ENTITY *ent, CCT_PHYSICS *physics);
	
	void camera_reset_aqua_damage();

	#include "player_camera.c"
#endif