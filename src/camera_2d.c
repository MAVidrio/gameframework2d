#include "simple_logger.h"
#include "camera_2d.h"

typedef struct {
	GFC_Vector2D position;	// position of the camera in world space
	GFC_Vector2D size;		// width and height of the screen
	GFC_Rect bounds;		// The edges of the camera, used for entity's offscreen
	Bool bindCamera;
}Camera;

static Camera _camera = { 0 };

void camera_set_bounds(GFC_Rect rect) {
	gfc_rect_copy(_camera.bounds, rect);
}

void camera_enable_binding(Bool bindCamera) {
	_camera.bindCamera = bindCamera;
}

GFC_Vector2D camera_get_position() {
	return _camera.position;
}

GFC_Vector2D camera_get_offset() {
	return gfc_vector2d(-_camera.position.x, -_camera.position.y);
}

void camera_set_position(GFC_Vector2D position) {
	gfc_vector2d_copy(_camera.position, position);
	if (_camera.bindCamera) {
		camera_apply_bounds();
	}
}

void camera_set_size(GFC_Vector2D size) {
	gfc_vector2d_copy(_camera.size, size);
}

void camera_apply_bounds() {
	_camera.bounds.x = _camera.position.x - 135.0;
	_camera.bounds.y = _camera.position.y - 135.0;
}

Uint8 camera_entity_collision(GFC_Vector2D entity_point) {
	return gfc_point_in_rect(entity_point, _camera.bounds);
}

GFC_Rect camera_get_bounds() {
	return _camera.bounds;
}

void camera_center_on(GFC_Vector2D target) {
	GFC_Vector2D position;
	position.x = target.x - (_camera.size.x * 0.45);
	position.y = target.y - (_camera.size.y * 0.45);
	camera_set_position(position);
	if (_camera.bindCamera)camera_apply_bounds();
}