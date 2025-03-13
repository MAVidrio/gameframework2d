#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include <SDL.h>

#include "simple_logger.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "camera_2d.h"

typedef enum {
	NA,
	PLAYER,
	ENEMY,
	PROJECTILE,
	PICKUP
}Tag;

typedef enum {
	XP,
	CHEST
}PickupType;

typedef enum {
	LASER,
	SWORD
}WeaponType;

typedef enum {
	CH1,
	CH2
}CharacterType;

typedef struct Entity_S
{
	Uint8			_inuse;		/**<Used to check to see if the entity is in use or not>*/

	Tag				tag;		/**<type of the entity>*/
	Sprite			*sprite;	/**<the image of the entity>*/
	float			pf;			/**<the float value for animations*/
	float			ff;			/**<the float value for seconds, used for attacks*/
	GFC_Vector2D	position;	/**<Where to draw this>*/
	GFC_Vector2D	direction;	/**<Where am I heading to>*/
	GFC_Rect		hitbox;		/**<The collision box for interacting with other entities>*/

	// Enemy
	GFC_Vector2D	player_pos;	/**<Where the player is.>*/
	float			need_pos;	/**<Tell manager that it needs player position>*/

	int				max_health; /**<Amount of total health entity has>*/
	CharacterType	character;	/**<Character of the player>*/
	int				health;		/**<Amount of health entity has>*/
	int				recovery;	/**<Amount of health recovered per second>*/
	int				level;
	int				xp_need;
	int				xp;
	int				growth;
	float			cooldown;
	int				speed;

	//Projectile
	int				pierce;		/**<How many times proj can hit enemies>*/
	int				damage;		/**<How much damage proj does>*/
	WeaponType		weapon;		/**<Weapon that is firing>*/

	// Loot
	PickupType		pickup;		/**<type of pickup if it's a pickup*/
	int				amount;		/**<Amount of xp or items given from chest*/

	// Camera Bounds
	float			need_bounds;	/**<Tell everyone if you need camera bounds or not*/
	GFC_Rect		camera_bounds;	/**<What are the bounds of the camera?*/

	// behavior
	void (*think)(struct Entity_S* self);		/**Function to call to make decisions*/
	void (*update)(struct Entity_S* self);		/**Function to call to execute think's decisions*/
	int  (*draw)(struct Entity_S* self);		/**Function to call to draw it's model*/
	void (*free)(struct Entity_S* self);		/**clean up any custom data*/
	void (*collision)(struct Entity_S* self, struct Entity_S* other);	/**Functoin to call for collision with other entites*/
	void (*cam_coll)(struct Entity_S* self);	/**Function to call to execute camera collision actions*/

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

/*
* @brief Makes all active entities run their update functions
* @note Used for simple calling for updating of all entities.
*/
void entity_update_all();

// If the entity needs player position, it shall recieve it
void entity_get_pos(Entity *ent);

void entity_bounds();

// If the entity needs camera bounds for detecting if it's out of bounds, it shall recieve it
GFC_Rect entity_get_hitbox(Entity* self);

void entity_hitbox();

/*
* @brief Check all entities if they have collided
*/
void entity_system_collision();

#endif