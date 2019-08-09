#ifndef _PLAYER_WPN_LASERGUN_H_
#define _PLAYER_WPN_LASERGUN_H_

// initialize all lasergun parameters
void player_lasergun_init(PLAYER *hero);

// lasergun shooting function
void player_lasergun_shoot(PLAYER *hero);

// animate lasergun
void player_lasergun_animate(PLAYER *hero);

#endif