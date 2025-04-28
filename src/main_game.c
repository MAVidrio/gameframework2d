#include "main_game.h"

void main_game_think_and_update(Entity* p) {
    entity_bounds();
    entity_system_collision();
    entity_get_pos(p);

    // Runs if player has no weapons
    if (get_amount_weapons() == 0) get_weapon("0");

    entity_think_all();
    entity_update_all();

    camera_apply_bounds();
}

void main_game_draw(GFC_Vector2D offset, Sprite* sprite, Sprite* player, Bool hitbox_on, float pf) {
    GFC_Color mouseGFC_Color = gfc_color8(255, 100, 255, 200);


    offset = camera_get_offset();
    gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));

    entity_draw_all();

    if (hitbox_on) {
        entity_hitbox();
    }

    //Player
    gf2d_sprite_draw(
        player,
        offset,
        NULL,
        NULL,
        NULL,
        NULL,
        &mouseGFC_Color,
        (int)pf);
}

void debug_game_think(Bool enable_game, Bool enable_collision, Entity* p) {
    if (enable_game) {
        if (enable_collision) {
            entity_bounds();
            entity_system_collision();
        }
        entity_get_pos(p);

        entity_think_all();
    }
}

void debug_game_update() {
    entity_update_all();
    camera_apply_bounds();
}

void debug_game_draw(GFC_Vector2D offset, Sprite* sprite, Sprite* player, Bool hitbox_on, float pf) {
    GFC_Color mouseGFC_Color = gfc_color8(255, 100, 255, 200);


    offset = camera_get_offset();
    gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));

    entity_draw_all();

    if (hitbox_on) {
        entity_hitbox();
    }

    //Player
    gf2d_sprite_draw(
        player,
        offset,
        NULL,
        NULL,
        NULL,
        NULL,
        &mouseGFC_Color,
        (int)pf);
}