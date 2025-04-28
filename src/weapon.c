#include "weapon.h"
#include "player.h"
#include "projectile.h"
#include "gfc_config.h"
#include "simple_json_object.h"

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

int count_weapons() {
	int i;
	int amount = 0;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		if (!weapon_system.weapon_list[i]._inuse)continue;		//Skip any active entites
		amount++;
	}
	//slog("Amount of weapons: %i", amount);
	return amount;
}

Weapon* create_weapon_from_debug() {
	//sj_object_insert();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! TODO:
//!		Make this be able to modify the wData values of the weapon (except for id)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void modify_custom_weapon() {

}

Weapon* new_custom_weapon_from_debug() {
	Weapon* self;
	SJson* json, *list, *last_id;
	SJList* weaponList;
	
	int* last_key = -2;
	int d_damage = -2, *i, c;
	const char* damage = NULL;
	const char* sprite = NULL;

	self = new_weapon();
	if (!self)
	{
		slog("Failed to load weapon.");
		return NULL;
	}

	WeaponData* wData;
	wData = gfc_allocate_array(sizeof(WeaponData), 1);
	if (!wData) {
		slog("Failed to allocate weapon data");
		return NULL;
	}
	memset(wData, 0, sizeof(WeaponData));

	// Add basic data for this weapon
	json = sj_load("defs/weapon_id_list.def");
	weaponList = sj_object_get_value(json, "Weapons");

	// Get the last weapon on the array
	c = sj_array_count(weaponList);
	list = sj_array_get_nth(weaponList, c-1);
	if (!list) slog("Error");

	// Get last weapon ID
	sj_object_get_value_as_int(list, "Id", &i);
	if (!i) slog("Value Error");
	
	wData->w_type = 1;
	wData->id = i; wData->id += 1;
	wData->amount = 1;
	wData->damage = 1;
	wData->pierce = 1;
	wData->speed = 1;
	wData->max_level = 8;
	wData->fire_time = 30.0f;

	wData->require_evo_1 = -1;
	wData->require_evo_2 = -1;
	wData->require_evo_3 = -1;

	sprite = "images/laser.png";
	wData->projectile = gf2d_sprite_load_all(sprite, 128, 128, 1, 0);
	wData->level = 0;

	self->wf = 0.1;
	self->level = 1;
	self->data = wData;

	slog("Weapon %i loaded successfully.", wData->id);

	sj_free(json);
	sj_object_free(weaponList);
}

void save_custom_weapon_to_json(Weapon *self) {
	SJson* json, *weapon, *type, *id, *amount,*damage, *pierce, *speed, *max, *time, *req1, *req2, *req3, *hitboxW, *hitboxH,*laser_type, *sprite;
	SJson* new_file;
	SJList *weaponList, *weapon_result;
	WeaponData* wData;


	if (!self)
	{
		slog("Failed to load weapon.");
		return NULL;
	}
	wData = self->data;

	weapon = sj_object_new();
	if (!weapon) {
		slog("Null Weapon");
	}

	new_file = sj_object_new();
	if (!new_file) {
		slog("Null Weapon");
	}

	type = sj_new_int(1);
	id = sj_new_int(wData->id);
	amount = sj_new_int(1);
	time = sj_new_int(30);
	req1 = sj_new_float(-0.2);
	req2 = sj_new_float(-0.2);
	req3 = sj_new_float(-0.2);
	hitboxW = sj_new_float(100.0);
	hitboxH = sj_new_float(100.0);
	laser_type = sj_new_int(1);
	sprite = sj_new_str("images/laser.png");
	
	sj_object_insert(weapon, "WeaponType", type);
	sj_object_insert(weapon, "Id", id);
	sj_object_insert(weapon, "Amount", amount);
	sj_object_insert(weapon, "Time", time);
	sj_object_insert(weapon, "req_1", req1);
	sj_object_insert(weapon, "req_2", req2);
	sj_object_insert(weapon, "req_3", req3);
	sj_object_insert(weapon, "hitbox_W",hitboxW);
	sj_object_insert(weapon, "hitbox_H", hitboxH);
	sj_object_insert(weapon, "Laser_Type", laser_type);
	sj_object_insert(weapon, "Sprite_P", sprite);

	sj_echo(weapon);

	json = sj_load("defs/weapon_id_list.def");
	// Get to the weapon list
	weaponList = sj_object_get_value(json, "Weapons");

	sj_array_append(weaponList, weapon);

	sj_object_insert(new_file, "Weapons",weaponList);

	sj_save(new_file, "defs/weapon_id_list.def");

	sj_echo(new_file);

	slog("Weapon %i is saved", wData->id);
}

void weapon_remove(Weapon* self)
{
	if (!self) return;
	WeaponData* wData = self->data;
	if (!wData) return;
	/* Sprite is not own by the entity. We need to ask the sprite manager to free it*/
	if (wData->weapon_Sprite) gf2d_sprite_free(wData->weapon_Sprite);
	if (wData->projectile) gf2d_sprite_free(wData->projectile);
	slog("Weapon removed.");
	free(wData);
	memset(self, 0, sizeof(Weapon));
}

