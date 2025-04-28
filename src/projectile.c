#include "simple_logger.h"

#include "projectile.h"
#include "player.h"
#include "camera_2d.h"

// Laser Weapon
void projectile_think(Entity* self);
void projectile_update(Entity* self);
int projectile_draw(Entity* self);
void projectile_free(Entity* self);
void projectile_cam_coll(Entity* self);
void projectile_coll(Entity* self, Entity* other);

// Weapon 2
void weapon_2_update(Entity* self);
int weapon_2_draw(Entity* self);
void weapon2_projectile_coll(Entity* self, Entity* other);

Entity* weapon_projectile_new(Sprite* sprite, GFC_Vector2D position, GFC_Vector2D direction,
	WeaponType weapon, int damage, int pierce, int level) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn projectile entity.");
		return NULL;
	}

	// Define
	self->tag = PROJECTILE;

	self->position = position;
	self->sprite = sprite;
	self->pf = 0;
	self->ff = 0;
	self->direction = direction;
	self->hitbox = gfc_rect(self->position.x + 27, self->position.y + 27, 100, 100);

	// Set projectiles to correct weapon type
	switch (weapon) {
	case LASER:
		self->think = projectile_think;
		self->update = projectile_update;
		self->draw = projectile_draw;
		self->free = projectile_free;
		self->cam_coll = projectile_cam_coll;
		self->collision = projectile_coll;
		// Camera bounds
		self->need_bounds = 1.0;
		break;
	case SWORD:
		self->update = weapon_2_update;
		self->draw = weapon_2_draw;
		self->free = projectile_free;
		self->collision = weapon2_projectile_coll;
		// Camera bounds
		self->need_bounds = 0.0;
		break;
	case AREA:
	default:
		self->think = projectile_think;
		self->update = projectile_update;
		self->draw = projectile_draw;
		self->free = projectile_free;
		self->cam_coll = projectile_cam_coll;
		self->collision = projectile_coll;
		// Camera bounds
		self->need_bounds = 0.0;
		break;
	}
	
	self->level = level;

	// Set damage and pierce values depending on levels
	switch (self->level) {
	case 10:
		self->damage = damage + 5;
		self->pierce = pierce + 3;
		break;
	case 9:
	case 8:
	case 7:
		self->damage = damage + 3;
		self->pierce = pierce + 2;
		break;
	case 6:
	case 5:
	case 4:
		self->damage = damage + 2;
		self->pierce = pierce + 1;
		break;
	case 3:
	case 2:
	case 1:
		self->damage = damage + 1;
		self->pierce = pierce;
		break;
	default:
		self->damage = damage;
		self->pierce = pierce;
		break;
	}

	self->need_pos = 1;

	//slog("Projectile succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

Entity* projectile_new(Sprite* sprite, GFC_Vector2D position, GFC_Vector2D direction) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn projectile entity.");
		return NULL;
	}

	// Define
	self->tag = PROJECTILE;

	self->position = position;
	self->sprite = sprite;
	self->pf = 0;
	self->direction = direction;
	self->hitbox = gfc_rect(self->position.x, self->position.y, 10, 10);

	self->think = projectile_think;
	self->update = projectile_update;
	self->draw = projectile_draw;
	self->free = projectile_free;
	self->cam_coll = projectile_cam_coll;
	self->collision = projectile_coll;
	
	self->damage = 1;
	self->pierce = 1;

	// Camera bounds
	self->need_bounds = 1.0;

	//slog("Projectile succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

Entity* projectile_new_from_file(const char* filename)
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

	projectile_config_from_file(self, filename);

	self->think = projectile_think;
	self->update = projectile_update;
	self->draw = projectile_draw;
	self->free = projectile_free;

	slog("Projectile succefully spawned.");
	//slog("%f/%f/%f", self->collisionX.s.b.xC, self->collisionX.s.b.yC, self->collisionX.s.b.zC);
	return self;
}

void projectile_config_from_file(Entity* self, const char* filename)
{
	SJson* json;
	json = sj_load(filename);

	//projectile_config(self,json);

	sj_object_free(json);
}

void projectile_config(Entity* self, SJson* json)
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
/*-------------------- ( WEAPON 1 ) -----------------------*/
void projectile_update(Entity* self) {
	if (!self) return;

	if (self->pierce <= 0) {
		projectile_free(self);
	}
	
	GFC_Vector2D entPos = self->position;
	self->hitbox = gfc_rect(self->position.x, self->position.y+50, 100, 15);
}

void projectile_think(Entity* self) {
	if (!self)return;

	GFC_Vector2D movement = gfc_vector2d(0, 0);
	switch (self->level) {
	case (1):
		movement.x = self->direction.x * 2;
		movement.y = self->direction.y * 2;
		break;
	case 2:
		movement.x = (self->direction.x * 2) / 0.8;
		movement.y = (self->direction.y * 2) / 0.8;
		break;
	case 4:
		movement.x = (self->direction.x * 2) / 0.6;
		movement.y = (self->direction.y * 2) / 0.6;
		break;
	case 6:
		movement.x = (self->direction.x * 2) / 0.4;
		movement.y = (self->direction.y * 2) / 0.4;
		break;
	default:
		movement.x = (self->direction.x * 2) / 0.2;
		movement.y = (self->direction.y * 2) / 0.2;
		break;
	}

	gfc_vector2d_add(self->position, self->position, movement);
}

int projectile_draw(Entity* self)
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
		0);
	return;
}

void projectile_free(Entity* self) {
	if (!self) return;

	//projectileData* pData = self->data;
	//if (!pData) return;
	//free(pData);
	memset(self, 0, sizeof(Entity));
}

void projectile_cam_coll(Entity* self) {
	//slog("Doing collision actions");
	//slog("Deleting at %f/%f.", self->position.x, self->position.y);
	//slog("Camera bounds: %f/%f", self->camera_bounds.x, self->camera_bounds.y);
	projectile_free(self);
}

void projectile_coll(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	switch (otherTag) {
	case ENEMY:
		self->pierce -= 1;
		if (self->pierce == 0) {
			projectile_free(self);
		}
		break;
	default:
		break;
	}
}

/*-------------------- ( WEAPON 2 ) -----------------------*/

void weapon_2_update(Entity* self) {
	if (!self) return;
	self->position = self->player_pos;
	if (self->ff >= 30) {
		projectile_free(self);
	}

	self->position.x += 50 * self->direction.x;
	self->position.y += 10;
	self->hitbox = gfc_rect(self->position.x, self->position.y, 100, 100);
	self->ff += 1;
}

int weapon_2_draw(Entity* self)
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
		0);
	return;
}

void weapon2_projectile_coll(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	switch (otherTag) {
	case ENEMY:
		
		break;
	default:
		break;
	}
}