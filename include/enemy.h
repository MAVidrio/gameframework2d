#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "gf2d_entity.h"

typedef struct Enemy_Data {
	float speed;
}enemyData;

/**
* @brief spawn a test enemy
* @return NULL on error, or a pointer of the enemy otherwise
*/
Entity* enemy_new(Sprite* sprite);

/**
* @brief spawn a enemy from a file
* @param filename the file's name
* @return NULL on error, or a pointer of the enemy otherwise
*/
Entity* enemy_new_from_file(const char* filename);

/**
* @brief Get data for selected character
* @parem self - the enemy
* @parem filename - the file to get the selected enemy data
* @note Used only special enemies like bosses and/or shooters
*/
void enemy_config_from_file(Entity* self, const char* filename);

void enemy_get_player_pos(Entity* self, GFC_Vector2D playerPOS);

#endif