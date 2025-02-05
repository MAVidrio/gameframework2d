#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include <SDL.h>

#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gfc_text.h"
#include "gfc_vector.h"

typedef struct Entity_S
{
	Uint8			_inuse;		/**<Used to check to see if the entity is in use or not>*/

	GFC_TextLine	name;		/**<name of the entity for debugging>*/
	GFC_TextLine	tag;		/**<type of the entity>*/
	Sprite			*sprite;	/**<the image of the entity>*/
	GFC_Vector2D	position;	/**<Where to draw this>*/

	// behavior
	void (*think)(struct Entity_S* self);		/**Function to call to make decisions*/
	void (*update)(struct Entity_S* self);		/**Function to call to execute think's decisions*/
	int  (*draw)(struct Entity_S* self);		/**Function to call to draw it's model*/
	void (*free)(struct Entity_S* self);		/**clean up any custom data*/

	void			*data;		/**<Used for extra data>*/
}Entity;

void entity_system_close();

/**
* @brief initialize the entity sub entity_system_init
* @param maxEnts - Max amount of entities that can exist at once
*/
void entity_system_init(Uint32 maxEnts);

/**
* @brief free all entities in the manager
*/
void entity_system_free_all();

/**
* @brief create a new entity to work with
* @return NULL if out of entites, or a blank entity otherwise
*/
Entity *entity_new();

/*
* @brief free the entity
*/
void entity_free(Entity* self);

/*
* @brief Draw all entities that exist.
* @note Used for all draw functions
*/
void entity_draw_all();

/*
* @brief Makes all active entities run their think functions
* @note Used for simple calling for thinking of all entities.
*/
void entity_think_all();


#endif