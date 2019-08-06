#include "defines.h"
#include "props.h"
#include "interaction.h"

// enable all switches with the given id
void switch_enable_by_id(var num){
	
	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		if(temp_ent->obj_type == TYPE_SWITCH){
			
			// if it needs to be enabled only once, then ignore !
			if(!is(temp_ent, toggleable)){ continue; }
			
			// same id? then disable it
			if(temp_ent->id == num){
				
				temp_ent->OBJ_FLAGS |= OBJ_ENABLED;
			}
		}
	}
}

// disable all switches with the given id
void switch_disable_by_id(var num){

	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		if(temp_ent->obj_type == TYPE_SWITCH){
			
			// same id? then disable it
			if(temp_ent->id == num){
				
				temp_ent->OBJ_FLAGS &= ~OBJ_ENABLED;
				// temp_ent->emask &= ~ENABLE_SHOOT;
			}
		}
	}	
}

// function used to trigger props with the same id
void switch_trigger_props(var prop_id){
	
	ENTITY *temp_ent = NULL;			
	for(temp_ent = ent_next(NULL); temp_ent; temp_ent = ent_next(temp_ent)){
		
		// props
		if(temp_ent->obj_type == TYPE_ELEVATOR || temp_ent->obj_type == TYPE_PLATFORM || temp_ent->obj_type == TYPE_DOOR || temp_ent->obj_type == TYPE_SECRET_WALL){
			
			// not the same id OR not using switch ? ignore
			if(temp_ent->id != prop_id || !is(temp_ent, use_switch)){ continue; }
			
			// already triggered ? ignore
			if(temp_ent->obj_state != IDLE){ continue; }
			
			// if you were made to be triggered
			if(is(temp_ent, toggleable)){
				
				// toggle between open and close states
				temp_ent->obj_allow_move += 1;
				temp_ent->obj_allow_move = cycle(temp_ent->obj_allow_move, 1, 3);
				temp_ent->obj_state = temp_ent->obj_allow_move;
			}
			else{
				
				// other ways, open !
				temp_ent->obj_state = OPEN;
			}
		}
	}
}

#define PROPS_SWITCH_NO_TIMEOUT 0

// switch to trigger other props
// uses: id, toggleable
// FLAG3: toggleable 1
action props_switch(){
	
	vec_fill(&my->scale_x, 1);
	c_setminmax(my);
	set(my, POLYGON);
	
	my->group = SWITCH_ITEM_GROUP;
	my->push = SWITCH_ITEM_GROUP;

	my->obj_type = TYPE_SWITCH;
	my->OBJ_SWITCH_TIMEOUT = PROPS_SWITCH_NO_TIMEOUT;

	my->OBJ_FLAGS |= OBJ_ENABLED;

	interaction_enable(my);
}

void switch_update(ENTITY * ent)
{
	if(interaction_was_triggered(ent))
	{
		if(ent->OBJ_FLAGS & OBJ_ENABLED)
		{
			ENTITY * source = interaction_get_source(ent);
			
			// only player can trigger switches ?
			if(source->obj_type == TYPE_PLAYER)
			{
				// disable all switches with the same id
				switch_disable_by_id(ent->id);
				
				// play switch soundd
				if(snd_playing(ent->obj_snd_handle)){ snd_stop(ent->obj_snd_handle); }
				ent->obj_snd_handle = ent_playsound(ent, switch_ogg, props_switch_snd_volume);
				
				if(ent->OBJ_SWITCH_TIMEOUT == PROPS_SWITCH_NO_TIMEOUT)
					ent->OBJ_SWITCH_TIMEOUT = total_ticks + 16; // 1 second timeout
			}
		}
	}

	if(ent->OBJ_SWITCH_TIMEOUT != PROPS_SWITCH_NO_TIMEOUT)
	{
		if(total_ticks >= ent->OBJ_SWITCH_TIMEOUT) {
			// trigger props with the same id
			switch_trigger_props(ent->id);

			switch_enable_by_id(ent->id);

			ent->OBJ_SWITCH_TIMEOUT = PROPS_SWITCH_NO_TIMEOUT;
		}
	}
}