Weapon* add_weapon(const char* id) {
	Weapon* self;
	SJson* json, *weapon;
	int d_damage = -2;
	const char* damage = NULL;
	const char* sprite = NULL;
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

	SJson* type = sj_array_get_nth(weapon, 0);

	

	// Give the json data to wData
	sj_object_get_int(weapon, "WeaponType", &wData->w_type);
	sj_object_get_int(weapon, "Id", &wData->id);
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

	// Get sprites for projectiles and weapon icon
	sprite = sj_object_get_string(weapon, "Sprite_P", sprite);
	wData->projectile = gf2d_sprite_load_all(sprite, 128, 128, 1, 0);
	wData->level = 0;

	self->wf = 0.1;
	self->level = 1;
	self->data = wData;

	slog("Weapon %i loaded successfully.", wData->id);

	sj_free(json);
	sj_object_free(weapon);
}

void weapon_think(Weapon* self, GFC_Vector2D shootPos, GFC_Vector2D direction, int amount) {
	WeaponData* wData = self->data;
	
	float time = wData->fire_time;
	int i;

	switch (self->level) {
	case 2:
		if (self->wf >= time ) {
			for (i = 0; i < amount + wData->amount;i++) {

			}
			weapon_projectile_new(wData->projectile, shootPos, direction, wData->w_type,
				wData->damage, wData->pierce, self->level);
		}
		break;
	default:
		if (self->wf >= (time * 0.8) ) {
			weapon_projectile_new(wData->projectile, shootPos, direction, wData->w_type,
				wData->damage, wData->pierce, self->level);
		}
		break;
	}
}

void weapon_think_all(GFC_Vector2D shootPos, GFC_Vector2D direction, int amount) {
	int i;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		if (!weapon_system.weapon_list[i]._inuse)continue;	//Skip this iteration of the loop
		weapon_think(&weapon_system.weapon_list[i],shootPos,direction,amount);
	}
}

void weapon_update(Weapon* self) {
	WeaponData* wData = self->data;
	float maxFrame;

	maxFrame = wData->fire_time;

	if (self->wf >= maxFrame) {
		self->wf = 0.1;
	}

	self->wf += 0.1;
}

void weapon_update_all() {
	int i;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		if (!weapon_system.weapon_list[i]._inuse)continue;	//Skip this iteration of the loop
		weapon_update(&weapon_system.weapon_list[i]);
	}
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! // Make Sure that Upgrade works
//! 
void upgrade_weapon(Weapon self) {
	WeaponData* w_Data;
	w_Data = self.data;
	int current_lvl = self.level;
	SJson* json, * weapon, * upgrades;
	char buffer[10];
	const char* id_char, * key_char, *stat_char;
	SJString* jString;
	int size, i;
	float *stat = -1;

	json = sj_load("defs/weapon_upgrade_list/weapon_upgrades.def");

	slog("Getting weapon.");
	sprintf(buffer, "%d", w_Data->id);
	id_char = buffer;
	weapon = sj_object_get_value(json, id_char);

	slog("Getting upgrades.");
	sprintf(buffer, "%d", current_lvl);
	key_char = buffer;
	upgrades = sj_object_get_value(weapon, key_char);
	slog("Obtained upgrade");

	
	/*if (!sj_object_get_value_as_float(upgrades, "Damage", stat)) {
		slog("Damage Upgrade!");
	}
	else {
		slog("No damage.");
	}*/
	
	//value_char = buffer;
	

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! TODO: Get it be able to read the values within level arrays in weapon_upgrade.def
	//value = 

	slog("ID: %i", w_Data->id);
	sj_free(json);
}

void upgrade_rand() {
	int list[12];
	int i,m;
	int j = 0;
	for (i = 0; i < weapon_system.weapon_max; i++)
	{
		WeaponData* w_Data;
		if (!weapon_system.weapon_list[i]._inuse)continue;	//Skip this iteration of the loop
		
		w_Data = weapon_system.weapon_list[i].data;

		if (weapon_system.weapon_list[i].level < w_Data->max_level) {
			slog("Weapon '%f' can level up", w_Data->id);
			list[j] = i;
			j++;
		}
	}
	
	slog("Size of list: %i", j);
	
	if (j != 0) {
		int rand_weap = rand() % j;
		
		weapon_system.weapon_list[rand_weap].level += 1;

		upgrade_weapon(weapon_system.weapon_list[rand_weap], weapon_system.weapon_list[rand_weap]);

		slog("Random weapon choosen: %i", rand_weap);
	}
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void remove_weapon(int key) {
	weapon_remove(&weapon_system.weapon_list[key]);
}

void save_weapon(int key) {
	save_custom_weapon_to_json(&weapon_system.weapon_list[key]);
}
