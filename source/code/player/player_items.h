#ifndef _PLAYER_ITEMS_H_
#define _PLAYER_ITEMS_H_

// visual sprites function
void item_visual_func();

// set animations for the sprite
void item_set_anim(ENTITY *ent, var num);

// create visual sprite for the item
void item_create_sprite();

// pickup item
void item_pickup(ENTITY *ent, var num);

// main event function for all items
void item_event_function();

// function for all items
void item_init(var num);

#endif