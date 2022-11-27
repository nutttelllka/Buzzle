#include "classes.h"
//using namespace std;

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Menu m(535, 429, 125, 107, 430);
		LevelSelection l(535, 429, 124, 107, 430);
		Gallery g(74, 374, 1214, 252, 138);
		Puzzle pzz;
		Exit e(535, 429, 125, 107, 430);
		//Load media
		if (!m.LoadButton() ||
			!l.LoadButton() ||
			//!pzz.loadPuzzle("img/Puzzle1.png") || 
			!loadMedia() || !g.Load_Pics())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Launch intro
			//CreateThread(0, 0, Intro, 0, 0, 0);
			//SDL_Delay(6000);

			int window_click = Window::MENU_WIND;
			int wind_to_return = Window::MENU_WIND;
			while (window_click != Window::CLOSE_GAME)
			{
				switch (window_click)
				{
				case Window::MENU_WIND:
					wind_to_return = MENU_WIND;
					window_click = m.click_window();
					break;
				case Window::GALLERY_WIND:
					//g.Show_gallery();
					wind_to_return = GALLERY_WIND;
					//g.ShowPics();
					window_click = g.click_window();
					break;
				case Window::GAME_WIND:
					wind_to_return = GAME_WIND;
					window_click = l.click_window();
					break;
				case Window::EXIT_WIND:
					e.SetSelectedWindow(wind_to_return);
					window_click = e.click_window();
				}

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
