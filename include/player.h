#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf2d_entity.h"

typedef struct Player_Data {
	float hp;
	float sd;
	float dm;
}playerData;

/**
* @brief spawn a player
* @return NULL on error, or a pointer of the player otherwise
*/
Entity* player_new(const char* filename);

/**
* @brief Get data for selected character
* @parem self - the player
* @parem filename - the file to get the selected character data
* @note Used this only when the game is starting and character has been selected
*/
void player_config_from_file(Entity *self, const char* filename);
#endif