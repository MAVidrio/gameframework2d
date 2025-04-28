#ifndef __MAIN_GAME_H__
#define __MAIN_GAME_H__

#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"
#include "camera_2d.h"
#include "gf2d_entity.h"
#include "player.h"

void main_game_think_and_update(Entity* p);

void main_game_draw(GFC_Vector2D offset, Sprite* sprite, Sprite* player, Bool hitbox_on, float pf);

void debug_game_think(Bool enable_game, Bool enable_collision, Entity* p);

void debug_game_update();

void debug_game_draw(GFC_Vector2D offset, Sprite* sprite, Sprite* player, Bool hitbox_on, float pf);

#endif