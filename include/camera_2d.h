#ifndef __CAMERA_2D_H__
#define __CAMERA_2D_H__

#include <SDL.h>
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gf2d_entity.h"

/*
* @brief get the camera's position in world space
* @return the position
*/
GFC_Vector2D camera_get_position();

/*
* @brief get the offset to draw things relative to the camera
* @return the offset
*/
GFC_Vector2D camera_get_offset();

/*
* @brief set the camera's position in world space
* @param position - where the camera must go
*/
void camera_set_position(GFC_Vector2D position);

/*
* @brief set the camera's size in world space
* @param size - the size of the camera
*/
void camera_set_size(GFC_Vector2D size);

void camera_enable_binding(Bool bindCamera);

void camera_apply_bounds();

void camera_set_bounds(GFC_Rect rect);

Uint8 camera_entity_collision(GFC_Vector2D entity_point);

GFC_Rect camera_get_bounds();

void camera_center_on(GFC_Vector2D target);

#endif