#include "classes.h"
//using namespace std;
int level_game(int window_click)
{
	if (window_click == LEVEL1_WIND)
	{
		Puzzle pzz1(1);
		//wind_to_return = LEVEL1_WIND;
		window_click = pzz1.Game();
	}
	else if (window_click == Window::LEVEL2_WIND)
	{
		Puzzle pzz2(2);
		//wind_to_return = LEVEL2_WIND;
		window_click = pzz2.Game();
	}
	else if (window_click == Window::LEVEL3_WIND)
	{
		Puzzle pzz3(3);
		//wind_to_return = LEVEL3_WIND;
		window_click = pzz3.Game();
	}
	return window_click;
}
int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		
		
		/*Menu m;
		Gallery g(74, 374, 1214, 252, 138);
		LevelSelection l(535, 429, 124, 107, 430);*/
		/*Puzzle pzz1(1);
		Puzzle pzz2(2);
		Puzzle pzz3(3);*/
		//Load media
		if (//!m.LoadButton() ||
			//!l.LoadButton() ||
			//!pzz1.loadPuzzle() ||
			//!pzz2.loadPuzzle() ||
			//!pzz3.loadPuzzle() ||
			!loadMedia() )
		{
			printf("Failed to load media!\n");
		}
		else
		{
			// Launch intro
			CreateThread(0, 0, Intro, 0, 0, 0);
			SDL_Delay(3000);


			int window_click = Window::MENU_WIND;
			//int wind_to_return = Window::MENU_WIND;
			while (window_click != Window::CLOSE_GAME)
			{
				if (window_click == Window::MENU_WIND)
				{
					//wind_to_return = MENU_WIND;
					Menu m;
					window_click = m.click_window();
				}
				else if (window_click == Window::GALLERY_WIND)
				{
					//wind_to_return  = MENU_WIND;
					Gallery g(74, 374, 1214, 252, 138);
					window_click = g.click_window();
				}

				else if (window_click == Window::GAME_WIND)
				{
					//wind_to_return = GAME_WIND;
					LevelSelection l(535, 429, 124, 107, 430);
					window_click = l.click_window();
				}
				else if (window_click == LEVEL1_WIND || window_click == Window::LEVEL2_WIND || window_click == Window::LEVEL3_WIND)
				{
					window_click = level_game(window_click);
				}
				//else if (window_click == LEVEL1_WIND)
				//{
				//	Puzzle pzz1(1);
				//	//wind_to_return = LEVEL1_WIND;
				//	window_click = pzz1.Game();
				//}
				//else if (window_click == Window::LEVEL2_WIND)
				//{
				//	Puzzle pzz2(2);
				//	//wind_to_return = LEVEL2_WIND;
				//	window_click = pzz2.Game();
				//}
				//else if (window_click == Window::LEVEL3_WIND)
				//{
				//	Puzzle pzz3(3);
				//	//wind_to_return = LEVEL3_WIND;
				//	window_click = pzz3.Game();
				//}
				else if (window_click == Window::EXIT_WIND)
				{
					Exit e;
					window_click = e.click_window();
				}
				//switch (window_click)
				//{
				//case Window::MENU_WIND:

				//	//wind_to_return = MENU_WIND;
				//	window_click = m.click_window();
				//	break;
				//case Window::GALLERY_WIND:
				//	//g.Show_gallery();
				//	//wind_to_return = GALLERY_WIND;
				//	window_click = g.click_window();
				//	break;
				//case Window::GAME_WIND:
				//	//wind_to_return = GAME_WIND;
				//	window_click = l.click_window();

				//	break;
				//case Window::LEVEL1_WIND:
				//	//wind_to_return = LEVEL1_WIND;
				//	window_click = pzz1.Game();
				//	break;
				//case Window::LEVEL2_WIND:
				//	//wind_to_return = LEVEL2_WIND;
				//	window_click = pzz2.Game();
				//	break;
				//case Window::LEVEL3_WIND:
				//	//wind_to_return = LEVEL3_WIND;
				//	window_click = pzz3.Game();
				//	break;
				//case Window::EXIT_WIND:
				//	Exit e;

				//	//e.SetSelectedWindow(wind_to_return);
				//	window_click = e.click_window();
				//	break;
				//}

			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
