#ifndef _PLAYER_CAMERA_H_
#define _PLAYER_CAMERA_H_

// This header includes everything related to player's camera

// get input from player
void camera_get_input(PLAYER *hero);

// animate camera
void camera_bobbing(CCT *cct, PLAYER *hero);

// reset bobbing effect
void camera_bobbing_reset(CCT *cct, PLAYER *hero);

// change camera's fog, if we are inside of the water region
void set_camera_fog(CCT *cct, PLAYER *hero);

// handle recoil effect
void camera_recoil(ENTITY *ent, PLAYER *hero);

// handle camera shaking effect (used for explosions and moving on platforms/elevators)
void camera_explosion_shake(PLAYER *hero);

// update everything related to the camera
void camera_update(ENTITY *ent, CCT *cct, PLAYER *hero);

#endif