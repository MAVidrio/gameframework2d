#include "simple_logger.h"

#include "loot.h"
#include "gfc_input.h"
#include "camera_2d.h"

// XP Orb Functions
void xp_think(Entity* self);
void xp_update(Entity* self);
int xp_draw(Entity* self);
void xp_free(Entity* self);
void xp_cam_coll(Entity* self);
void xp_coll(Entity* self, Entity* other);

// Chest Functions
void chest_think(Entity* self);
void chest_update(Entity* self);
int chest_draw(Entity* self);
void chest_free(Entity* self);
void chest_coll(Entity* self, Entity* other);

// XP Orb Functions
Entity* xp_new(int amount, GFC_Vector2D pos) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn xp orb entity.");
		return NULL;
	}

	// Define
	self->tag = PICKUP;
	self->pickup = XP;
	self->amount = amount;

	// 
	self->position = pos;

	if (amount <= 10) {
		self->sprite = gf2d_sprite_load_image("images/gems/blue_gem.png");
	}
	else if (amount <= 20) {
		self->sprite = gf2d_sprite_load_image("images/gems/green_gem.png");
	}
	else {
		self->sprite = gf2d_sprite_load_image("images/gems/red_gem.png");
	}
	
	self->direction = gfc_vector2d(1, 0);
	self->hitbox = gfc_rect(self->position.x+27, self->position.y+27, 25, 25);

	self->think = xp_think;
	self->update = xp_update;
	self->draw = xp_draw;
	self->free = xp_free;
	self->cam_coll = xp_cam_coll;
	self->collision = xp_coll;

	self->need_bounds = 1;

	slog("XP Orb succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void xp_think(Entity* self) {
	if (!self) return;

	GFC_Vector2D entPos = self->position;
}
void xp_update(Entity* self) {
	if (!self) return;
	self->hitbox.x = self->position.x + 27;
	self->hitbox.y = self->position.y + 27;
}

int xp_draw(Entity* self) {
	GFC_Vector2D offset, position;
	if (!self) return;

	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	position.x += 27;
	position.y += 27;

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

void xp_free(Entity* self) {
	if (!self)return;
	free(self->data);
	memset(self, 0, sizeof(Entity));
}

void xp_cam_coll(Entity* self) {

}

void xp_coll(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	if (otherTag == PLAYER) {
		xp_free(self);
	}
}

// Chest Functions
Entity* chest_new(Sprite* sprite, GFC_Vector2D pos) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn chest entity.");
		return NULL;
	}

	// Define
	self->tag = PICKUP;
	self->pickup = XP;
	self->amount = rand() % 5 + 1;

	// 
	self->position = pos;
	self->sprite = sprite;
	self->direction = gfc_vector2d(1, 0);
	self->hitbox = gfc_rect(self->position.x, self->position.y, 5, 5);

	self->think = xp_think;
	self->update = xp_update;
	self->draw = xp_draw;
	self->free = xp_free;
	self->collision = chest_coll;

	self->need_bounds = 0;

	slog("XP Orb succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void chest_think(Entity* self){}

void chest_update(Entity* self){}

int chest_draw(Entity* self) {
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

void chest_free(Entity* self){
	if (!self)return;
	free(self->data);
	memset(self, 0, sizeof(Entity));
}

void chest_coll(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	if (otherTag == PLAYER) {
		xp_free(self);
	}
}