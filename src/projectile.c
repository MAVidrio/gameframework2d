#include "simple_logger.h"

#include "projectile.h"
#include "player.h"
#include "camera_2d.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
int projectile_draw(Entity* self);
void projectile_free(Entity* self);
void projectile_cam_coll(Entity* self);

Entity* projectile_new(Sprite* sprite, GFC_Vector2D position, GFC_Vector2D direction) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn projectile entity.");
		return NULL;
	}

	// Define
	self->tag = PROJECTILE;

	self->position = position;
	self->sprite = sprite;
	self->pf = 0;
	self->direction = direction;
	self->hitbox = gfc_rect(self->position.x, self->position.y, 10, 10);

	self->think = projectile_think;
	self->update = projectile_update;
	self->draw = projectile_draw;
	self->free = projectile_free;
	self->cam_coll = projectile_cam_coll;

	// Camera bounds
	self->need_bounds = 1.0;

	slog("Projectile succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

Entity* projectile_new_from_file(const char* filename)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn player entity.");
		return NULL;
	}
	if (!filename) {
		slog("Failed to get character file.");
		return NULL;
	}

	projectile_config_from_file(self, filename);

	self->think = projectile_think;
	self->update = projectile_update;
	self->draw = projectile_draw;
	self->free = projectile_free;

	slog("Projectile succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void projectile_config_from_file(Entity* self, const char* filename)
{
	SJson* json;
	json = sj_load(filename);

	//projectile_config(self,json);

	sj_object_free(json);
}

void projectile_config(Entity* self, SJson* json)
{
	GFC_Vector2D frameSize = { 0 };
	Uint32	framesPerLine = 0;
	const char* sprite = NULL;
	if ((!self) || (!json))return;
	sprite = sj_object_get_string(json, "sprite");
	if (sprite) {
		sj_object_get_uint32(json, "spriteFPL", &framesPerLine);
		self->sprite = gf2d_sprite_load_all(
			sprite,
			(Uint32)frameSize.x,
			(Uint32)frameSize.y,
			framesPerLine,
			0
		);
	}
	//sj_object_get_float(json, "speedMax", &self->speedMax);
	//sj_object_get_vector4d
}

void projectile_update(Entity* self) {
	if (!self) return;
	
	GFC_Vector2D entPos = self->position;
	self->hitbox = gfc_rect(self->position.x, self->position.y+50, 100, 15);
}

void projectile_think(Entity* self) {
	if (!self)return;

	GFC_Vector2D movement = gfc_vector2d(0, 0);

	movement.x = self->direction.x * 2;
	movement.y = self->direction.y * 2;

	gfc_vector2d_add(self->position, self->position, movement);
}

int projectile_draw(Entity* self)
{
	GFC_Vector2D offset, position;
	if (!self) return;

	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	gf2d_sprite_draw(
		self->sprite,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		0);
	return;
}

void projectile_free(Entity* self) {
	if (!self) return;

	//projectileData* pData = self->data;
	//if (!pData) return;
	//free(pData);
	memset(self, 0, sizeof(Entity));
}

void projectile_cam_coll(Entity* self) {
	slog("Doing collision actions");
	slog("Deleting at %f/%f.", self->position.x, self->position.y);
	slog("Camera bounds: %f/%f", self->camera_bounds.x, self->camera_bounds.y);
	projectile_free(self);
}