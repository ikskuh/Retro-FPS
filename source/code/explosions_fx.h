#ifndef _EXPLOSIONS_FX_H_
#define _EXPLOSIONS_FX_H_

// explosion light flashing effect (out of the water)
void explosion_light_fnc();

// explosion light flashing effect (in the water)
void explosion_light_in_water_fnc();

// explosion's sfx function (when out of the water)
// MAX 4 explosion sounds at the same time !
void explosion_simple_snd_func();

// explosion sfx out of water
void explosion_snd_out_of_water(VECTOR *pos);

// explosion's sfx function (when on water)
// MAX 4 explosion sounds at the same time !
void explosion_on_water_snd_func();

// explosion sfx on water
void explosion_snd_on_water(VECTOR *pos);

// explosion's sfx function (when in water)
// MAX 4 explosion sounds at the same time !
void explosion_in_water_snd_func();

// explosion sfx in water
void explosion_snd_in_water(VECTOR *pos);

// barrel sprite
void explosion_barrel_sprite();

// simple explosion sprite
void explosion_sprite();

// underwater explosion sprite
void explosion_underwater_sprite();

// explosion sprite (for quake like particle effect)
void explosion_particle_sprite();

// function for creating explosions
void explosion_create(VECTOR *pos, var is_barrel);

#endif