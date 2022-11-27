#pragma once
//Screen dimension constants
enum ScreenParams
{
	SCREEN_WIDTH = 1500,
	SCREEN_HEIGHT = 1000
};
//Button constants
enum ButtonParams
{
	BUTTON_WIDTH = 494,
	BUTTON_HEIGHT = 107
};
enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_TOTAL = 4
};
enum ButtonNameInMenu
{
	GAME,
	GALLERY,
	EXIT,
	COUNT_OF_BUTTONS = 3
};
enum ButtonInGallery
{
	BACK_GAL,
	NEXT,
	COUNT_BUT_GAL = 2
};
enum LevelSelectionButtonName
{
	LEVEL1,
	LEVEL2,
	LEVEL3,
	BACK,
	COUNT_OF_LEVELSELECTION_BUTTONS = 4
};
enum PuzzlepieceParams
{
	PUZZLEPIECE_WIDTH = 140,
	PUZZLEPIECE_HEIGHT = 140,
	PUZZLEPIECES_HOR = 5, // puzzle pieces horizontal count
	PUZZLEPIECES_VERT = 5, // puzzle pieces vertical count
	MARGIN = 20
};
enum PuzzlepieceSprites
{
	NOT_SELECTED = 0,
	SELECTED = 1,
	PUZZLEPIECE_SPRITE_TOTAL = 2
};
enum Window
{

	MENU_WIND,
	GAME_WIND,
	GALLERY_WIND,
	EXIT_WIND,
	CLOSE_GAME
};
enum KeyPress
{
	KEY_PRESS_DEFAULT,
	KEY_PRESS_UP,
	KEY_PRESS_DOWN,
	KEY_PRESS_LEFT,
	KEY_PRESS_RIGHT,
	KEY_PRESS_LEFT_CTRL,
	KEY_PRESS_Z
};
enum Pictures
{
	FIRST,
	SECOND,
	THIRD,
	COUNT_OF_PIC = 3
};
enum ExitButtons
{
	YES,
	NO,
	COUNT_OF_BUT = 2
};