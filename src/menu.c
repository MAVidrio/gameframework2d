#include "menu.h"

Uint32 main_menu_run(GFC_Vector2D mousePos, Uint32 mousePress) {
	Sprite* button1 = gf2d_sprite_load_image("images/Menu/Play_Button.png");
	Sprite* button2 = gf2d_sprite_load_image("images/Menu/Editor_Button.png");
	Sprite* button3 = gf2d_sprite_load_image("images/Menu/Quit_Button.png");

	GFC_Rect buttonRect1 = gfc_rect(450, 250, 300, 70);
	GFC_Rect buttonRect2 = gfc_rect(450, 325, 300, 70);
	GFC_Rect buttonRect3 = gfc_rect(450, 400, 300, 70);

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//! TODO: 
	//!		Create 2d hitbox for buttons and make new file for the game to run on.
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Play Button
	gf2d_sprite_draw(
		button1,
		gfc_vector2d(450, 250),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//gf2d_draw_rect_filled(buttonRect1, GFC_COLOR_BLACK);

	// Editor button
	gf2d_sprite_draw(
		button2,
		gfc_vector2d(450, 325),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//gf2d_draw_rect_filled(buttonRect2, GFC_COLOR_BLACK);

	// Quit Button
	gf2d_sprite_draw(
		button3,
		gfc_vector2d(450, 400),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//gf2d_draw_rect_filled(buttonRect3, GFC_COLOR_BLACK);

	// If Play button is pressed, play game
	if (gfc_point_in_rect(mousePos, buttonRect1) && mousePress == 1) {
		slog("Play game");
		return 1;
	}
	else if (gfc_point_in_rect(mousePos, buttonRect2) && mousePress == 1) {
		slog("Play debugged game");
		return 2;
	}
	else if (gfc_point_in_rect(mousePos, buttonRect3) && mousePress == 1) {
		slog("Quit game");
		return 3;
	}

	return 0;

}

Uint32 weapon_menu_run(GFC_Vector2D mousePos, Uint32 mousePress, int* weapon_dis, int* previous_Clicked) {
	Sprite* main_menu_button = gf2d_sprite_load_image("images/Menu/Main_Menu_Button.png");
	Sprite* save_button = gf2d_sprite_load_image("images/Menu/Save_Button.png");
	Sprite* background = gf2d_sprite_load_image("images/Menu/Weapon_Background.png");
	Sprite* right_arrow = gf2d_sprite_load_image("images/Menu/Right_Arrow.png");
	Sprite* left_arrow = gf2d_sprite_load_image("images/Menu/Left_Arrow.png");
	Sprite* weapon_display;

	switch (*weapon_dis) {
		case 1:
			weapon_display = gf2d_sprite_load_image("images/Menu/Weapon_Display_Laser_Gun.png");
			//slog(weapon_dis);
			break;
		default:
			weapon_display = gf2d_sprite_load_image("images/Menu/Weapon_Display_Laser_Sword.png");
			//slog(weapon_dis);
			break;
	}

	GFC_Rect main_menu_Rect = gfc_rect(20, 630, 300, 70);
	GFC_Rect save_button_Rect = gfc_rect(20, 550, 300, 70);
	GFC_Rect right_arrow_Rect = gfc_rect(220, 60, 50, 50);
	GFC_Rect left_arrow_Rect = gfc_rect(60, 60, 50, 50);

	// Background
	gf2d_sprite_draw(
		background,
		gfc_vector2d(0, 0),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	// Weapon Sprite
	gf2d_sprite_draw(
		weapon_display,
		gfc_vector2d(130,50),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	// Right Arrow
	gf2d_sprite_draw(
		right_arrow,
		gfc_vector2d(220, 60),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//gf2d_draw_rect_filled(right_arrow_Rect, GFC_COLOR_BLACK);

	// Left Arrow
	gf2d_sprite_draw(
		left_arrow,
		gfc_vector2d(60, 60),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	//gf2d_draw_rect_filled(left_arrow_Rect, GFC_COLOR_BLACK);


	gf2d_sprite_draw(
		save_button,
		gfc_vector2d(20, 550),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	gf2d_sprite_draw(
		main_menu_button,
		gfc_vector2d(20, 630),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	// If Left button is pressed, change weapon value to left
	if (gfc_point_in_rect(mousePos, left_arrow_Rect) && mousePress == 1 && *previous_Clicked != 1) {
		if (*weapon_dis == 0) {
			*weapon_dis = 1;
		}
		else {
			*weapon_dis -= 1;
		}
		slog("Left button");
		*previous_Clicked = 1;
		return 1;
	}
	// If Right button is pressed, change weapon value to right
	else if (gfc_point_in_rect(mousePos, right_arrow_Rect) && mousePress == 1 && *previous_Clicked != 1) {
		if (*weapon_dis == 1) {
			*weapon_dis = 0;
		}
		else {
			*weapon_dis += 1;
		}
		slog("Right button");
		*previous_Clicked = 1;
		return 2;
	}
	// If Save button is pressed, add the weapon to the json def file
	else if (gfc_point_in_rect(mousePos, save_button_Rect) && mousePress == 1 && *previous_Clicked != 1) {
		slog("Save weapon");
		*previous_Clicked = 1;
		return 3;
	}
	// If Main Menu button is pressed, call the game to free everything and return to main menu.
	else if (gfc_point_in_rect(mousePos, main_menu_Rect) && mousePress == 1 && *previous_Clicked != 1) {
		slog("Return to main menu.");
		*previous_Clicked = 1;
		return 4;
	}
	// This makes sure that the button is only pressed once.
	else if (*previous_Clicked == 1 && mousePress == 0) {
		*previous_Clicked = 0;
	}

}