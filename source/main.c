
#include <acknex.h>
#include <default.c>
#include <windows.h>
#include <strio.c>

#define PRAGMA_POINTER

#define PRAGMA_PATH "resources"
#define PRAGMA_PATH "resources\\models"
#define PRAGMA_PATH "resources\\sounds"
#define PRAGMA_PATH "resources\\sounds\\player"
#define PRAGMA_PATH "resources\\sounds\\props"
#define PRAGMA_PATH "resources\\sprites"
#define PRAGMA_PATH "code"
#define PRAGMA_PATH "code\\cct"
#define PRAGMA_PATH "code\\fx"
#define PRAGMA_PATH "code\\player"
#define PRAGMA_PATH "code\\props"

#include "defines.h"
#include "resources.h"
#include "particles.h"
#include "shader_pipeline.h"
#include "props.h"
#include "cct.h"
#include "level_logic.h"
#include "player.h"

void on_ent_remove_event(ENTITY *ent){
	
	if(ent->obj_type == TYPE_PLAYER){
		
		delete_cct(ent);
		delete_player_struct(ent);
	}
	
	if(ent->obj_type == TYPE_ELEVATOR || ent->obj_type == TYPE_DOOR || ent->obj_type == TYPE_PLATFORM || ent->obj_type == TYPE_SECRET_WALL){
		
		delete_props(ent);
	}
}

void mouse_lock_in_window(){
	
	static var autolock_mouse_locked = false;
	
	if(!autolock_mouse_locked && window_focus){
		
		autolock_mouse_locked = true;
		RECT rect;
		GetClientRect(hWnd, &rect);
		ClientToScreen(hWnd, &rect);
		ClientToScreen(hWnd, &rect.right);
		ClipCursor(&rect);
	}
	if(autolock_mouse_locked && !window_focus){
		
		autolock_mouse_locked = false;
		ClipCursor(NULL);
	}
}

void set_video_settings(){
	
	video_set(800, 600, 16, 2);
	video_aspect = 1.333;
}

void set_engine_settings(){
	
	warn_level = 6;
	
	doppler_factor = 0;
	
	max_entities = 10000;
	preload_mode = 3 + 4;
	
	d3d_near_models = 0.5;
	d3d_near_sprites = 0.5;
	d3d_near_decals = 0.99;
	
	fps_max = 60;
	fps_min = 30;
	time_smooth = 0.9;
	
	random_seed(0);
	
	mouse_pointer = 0;
}

void main_update()
{
	if(!game_running)
	return;
	
	game_ticks += time_frame;	
	mouse_lock_in_window();
	
	if(key_e) { fps_max = 20; }
	else { fps_max = 60; }
	

	#define FOR_ENTITY_OF_TYPE(_Var, _Type) for((_Var) = ent_next(NULL); (_Var) != NULL; (_Var) = ent_next((_Var))) if((_Var)->obj_type == (_Type))
	
	if(game_level_is_loaded)
	{
		ENTITY * ent;

		FOR_ENTITY_OF_TYPE(ent, TYPE_SWITCH) {
			switch_update(ent);
		}

		FOR_ENTITY_OF_TYPE(ent, TYPE_ELEVATOR) {
			elevator_update(ent);
		}

		FOR_ENTITY_OF_TYPE(ent, TYPE_DOOR) {
			door_update(ent);
		}

		FOR_ENTITY_OF_TYPE(ent, TYPE_PLATFORM) {
			platform_update(ent);
		}

		FOR_ENTITY_OF_TYPE(ent, TYPE_SECRET_WALL) {
			secret_wall_update(ent);
		}	

		FOR_ENTITY_OF_TYPE(ent, TYPE_PLAYER) {
			player_update(ent);
		}
	}

	// cleanup all entities that should be deleted
	{
		ENTITY * ent = ent_next(NULL);
		while(ent)
		{
			you = ent;
			ent = ent_next(ent);
			if(you->OBJ_FLAGS & OBJ_DELETE_LATER) {
				ptr_remove(you);
			}
		}
	}
}

void main(){
	
	on_ent_remove = on_ent_remove_event;
	on_f4 = pipeline_update;
	on_f5 = pipeline_resolution;
	
	game_running = true;
	
	set_level_names();
	set_video_settings();
	set_engine_settings();
	
	level_load_();
	pipeline_start();


	on_caps = player_toggle_run;
	on_r = level_restart;
	
	on_frame = main_update;
}

#include "cct.c"
#include "cct_helper.c"
#include "cct_movement.c"
#include "cct_water_detection.c"
#include "defines.c"
#include "level_logic.c"
#include "particles.c"
#include "player.c"
#include "player_behaviour.c"
#include "player_camera.c"
#include "player_state_machine.c"
#include "player_weapon.c"
#include "props.c"
#include "props_door.c"
#include "props_elevator.c"
#include "props_helper.c"
#include "props_liquid.c"
#include "props_platform.c"
#include "props_secret.c"
#include "props_switch.c"
#include "props_trigger.c"
#include "shader_pipeline.c"
#include "interaction.c"
