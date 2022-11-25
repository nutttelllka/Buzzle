//class Menu
//{
//private:
//	Picture menu_texture;
//	LButton menu_button[COUNT_OF_BUTTONS];
//	int event_menu_buttons[COUNT_OF_BUTTONS];
//	int current_over_motion;
//	int count_of_but = COUNT_OF_BUTTONS;
//public:
//	Menu()
//	{
//		for (int i = 0, j = 429; i < COUNT_OF_BUTTONS; i++, j += 124)
//		{
//
//			menu_button[i].setPosition(535, j);
//			menu_button[i].setHeight(105.8);
//			menu_button[i].setWidth(430);
//		}
//	}
//	//LTexture Get_menu_texture()
//	//{
//	//	return menu_texture;
//	//}
//	LButton* Get_menu_button(int index)
//	{
//		return &menu_button[index];
//	}
//	void Set_menu_texture(Picture texture)
//	{
//		menu_texture = texture;
//	}
//	void Set_menu_texture(LButton button, int index)
//	{
//		menu_button[index] = button;
//	}
//	bool Load_menu_button()
//	{
//		bool success = true;
//		if (!menu_button[EXIT].loadButton("img\\Exit.png"))
//		{
//			success = false;
//		}
//		if (!menu_button[GALLERY].loadButton("img\\Gallery.png"))
//		{
//			success = false;
//		}
//		if (!menu_button[GAME].loadButton("img\\Play.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	bool Load_Bg()
//	{
//		bool success = true;
//		if (!menu_texture.loadFromFile("img\\Background.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	void Show_menu()
//	{
//		Load_Bg();//show bg
//		menu_texture.render(0, 0);
//		SDL_RenderPresent(gRenderer);
//	}
//	int SelectedWindowInMenu(int button)
//	{
//		switch (button)
//		{
//		case GAME:
//			return GAME_WIND;
//			break;
//		case GALLERY:
//			return GALLERY_WIND;
//			break;
//		case EXIT:
//			return EXIT_WIND;
//			break;
//		}
//	}
//	int click_menu()
//	{
//		bool quit = false;
//		SDL_Event e;
//		Show_menu();
//		while (!quit)
//		{
//			//Handle events on queue
//			while (SDL_PollEvent(&e) != 0)
//			{
//				if (EscapeIsPressed(e))
//					return Window::EXIT_WIND;
//
//				for (int i = 0; i < count_of_but; i++)
//				{
//					event_menu_buttons[i] = menu_button[i].handleEvent(&e);
//				}
//			}
//			for (int i = 0; i < count_of_but; i++)
//			{
//				if (event_menu_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
//				{
//					for (int i = 0; i < count_of_but; i++)
//					{
//						menu_button[i].render();
//					}
//					SDL_RenderPresent(gRenderer);
//					SDL_Delay(300);
//					event_menu_buttons[i] = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
//					return SelectedWindowInMenu(i);
//				}
//				else
//				{
//					SDL_RenderClear(gRenderer);
//					menu_texture.renderP();
//
//					for (int i = 0; i < count_of_but; i++)
//					{
//						menu_button[i].render();
//					}
//				}
//			}
//			//Update screen
//			SDL_RenderPresent(gRenderer);
//		}
//	}
//};
//class LevelSelection
//{
//private:
//	Picture levelselection_bgtexture;
//	int count_of_but = COUNT_OF_LEVELSELECTION_BUTTONS;
//	LButton levelselection_button[COUNT_OF_LEVELSELECTION_BUTTONS];
//    int event_levelselection_buttons[COUNT_OF_LEVELSELECTION_BUTTONS];
//public:
//	LevelSelection()
//	{
//		for (int i = 0, j = 429; i < count_of_but; i++, j += 124)
//		{
//
//			levelselection_button[i].setPosition(535, j);
//			levelselection_button[i].setHeight(105.8);
//			levelselection_button[i].setWidth(430);
//		}
//	}
//	/*Picture Get_levelselection_bgtexture()
//	{
//		return levelselection_bgtexture;
//	}*/
//	LButton* Get_levelselection_button(int index)
//	{
//		return &levelselection_button[index];
//	}
//	void Set_levelselection_bgtexture(Picture texture)
//	{
//		levelselection_bgtexture = texture;
//	}
//	void Set_levelselection_button(LButton button, int index)
//	{
//		levelselection_button[index] = button;
//	}
//	bool Load_levelselection_button()
//	{
//		bool success = true;
//		if (!levelselection_button[LEVEL1].loadButton("img\\Level\\Level1.png"))
//		{
//			success = false;
//		}
//		if (!levelselection_button[LEVEL2].loadButton("img\\Level\\Level2.png"))
//		{
//			success = false;
//		}
//		if (!levelselection_button[LEVEL3].loadButton("img\\Level\\Level3.png"))
//		{
//			success = false;
//		}
//		if (!levelselection_button[BACK].loadButton("img\\Level\\Back.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	bool Load_levelselection_bg()
//	{
//		bool success = true;
//		if (!levelselection_bgtexture.loadFromFile("img\\Background_Gallery.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	int Show_levelselection()
//	{
//		bool back = false;
//		SDL_Event e;
//
//		//show bg
//		Load_levelselection_bg();
//		levelselection_bgtexture.renderP();
//		SDL_RenderPresent(gRenderer);
//
//		while (!back)
//		{
//			//Handle events on queue
//			while (SDL_PollEvent(&e) != 0)
//			{
//				//User requests quit
//				if (EscapeIsPressed(e))
//					return Window::MENU_WIND;
//
//				for (int i = 0; i < count_of_but; i++)
//				{
//					event_levelselection_buttons[i] = levelselection_button[i].handleEvent(&e);
//				}
//				for (int i = 0; i < count_of_but; i++)
//				{
//					if (event_levelselection_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
//					{
//						for (int i = 0; i < count_of_but; i++)
//						{
//							levelselection_button[i].render();
//						}
//						SDL_RenderPresent(gRenderer);
//						SDL_Delay(300);
//						event_levelselection_buttons[i] = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
//						//return SelectedWindowInMenu(i);
//					}
//					else
//					{
//						SDL_RenderClear(gRenderer);
//						levelselection_bgtexture.renderP();
//
//						for (int i = 0; i < count_of_but; i++)
//						{
//							levelselection_button[i].render();
//						}
//					}
//				}
//			}
//			//Update screen
//			SDL_RenderPresent(gRenderer);
//		}
//		return 0;
//	}
//};
//class Gallery
//{
//private:
//	Picture texture_bg;
//	LButton buttons[COUNT_BUT_GAL];
//	int event_gal_buttons[COUNT_BUT_GAL];
//	int current_over_motion;
//	Picture pictures[COUNT_OF_PIC];
//	int selected_pic = FIRST;
//public:
//	Gallery()
//	{
//		for (int i = 0, j = 74; i < COUNT_BUT_GAL; i++, j += 1214)
//		{
//
//			buttons[i].setPosition(j, 374);
//			buttons[i].setHeight(252.5);
//			buttons[i].setWidth(138);
//		}
//		for (int i = 0; i < COUNT_OF_PIC; i++)
//		{
//			pictures[i].setPosition(281, 45);
//		}
//	}
//	Picture Get_menu_texture()
//	{
//		return texture_bg;
//	}
//	LButton* Get_menu_button(int index)
//	{
//		return &buttons[index];
//	}
//	void Set_menu_texture(Picture texture)
//	{
//		texture_bg = texture;
//	}
//	void Set_menu_texture(LButton button, int index)
//	{
//		buttons[index] = button;
//	}
//	bool Load_button()
//	{
//		bool success = true;
//
//		return success;
//	}
//	bool Load_Buttons()
//	{
//		bool success = true;
//		if (!buttons[NEXT].loadButton("img\\Buttons Gallery\\next.png"))
//		{
//			success = false;
//		}
//		if (!buttons[ButtonInGallery::BACK_GAL].loadButton("img\\Buttons Gallery\\back.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	bool Load_Bg()
//	{
//		bool success = true;
//		if (!texture_bg.loadFromFile("img\\Background_Gallery.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	bool Load_Pics()
//	{
//		bool success = true;
//		if (!pictures[FIRST].loadFromFile("img\\Gallery\\1.png"))
//		{
//			success = false;
//		}
//		if (!pictures[SECOND].loadFromFile("img\\Gallery\\2.png"))
//		{
//			success = false;
//		}
//		if (!pictures[THIRD].loadFromFile("img\\Gallery\\2.png"))
//		{
//			success = false;
//		}
//		return success;
//	}
//	void Show_gallery()
//	{
//		Load_Bg();
//		Load_Buttons();
//		if (!Load_Pics())
//		{
//			printf("Failed to load media!\n");
//		}
//		texture_bg.renderP();
//		pictures[FIRST].renderP();
//		SDL_RenderPresent(gRenderer);
//	}
//	int SelectedGalleryWindow(int button)
//	{
//		switch (button)
//		{
//		case EXIT_WIND:
//			return MENU_WIND;
//			break;
//		case GALLERY:
//			return GALLERY_WIND;
//			break;
//		case EXIT:
//			return EXIT_WIND;
//			break;
//		}
//	}
//	int Click_gallery()
//	{
//		int quit = false;
//		SDL_Event e;
//		Show_gallery();
//		while (!quit)
//		{
//			while (SDL_PollEvent(&e) != 0)
//			{
//				if (EscapeIsPressed(e))
//					return Window::MENU_WIND;
//
//				for (int i = 0; i < COUNT_BUT_GAL; i++)
//				{
//					event_gal_buttons[i] = buttons[i].handleEvent(&e);
//				}
//			}
//			for (int i = 0; i < COUNT_BUT_GAL; i++)
//			{
//				if (event_gal_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
//				{
//					for (int i = 0; i < COUNT_BUT_GAL; i++)
//					{
//						buttons[i].render();
//					}
//					SDL_RenderPresent(gRenderer);
//					SDL_Delay(300);
//					event_gal_buttons[i] = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
//						switch (i)
//						{
//						case ButtonInGallery::BACK_GAL:
//							switch (selected_pic)
//							{
//							case FIRST:
//								selected_pic = FIRST;
//								return Window::MENU_WIND;
//								break;
//							case SECOND:
//							case THIRD:
//								selected_pic--;
//								break;
//							}
//							break;
//						case NEXT:
//							switch (selected_pic)
//							{
//							case FIRST:
//							case SECOND:
//								selected_pic++;
//								break;
//							case THIRD:
//								selected_pic = FIRST;
//								return Window::MENU_WIND;
//								break;
//							}
//							break;
//						}
//						SDL_RenderClear(gRenderer);
//						texture_bg.renderP();
//						pictures[selected_pic].renderP();
//						for (int i = 0; i < COUNT_BUT_GAL; i++)
//						{
//							buttons[i].render();
//						}
//				}
//				else
//				{
//					SDL_RenderClear(gRenderer);
//					texture_bg.render(0, 0);
//					pictures[selected_pic].renderP();
//					for (int i = 0; i < COUNT_BUT_GAL; i++)
//					{
//						buttons[i].render();
//					}
//				}
//			}
//			SDL_RenderPresent(gRenderer);
//		}
//	}
//};
