#include "simple_logger.h"

#include "player.h"
#include "weapon.h"
#include "gfc_input.h"
#include "camera_2d.h"
#include "projectile.h"
#include "enemy.h"

void player_think(Entity* self);
void player_update(Entity* self);
int player_draw(Entity* self);
void player_free(Entity* self);
void player_collision(Entity* self, Entity* other);

Entity* player_new(Sprite *sprite) {
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("Failed to spawn player entity.");
		return NULL;
	}

	// initialize this player's weapon system
	weapon_system_init(12);

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
	self->collision = player_collision;

	self->health = 90;
	self->max_health = 100;
	self->recovery = 1;
	self->level = 1;
	self->xp_need = 10;
	self->growth = 1;
	self->speed = 1;
	self->cooldown = 150;

	self->need_pos = 0;
	self->need_bounds = 0;

	add_weapon("0.0");

	//slog("Player succefully spawned.");
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
	self->update = player_update;
	self->draw = player_draw;
	self->free = player_free;

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

	if (self->xp >= self->xp_need) {
		self->level += 1;
		self->xp -= self->xp_need;
		self->xp_need += 10 * self->level;

		// character specific
		self->cooldown -= self->cooldown * 0.05;
		slog("Cooldown: %f", self->cooldown);
	}

	shoot_position.x = self->position.x;
	shoot_position.y = self->position.y;

	self->hitbox.x = self->position.x + 27;
	self->hitbox.y = self->position.y + 27;

	// Call all weapons to fire
	weapon_think_all(shoot_position, self->direction);


	// Fire Weapon 1
	if (gfc_input_command_pressed("Debug_Fire") || fmodf(self->ff, (int)self->cooldown) == 0) {
		projectile = gf2d_sprite_load_all("images/laser.png", 128, 128, 1, 0);

		projectile_new(projectile, gfc_vector2d(shoot_position.x,shoot_position.y), self->direction);
		if (self->level >= 2) {
			projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y - 20), self->direction);
		}
		if (self->level >= 4) {
			projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y + 20), self->direction);
		}
		if (self->level >= 6) {
			self->damage = 4;
			projectile_new(projectile, gfc_vector2d(shoot_position.x - 20, shoot_position.y), self->direction);
		}
	}
	// Fire Weapon 2
	if (gfc_input_command_pressed("Debug_Fire") || fmodf(self->ff, (int)self->cooldown) == 0) {
		projectile = gf2d_sprite_load_all("images/projectile/sword_swing.png", 128, 128, 1, 0);

		//projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y), self->direction);
		weapon_projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y), self->direction, SWORD, self->level);
		/*if (self->level >= 2) {
			projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y - 20), self->direction);
		}
		if (self->level >= 4) {
			projectile_new(projectile, gfc_vector2d(shoot_position.x, shoot_position.y + 20), self->direction);
		}
		if (self->level >= 6) {
			self->damage = 4;
			projectile_new(projectile, gfc_vector2d(shoot_position.x - 20, shoot_position.y), self->direction);
		}*/
	}
	// Debug: Show level, xp required and xp amount
	if (gfc_input_command_pressed("Debug_Show_Level")) {
		slog("Level: %i", self->level);
		slog("Xp Needed: %i", self->xp_need);
		slog("Xp Have: %i", self->xp);
	}

	// Heal
	if (fmodf(self->ff, 100) == 0 && self->health != self->max_health) {
		self->health += self->recovery;
	}

	//Test Spawn next wave
	if (fmodf(self->ff, 240) == 0) {
		enemy_wave(1, self->position);
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
	GFC_Rect healthbarB;
	GFC_Rect healthbar;
	GFC_Rect levelbar;
	offset = camera_get_offset();
	gfc_vector2d_add(position, self->position, offset);

	healthbarB = gfc_rect(position.x + 12, position.y + 100, 100, 10);
	healthbar = gfc_rect(position.x + 12, position.y + 100, self->health, 10);
	levelbar = gfc_rect(position.x - 500, position.y - 300, 100, 100);

	gf2d_sprite_draw(
		self->sprite,
		position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(int)self->pf);

	// Show health bar
	gf2d_draw_rect_filled(healthbarB, GFC_COLOR_BLACK);
	gf2d_draw_rect_filled(healthbar, GFC_COLOR_RED);

	// Show level
	//gfc_rect_draw(levelbar, GFC_COLOR_BLUE);
	
	return;
}

void player_free(Entity* self) {
	if (!self)return;
	weapon_system_close();
	playerData* pData = self->data;
	if (!pData) return;
	free(pData);
	memset(self, 0, sizeof(Entity));
}

void player_collision(Entity* self, Entity* other) {
	Tag otherTag = other->tag;

	switch (otherTag) {
	case ENEMY:
		self->health -= other->damage;
		if (self->health == 0) slog("Dead");
		break;
	case PICKUP:
		if (other->pickup == XP) {
			slog("Pickup XP amount: %i", other->amount);
			self->xp += other->amount * self->growth;
			break;
		}
		else if (other->pickup == CHEST) {
			slog("Pickup Chest amount: %i", other->amount);
			break;
		}
	default:
		break;
	}
}

GFC_Vector2D player_get_position(Entity *self) {
	if (!self)return;
	return self->position;
}