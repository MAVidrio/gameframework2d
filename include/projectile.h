#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gf2d_entity.h"

typedef struct Projectile_Data {
	int pierce;
	float speed;
}projectileData;

/**
* @brief spawn a projectile entity
* @return NULL on error, or a pointer of the player otherwise
*/
Entity* projectile_new(Sprite* sprite,GFC_Vector2D position, GFC_Vector2D direction);

/**
* @brief spawn a projectile from a file
* @param filename the file's name
* @return NULL on error, or a pointer of the projectile otherwise
*/
Entity* projectile_new_from_file(const char* filename);

/**
* @brief Get data from weapon
* @parem self - the weapon
* @parem filename - the file to get the selected weapon data
* @return NULL on error, or a pointer of the projectile otherwise
* @note Used this for getting the weapon projectile
*/
void projectile_config_from_file(Entity* self, const char* filename);
#endif