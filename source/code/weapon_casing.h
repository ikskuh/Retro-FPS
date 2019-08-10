#ifndef _WEAPON_CASING_H_
#define _WEAPON_CASING_H_

// play impact sound
void casing_impact_snd(ENTITY *ent, var is_shell);

// function to update casing
void casing_update(ENTITY *ent);

// init casing/shell
void casing_shell_init(var is_shell);

// function to setup casing
void casing_func();

// function to setup shells
void shell_func();

#endif