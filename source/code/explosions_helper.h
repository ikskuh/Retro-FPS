#ifndef _EXPLOSIONS_HELPER_H_
#define _EXPLOSIONS_HELPER_H_

// check if there is anothing between two positions, if not, take damage !
// using c_trace here, cause it's gonna be called within event functions
void explo_check_walls(ENTITY *ent, VECTOR *from, VECTOR *to, var dmg);

// explosion scan for entities around
// we use c_scan here, because it's used in while with wait
// so there is gonna be valid ME pointer !
void explo_scan(ENTITY *ent);

#endif