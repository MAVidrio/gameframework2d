#include "weapon.h"
#include "player.h"
#include "projectile.h"

typedef struct {
	Uint32 weapon_max;
	Weapon* weapon_list;
}WeaponSystem;

static WeaponSystem weapon_system = { 0 };

void weapon_system_close()
{
	weapon_system_free_all();
	if (weapon_system.weapon_list)free(weapon_system.weapon_list);
	memset(&weapon_system, 0, sizeof(WeaponSystem));
}

void weapon_system_init(Uint32 maxWeaps) {
	if (weapon_system.weapon_list)
	{
		slog("cannot have two instances of an entity system, one is already active");
		return;
	}
	if (!maxWeaps) {
		slog("cannot initialize entity system for zero entities");
		return;
	}
	weapon_system.weapon_list = gfc_allocate_array(sizeof(Entity), maxWeaps);
	if (!weapon_system.weapon_list) {
		slog("failed to allocated global weapon list");
		return;
	}
	weapon_system.weapon_max = maxWeaps;
	atexit(weapon_system_close);
	slog("Weapon system initialized");
}

void weapon_system_free_all() {
	int i;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		if (!weapon_system.weapon_list[i]._inuse)continue;	//Skip this iteration of the loop
		weapon_remove(&weapon_system.weapon_list[i]);
	}
}

Weapon* new_weapon() {
	int i;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		if (weapon_system.weapon_list[i]._inuse)continue;		//Skip any active entites
		memset(&weapon_system.weapon_list[i], 0, sizeof(Weapon));
		weapon_system.weapon_list[i]._inuse = 1;
		return &weapon_system.weapon_list[i];
	}
	slog("no more available entities");
	return NULL;
}

void weapon_remove(Weapon* self)
{
	if (!self) return;
	WeaponData* wData = self->data;
	if (!wData) return;
	/* Sprite is not own by the entity. We need to ask the sprite manager to free it*/
	if (wData->weapon_Sprite) gf2d_sprite_free(wData->weapon_Sprite);
	if (wData->projectile) gf2d_sprite_free(wData->projectile);
	free(wData);
	memset(self, 0, sizeof(Weapon));
}

Weapon* add_weapon(const char* id) {
	Weapon* self;
	SJson* json, *weapon;
	int d_damage = -2;
	const char* damage = NULL;
	self = new_weapon();
	if (!self)
	{
		slog("Failed to load weapon.");
		return NULL;
	}
	if (!id) {
		slog("Missing id.");
		return NULL;
	}

	WeaponData* wData;
	wData = gfc_allocate_array(sizeof(WeaponData), 1);
	if (!wData) {
		slog("Failed to allocate weapon data");
		return NULL;
	}
	memset(wData, 0, sizeof(WeaponData));

	json = sj_load("defs/weapon_id_list.def");	
	weapon = sj_object_get_value(json, id);

	// Give the json data to wData
	sj_object_get_int(weapon, "WeaponType", &wData->w_type);
	sj_object_get_double(weapon, "Id", &wData->id);
	sj_object_get_int(weapon, "Amount", &wData->amount);
	sj_object_get_int(weapon, "Damage", &wData->damage);
	sj_object_get_int(weapon, "Pierce", &wData->pierce);
	sj_object_get_float(weapon, "Speed", &wData->speed);
	sj_object_get_int(weapon, "Max", &wData->max_level);
	sj_object_get_float(weapon, "Time", &wData->fire_time);

	// Get required weapons for evo of this weapon
	sj_object_get_double(weapon, "req_1", &wData->require_evo_1);
	sj_object_get_double(weapon, "req_2", &wData->require_evo_2);
	sj_object_get_double(weapon, "req_3", &wData->require_evo_3);

	slog("%f", wData->fire_time);

	sj_free(json);
	sj_object_free(weapon);
}

void weapon_think_all(GFC_Vector2D shootPos, GFC_Vector2D direction) {

}

void weapon_update_all() {

}