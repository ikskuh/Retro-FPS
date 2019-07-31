
#include <acknex.h>
#include <default.c>
#include <windows.h>
#include <strio.c>

#define PRAGMA_POINTER

#define PRAGMA_PATH "data"

#include "defines.h"
#include "level_logic.h"
#include "cct_physics.h"
#include "player_behaviour.h"

void on_ent_remove_event(ENTITY *ent){
	
	if(ent->obj_type == TYPE_PLAYER){
		
		delete_cct(ent);
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
}

void main(){
	
	on_ent_remove = on_ent_remove_event;
	
	game_running = true;
	
	set_video_settings();
	set_engine_settings();
	level_load_("map.wmb");
	
	while(game_running == true){
		
		game_ticks += time_frame;
		
		mouse_lock_in_window();
		
		if(key_e){ fps_max = 20; }
		else{ fps_max = 60; }
		
		wait(1);
	}
}