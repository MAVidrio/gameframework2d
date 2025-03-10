#include "simple_logger.h"

#include "player.h"
#include "gfc_input.h"
#include "camera_2d.h"
#include "projectile.h"

void player_think(Entity* self);
void player_update(Entity* self);
int player_draw(Entity* self);
void player_free(Entity* self);

Entity* player_new(Sprite *sprite) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn player entity.");
		return NULL;
	}

	// Define
	self->tag = PLAYER;

	// 
	self->position = gfc_vector2d(1200*0.45, 720*0.45);
	self->sprite = sprite;
	self->pf = 0;
	self->ff = 0;
	self->direction = gfc_vector2d(1, 0);
	self->hitbox = gfc_rect(self->position.x+27, self->position.y+27, 70, 70);

	self->think = player_think;
	self->update = player_update;
	self->draw = player_draw;
	self->free = player_free;

	self->need_bounds = 0;

	slog("Player succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

Entity* player_new_from_file(const char* filename)
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

	player_config_from_file(self, filename);

	self->think = player_think;
	//self->update = player_update;
	//self->draw = player_draw;
	//self->free = player_free;

	slog("Player succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void player_config_from_file(Entity* self, const char* filename)
{
	SJson* json;
	json = sj_load(filename);

	//player_config(self,json);

	sj_object_free(json);
}

void player_config(Entity* self, SJson* json)
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

void player_update(Entity* self) {
	if (!self) return;
	Sprite* projectile;
	GFC_Vector2D shoot_position;

	shoot_position.x = self->position.x;
	shoot_position.y = self->position.y;

	self->hitbox.x = self->position.x;
	self->hitbox.y = self->position.y;

	// Fire projectiles
	if (gfc_input_command_pressed("Debug_Fire") /* || fmodf(self->ff, 240) == 0*/) {
		projectile = gf2d_sprite_load_all("images/laser.png", 128, 128, 1, 0);

		slog("Debug firing");
		//slog("Movement: %f,%f", self->direction.x, self->direction.y);
		projectile_new(projectile, gfc_vector2d(shoot_position.x,shoot_position.y), self->direction);

		/* How to do projectiles*/
		//1. Go throught weapon manager
		//2. 
	}

	self->ff += 1;
}

void player_think(Entity* self) {
	if (!self)return;

	GFC_Vector2D movement = gfc_vector2d(0, 0);
	GFC_Vector2D dir;
	gfc_vector2d_copy(dir, self->direction);

	// Controls
	if (gfc_input_command_down("left")) {
		movement.x -= 1;
		dir.x -= 1;
	}
	if (gfc_input_command_down("right")) {
		movement.x += 1;
		dir.x += 1;
	}
	if (gfc_input_command_down("up")) {
		movement.y -= 1;
		dir.y -= 1;
	}
	if (gfc_input_command_down("down")) {
		movement.y += 1;
		dir.y += 1;
	}

	// Add frames to sprite animation
	if(movement.x != 0 || movement.y != 0) self->pf += 0.1;

	// Keep direction when there's no movement
	if (movement.x == 0 && movement.y == 0) {
		dir = dir;
	}
	// Set direction to zero if there is no movement in that direction
	else {
		if (movement.x == 0) dir.x = 0;
		if (movement.y == 0) dir.y = 0;
	}

	// Prevent direction going over 1 or -1 so that it can work as direction
	if (dir.x > 1) dir.x = 1;
	else if (dir.x < -1) dir.x = -1;
	if (dir.y > 1) dir.y = 1;
	else if (dir.y < -1) dir.y = -1;

	// Loop Sprite animation
	if (self->pf >= 3.0)self->pf = 0;

	camera_center_on(self->position);

	gfc_vector2d_add(self->position, self->position, movement);
	gfc_vector2d_copy(self->direction, dir);
}

int player_draw(Entity* self)
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
		(int)self->pf);
	return;
}

void player_free(Entity* self) {
	if (!self)return;
	playerData* pData = self->data;
	if (!pData) return;
	free(pData);
	memset(self, 0, sizeof(Entity));
}

GFC_Vector2D player_get_position(Entity *self) {
	if (!self)return;
	return self->position;
}