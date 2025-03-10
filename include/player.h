#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf2d_entity.h"

typedef struct Player_Data {
	float hp;
	float sd;
	float dm;
	//GFC_Vector2D direction;
}playerData;

/**
* @brief spawn a test player
* @return NULL on error, or a pointer of the player otherwise
*/
Entity* player_new(Sprite *sprite);

/**
* @brief spawn a player from a file
* @param filename the file's name
* @return NULL on error, or a pointer of the player otherwise
*/
Entity* player_new_from_file(const char* filename);

/**
* @brief Get data for selected character
* @parem self - the player
* @parem filename - the file to get the selected character data
* @note Used this only when the game is starting and character has been selected
*/
void player_config_from_file(Entity *self, const char* filename);

GFC_Vector2D player_get_position(Entity* self);
#endif