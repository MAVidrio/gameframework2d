#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gf2d_entity.h"

typedef struct Weapon_Data {
	WeaponType w_type;
	double id;

	int amount;
	int damage;
	int pierce;
	float speed;
	int level;
	int max_level;
	float fire_time;

	// If <= -0.2, no evo
	// If -0.1, no requirements to evolve
	double require_evo_1;
	double require_evo_2;
	double require_evo_3;

	Sprite* projectile;
	Sprite* weapon_Sprite;
}WeaponData;

typedef struct Weapon_Struct {
	Uint8			_inuse;		/**<Used to check to see if the weapon slot is in use or not>*/
	float wf;		// Float for attack


	void (*think)(struct Weapon_S* self);		/**Function to call for firing weapon*/
	void (*update)(struct Weapon_S* self);		/**Function to call for changing stats*/
	void (*free)(struct Weapon_S* self);

	void* data;		/**<Used for extra data>*/
}Weapon;

void weapon_system_close();

/**
* @brief initialize the weapon sub weapon_system_init
* @param maxEnts - Max amount of weapons that the player can have
*/
void weapon_system_init(Uint32 maxWeaps);

/**
* @brief free all weapons in the manager
*/
void weapon_system_free_all();

/*
* @brief free the weapon
*/
void weapon_remove(Weapon* self);

/*
* @brief Make a new weapon
* @return NULL on error, or a pointer of the weapon otherwise
*/
Weapon* new_weapon();

/*
* @brief Make a new weapon from pre-set json
* @param id - ID of the weapon
* @return NULL on error, or a pointer of the weapon otherwise
*/
Weapon* add_weapon(const char* id);

/*
* @brief Makes all active weapons run their think functions
* @note Used for simple calling for thinking of all weapons.
*/
void weapon_think_all(GFC_Vector2D shootPos, GFC_Vector2D direction);

/*
* @brief Makes all active weapons run their update functions
* @note Used for simple calling for updating of all weapons.
*/
void weapon_update_all();

/*
* @brief Calls all weapons to update their stats
* @note Used for updating stats of all weapons when accessory is obtained/leveled up
*/
void accessory_update_all();

#endif