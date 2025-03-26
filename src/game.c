#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_entity.h"
#include "gfc_input.h"
#include "player.h"
#include "enemy.h"
#include "camera_2d.h"
#include "loot.h"
#include "weapon.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    Sprite *player;
    Sprite* chest;
    Entity *p, *pp, *e, *xp, *c;
    const char* characterFile;
    Bool hitbox_on = 0;
    
    int mx,my;
    float mf = 0;
    float pf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(255,100,255,200);
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gfc_input_init("config/input.cfg");
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    //weapon_system_init(12);
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    player = gf2d_sprite_load_all ("images/ch1.png",128,128,3,0);
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    characterFile = "defs/player/character_1.def";
    p = player_new(player);
    e = enemy_new(player);
    xp = xp_new(100, gfc_vector2d(100,100));

    //camera setup
    camera_set_bounds(gfc_rect(0,0, 1300, 820));
    camera_apply_bounds();
    camera_enable_binding(0);

    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        GFC_Vector2D offset;
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        pf+=0.1;
        if (mf >= 16.0)mf = 0;
        if (pf >= 3.0)pf = 0;
        
        entity_bounds();
        entity_system_collision();
        entity_get_pos(p);
        entity_think_all();
        entity_update_all();

        camera_apply_bounds();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first

            offset = camera_get_offset();
            gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));

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
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (gfc_input_command_pressed("Debug_Hitbox")) {
            hitbox_on = !hitbox_on;
            if (hitbox_on) slog("Hitbox on");
            else slog("Hitbox off");
        }

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_system_close();
    weapon_system_close();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
