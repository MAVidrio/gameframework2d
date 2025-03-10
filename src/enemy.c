#include "simple_logger.h"

#include "enemy.h"
#include "gfc_input.h"
#include "camera_2d.h"
#include "projectile.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
int enemy_draw(Entity* self);
void enemy_free(Entity* self);
void enemy_cam_coll(Entity* self);

Entity* enemy_new(Sprite* sprite) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn player entity.");
		return NULL;
	}

	// Define
	self->tag = ENEMY;

	// 
	self->position = gfc_vector2d(1000, 0);
	self->sprite = sprite;
	self->pf = 0;
	self->ff = 0;
	self->direction = gfc_vector2d(1, 0);
	self->hitbox = gfc_rect(self->position.x, self->position.y, 20, 20);

	self->think = enemy_think;
	self->update = enemy_update;
	self->draw = enemy_draw;
	self->free = enemy_free;
	self->cam_coll = enemy_cam_coll;

	self->need_bounds = 1;

	slog("Enemy succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void enemy_think(Entity* self) {
	if (!self) return;

	GFC_Vector2D entPos = self->position;
	self->hitbox = gfc_rect(self->position.x, self->position.y + 50, 100, 15);
}
void enemy_update(Entity* self) {
	if (!self) return;
	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;
}

int enemy_draw(Entity* self) {
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

void enemy_free(Entity* self) {
	if (!self)return;
	enemyData* eData = self->data;
	if (!eData) return;
	free(eData);
	memset(self, 0, sizeof(Entity));
}

void enemy_cam_coll(Entity* self) {

}
