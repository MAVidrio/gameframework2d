#ifndef __MENU_H__
#define __MENU_H__

#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gf2d_draw.h"

/**
* @brief Draw main menu 
*/
Uint32 main_menu_run(GFC_Vector2D mousePos, Uint32 mousePress);

Uint32 weapon_menu_run(GFC_Vector2D mousePos, Uint32 mousePress, int* weapon_dis, int* previous_Clicked);

#endif