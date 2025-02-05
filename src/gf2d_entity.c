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
	if (!self) return;

	if (self->draw) {
		if (self->draw(self) == -1)return;
	}
	else {
		gf2d_sprite_draw(
			self->sprite,
			self->position,
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