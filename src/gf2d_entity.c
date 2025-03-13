#include <SDL.h>
#include "gfc_config.h"
#include "gf2d_entity.h"

typedef struct {
	Uint32 entity_max;
	Entity *entity_list;
}EntitySystem;

static EntitySystem entity_system = {0};

void entity_system_close()
{
	entity_system_free_all();
	if (entity_system.entity_list)free(entity_system.entity_list);
	memset(&entity_system, 0, sizeof(EntitySystem));
}

/**
* @brief initialize the entity sub entity_system_init
* @param maxEnts - Max amount of entities that can exist at once
*/
void entity_system_init(Uint32 maxEnts) {
	if (entity_system.entity_list)
	{
		slog("cannot have two instances of an entity system, one is already active");
		return;
	}
	if (!maxEnts) {
		slog("cannot initialize entity system for zero entities");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!entity_system.entity_list) {
		slog("failed to allocated global entity list");
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
	slog("Entity system initialized");
}

/**
* @brief free all entities in the manager
*/
void entity_system_free_all() {
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		entity_free(&entity_system.entity_list[i]);
	}
}

/**
* @brief create a new entity to work with
* @return NULL if out of entites, or a blank entity otherwise
*/
Entity* entity_new()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)continue;		//Skip any active entites
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		return &entity_system.entity_list[i];
	}
	slog("no more available entities");
	return NULL;
}

/*
* @brief free the entity
*/
void entity_free(Entity* self) 
{
	if (!self) return;
	/* Sprite is not own by the entity. We need to ask the sprite manager to free it*/
	if (self->sprite) gf2d_sprite_free(self->sprite);
	if (self->free) self->free(self);
}

void entity_config(Entity *self,SJson *json) 
{
	GFC_Vector2D frameSize = {0};
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

void entity_draw(Entity* self) {
	GFC_Vector2D offset, position;
	if (!self) return;

	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	if (self->draw) {
		if (self->draw(self) == -1)return;
	}
	else {
		gf2d_sprite_draw(
			self->sprite,
			position,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			0);
	}
}

void entity_draw_all() {
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		entity_draw(&entity_system.entity_list[i]);
	}
}

void entity_update(Entity* self) {
	if (!self) return;

	if (self->update)self->update(self);
}

void entity_update_all() {
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		entity_update(&entity_system.entity_list[i]);
	}
}

void entity_think(Entity* self) {
	if (!self) return;
	// Check if entity can think and make it do it's think function
	if (self->think)self->think(self);
}

void entity_think_all() {
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		entity_think(&entity_system.entity_list[i]);
	}
}

void entity_get_pos(Entity* ent) {
	if (!ent) return;

	int i = 0;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		if (entity_system.entity_list[i].need_pos == 1) {
			entity_system.entity_list[i].player_pos = ent->position;
		}
	}
}

void entity_bounds() {
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		if (entity_system.entity_list[i].need_bounds == 1) {//Check if the object needs 
			if (camera_entity_collision(entity_system.entity_list[i].position) == 0) {
				entity_system.entity_list[i].cam_coll(&entity_system.entity_list[i]);
			}
		}
	}
}

GFC_Rect entity_get_hitbox(Entity* self) {
	if (!self) return;
	if(&self->hitbox != NULL)return self->hitbox;
	return gfc_rect(0,0,1,1);
}

void entity_hitbox() {
	GFC_Vector2D offset;

	offset = camera_get_offset();
	int i = 0;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop
		GFC_Rect entity_hitbox = entity_get_hitbox(&entity_system.entity_list[i]);

		entity_hitbox.x += offset.x;
		entity_hitbox.y += offset.y;

		gfc_rect_draw(entity_hitbox, GFC_COLOR_BLACK);
	}
}

void entity_collision(Entity* self, Entity* other) {
	if (!self) return;
	if (!other) return;

	if (self->collision)self->collision(self, other);
}

void entity_system_collision() {
	int i,j;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;	//Skip this iteration of the loop

		for (j = 0; j < entity_system.entity_max; j++) {
			if (!entity_system.entity_list[j]._inuse)continue;	//Skip this iteration of the loop
			if (i == j)continue;								// Skip if the same entity

			if (gfc_rect_overlap(entity_system.entity_list[i].hitbox, entity_system.entity_list[j].hitbox)) {
				entity_collision(&entity_system.entity_list[i], &entity_system.entity_list[j]);
				entity_collision(&entity_system.entity_list[j], &entity_system.entity_list[i]);
			}
		}
		
	}
}