#include "simple_logger.h"

#include "enemy.h"
#include "gfc_input.h"
#include "camera_2d.h"
#include "projectile.h"
#include "loot.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
int enemy_draw(Entity* self);
void enemy_free(Entity* self);
void enemy_cam_coll(Entity* self);
void enemy_coll(Entity* self, Entity* other);

Entity* rand_enemy_new(GFC_Vector2D playerPos) {
	Entity* self;
	int randInt;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn enemy entity.");
		return NULL;
	}

	// Define
	self->tag = ENEMY;

	/*-A randomizer for sprites-*/
	int i;
	i = rand() % 5;

	switch (i) {
	case 0:
		self->sprite = gf2d_sprite_load_image("images/enemies/enemy1.png");
		break;
	case 1:
		self->sprite = gf2d_sprite_load_image("images/enemies/enemy2.png");
		break;
	case 2:
		self->sprite = gf2d_sprite_load_image("images/enemies/enemy3.png");
		break;
	case 3:
		self->sprite = gf2d_sprite_load_image("images/enemies/enemy4.png");
		break;
	case 4:
		self->sprite = gf2d_sprite_load_all("images/ch1.png", 128, 128, 3, 0);
		break;
	default:
		break;
	}

	/*-----------------------------*/

	// 
	self->position = playerPos;
	self->pf = 0;
	self->ff = 0;
	self->direction = gfc_vector2d(1, 0);
	self->hitbox = gfc_rect(self->position.x + 27, self->position.y + 27, 50, 50);

	self->health = 2;
	self->damage = 1;

	self->think = enemy_think;
	self->update = enemy_update;
	self->draw = enemy_draw;
	self->free = enemy_free;
	self->cam_coll = enemy_cam_coll;
	self->collision = enemy_coll;

	self->need_pos = 1;
	self->need_bounds = 1;

	//slog("Enemy succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

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
	self->hitbox = gfc_rect(self->position.x+27, self->position.y+27, 70, 70);

	self->health = 2;

	self->think = enemy_think;
	self->update = enemy_update;
	self->draw = enemy_draw;
	self->free = enemy_free;
	self->cam_coll = enemy_cam_coll;
	self->collision = enemy_coll;

	self->need_bounds = 1;

	slog("Enemy succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void enemy_wave(int amount, GFC_Vector2D playerPos) {
	int i;
	for (i = 0; i <= amount; i++) {
		GFC_Vector2D spawn;
		int randX = rand() % 2;
		int randY = rand() % 2;

		if (randX == 0)spawn.x = playerPos.x - (rand() % 500);
		else spawn.x = playerPos.x + (rand() % 500);
		if (randY == 0)spawn.y = playerPos.y - (rand() % 500);
		else spawn.y = playerPos.y + (rand() % 500);

		//slog("Spawn at %f/%f", spawn.x, spawn.y);

		rand_enemy_new(spawn);
	}
}

void enemy_think(Entity* self) {
	if (!self) return;
	GFC_Vector2D movement = gfc_vector2d(0, 0);
	//GFC_Vector2D dir;
	GFC_Vector2D playerPos = self->player_pos;

	// Player is to the left
	if (self->position.x >= playerPos.x) {
		movement.x -= 0.5;
		//dir.x -= 1;
	}
	// Player is to the right
	else if (self->position.x <= playerPos.x) {
		movement.x += 0.5;
		//dir.x += 1;
	}
	// Player is below
	if (self->position.y >= playerPos.y) {
		movement.y -= 0.5;
		//dir.y += 1;
	}
	// Player is above
	else if (self->position.y <= playerPos.y) {
		movement.y += 0.5;
		//dir.y -= 1;
	}

	gfc_vector2d_add(self->position, self->position, movement);
	//gfc_vector2d_copy(self->direction, dir);
}
void enemy_update(Entity* self) {
	if (!self) return;

	self->hitbox.x = self->position.x + 27;
	self->hitbox.y = self->position.y + 27;
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

	// Drop loot before death
	xp_new(rand() % 30 + 5, self->position);

	//enemyData* eData = self->data;
	//if (!eData) return;
	//free(eData);
	memset(self, 0, sizeof(Entity));
}

void enemy_cam_coll(Entity* self) {

}

void enemy_coll(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	switch (otherTag) {
	case PROJECTILE:
		slog("Tag: %i", other->damage);
		self->health -= other->damage;
		//other->pierce -= 1;
		if (self->health <= 0) {
			enemy_free(self);
		}
		break;
	default:
		break;
	}
}