#include "interaction.h"

// enable interactions for this entity
void interaction_enable(ENTITY * ent)
{
  ASSERT(ent != NULL);
  ent->OBJ_FLAGS |= OBJ_IS_INTERACTIBLE;
}

// disable interactions for this entity
void interaction_disable(ENTITY * ent)
{
  ASSERT(ent != NULL);
  ent->OBJ_FLAGS &= ~OBJ_IS_INTERACTIBLE;
}

// checks if interaction is enabled for this entity
bool interaction_is_enabled(ENTITY * ent)
{
  ASSERT(ent != NULL);
  return (ent->OBJ_FLAGS & OBJ_IS_INTERACTIBLE) != 0;
}

// resets the interaction state to "not interacted"
void interaction_reset(ENTITY * ent)
{
  ASSERT(ent != NULL);
  ASSERT(interaction_is_enabled(ent));
  ent->OBJ_FLAGS &= ~OBJ_HAD_INTERACTION;
}

// sets the interaction to "i was interacted with!"
void interaction_do(ENTITY * ent, ENTITY * source)
{
  ASSERT(ent != NULL);
  ASSERT(interaction_is_enabled(ent));

  diag(_chr(str_printf(NULL, "\ntry interaction on %p from %p: ", ent, source)));

  if(total_ticks <= ent->OBJ_NEXT_INTERACTION) {
    // suppress interaction here
    double ticks = (double)(ent->OBJ_NEXT_INTERACTION - total_ticks);
    diag(_chr(str_printf(NULL, "suppressed for %f ticks", ticks)));
    return;
  }
  diag("ok.");
  ent->OBJ_NEXT_INTERACTION = total_ticks + 16 * INTERACTION_SUPPRESSION_TIME;
  ent->OBJ_FLAGS |= OBJ_HAD_INTERACTION;
  ent->OBJ_INTERACTION_SOURCE = handle(source);
}

bool interaction_was_triggered(ENTITY * ent)
{
  ASSERT(ent != NULL);
  ASSERT(interaction_is_enabled(ent));

  bool good = (ent->OBJ_FLAGS & OBJ_HAD_INTERACTION) != 0;
  ent->OBJ_FLAGS &= ~OBJ_HAD_INTERACTION;
  return good;
}

ENTITY * interaction_get_source(ENTITY * ent)
{
  ASSERT(ent != NULL);
  ASSERT(interaction_is_enabled(ent));

  if(ent->OBJ_INTERACTION_SOURCE != NULL)
    return ptr_for_handle(ent->OBJ_INTERACTION_SOURCE);
  else
    return NULL;
}