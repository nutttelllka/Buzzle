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
		Menu m;
		LevelSelection l(535, 429, 124, 107, 430);
		Gallery g(74, 374, 1214, 252, 138);
		Puzzle pzz1(1);
		Puzzle pzz2(2);
		Puzzle pzz3(3);
		//Load media
		if (//!m.LoadButton() ||
			!l.LoadButton() ||
			!pzz1.loadPuzzle() ||
			!pzz2.loadPuzzle() ||
			!pzz3.loadPuzzle() ||
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
					window_click = g.click_window();
					break;
				case Window::GAME_WIND:
					wind_to_return = GAME_WIND;
					window_click = l.click_window();

					break;
				case Window::LEVEL1_WIND:
					wind_to_return = LEVEL1_WIND;
					pzz1.Game();
					break;
				case Window::LEVEL2_WIND:
					wind_to_return = LEVEL2_WIND;
					pzz2.Game();
					break;
				case Window::LEVEL3_WIND:
					wind_to_return = LEVEL3_WIND;
					pzz3.Game();
					break;
				case Window::EXIT_WIND:
					Exit e;

					//e.SetSelectedWindow(wind_to_return);
					window_click = e.click_window();
					break;
				}

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
