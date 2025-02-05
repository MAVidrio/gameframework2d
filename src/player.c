#include "simple_logger.h"

#include "player.h"
#include "gfc_input.h"

void player_think(Entity* self);
//void player_update(Entity* self);
//int player_draw(Entity* self);
//void player_free(Entity* self);

Entity* player_new(const char* filename)
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
	//self->update = player_update;
	//self->draw = player_draw;
	//self->free = player_free;

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

void player_think(Entity* self) {
	if (!self)return;

	//const Uint8* keys;

	//keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

	//if (keys[SDL_SCANCODE_W]) { slog("Up"); }	// Press W
	//else if (keys[SDL_SCANCODE_S]) { slog("Down"); }	// Press S
	//if (keys[SDL_SCANCODE_D]) { slog("Right"); }	// Press D
	//else if (keys[SDL_SCANCODE_A]) { slog("Left"); }	// Press A

	if (gfc_input_command_down("left")) {
		slog("Left");
	}
	if (gfc_input_command_down("right")) {
		slog("Right");
	}
	if (gfc_input_command_down("up")) {
		slog("Up");
	}
	if (gfc_input_command_down("down")) {
		slog("Down");
	}
}