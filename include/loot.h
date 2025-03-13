#ifndef __LOOT_H__
#define __LOOT_H__

#include "gf2d_entity.h"

/**
* @brief spawn an xp orb drop
* @return NULL on error, or a pointer of the xp orb otherwise
*/
Entity* xp_new(int amount, GFC_Vector2D pos);

/**
* @brief spawn a chest drop
* @return NULL on error, or a pointer of the chest otherwise
*/
Entity* chest_new(Sprite* sprite, GFC_Vector2D pos);


#endif // __LOOT_H__
