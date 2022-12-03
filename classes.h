#pragma once
#include "functions.h"

/// <summary>
/// Texture wrapper class
/// </summary>
/// 
class LTexture;
class Picture;
class LButton;
class CurrentWindow;
class Menu;
class LevelSelection;
class Gallery;
class PuzzlePiece;
class Exit;

bool is_exit(int window, SDL_Event ev);

class LTexture
{
private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;

public:
	//Initializes variables
	LTexture()
	{
		//Initialize
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
	//Deallocates memory
	~LTexture()
	{
		//Deallocate
		free();
	}
	//Loads image at specified path
	bool loadFromFile(string path)
	{
		//Get rid of preexisting texture
		free();

		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else
		{
			//Color key image
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

			//Create texture from surface pixels
			newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
			if (newTexture == NULL)
			{
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}
			else
			{
				//Get image dimensions
				mWidth = loadedSurface->w;
				mHeight = loadedSurface->h;
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);
		}

		//Return success
		mTexture = newTexture;
		return mTexture != NULL;
	}
	//Deallocates texture
	void free()
	{
		//Free texture if it exists
		if (mTexture != NULL)
		{
			SDL_DestroyTexture(mTexture);
			mTexture = NULL;
			mWidth = 0;
			mHeight = 0;
		}
	}

	//Renders texture at given point
	void render(int x = 0, int y = 0, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		//Set rendering space and render to screen
		SDL_Rect renderQuad = { x, y, mWidth, mHeight };

		//Set clip rendering dimensions
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		//Render to screen
		SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
	}

	//Gets image dimensions
	int getWidth()
	{
		return mWidth;
	}
	int getHeight()
	{
		return mHeight;
	}

};
LTexture IntroTextures[2];
DWORD WINAPI Intro(void* param)
{
	//Clear screen
	SDL_RenderClear(gRenderer);

	//Render texture to screen
	IntroTextures[0].render(0, 0);
	//Update screen
	SDL_RenderPresent(gRenderer);

	SDL_Delay(3000);

	//Clear screen
	SDL_RenderClear(gRenderer);
	//Render texture to screen
	IntroTextures[1].render(0, 0);
	//Update screen
	SDL_RenderPresent(gRenderer);
	return 0;
}
class Picture : public LTexture
{
private:
	int x;
	int y;
public:
	Picture() :Picture(0, 0)
	{
	}
	Picture(int x, int y)
	{
		setPosition(x, y);
	}
	void setX(int x)
	{
		this->x = x;
	}
	void setY(int y)
	{
		this->y = y;
	}
	void setPosition(int x, int y)
	{
		setX(x);
		setY(y);
	}
	void renderP()
	{
		render(x, y);
	}
};
/// <summary>
/// level selection window
/// </summary>

class LButton
{
private:
	//Top left position
	SDL_Point mPosition;
	//Currently used sprite
	LButtonSprite mCurrentSprite;
	SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
	LTexture gButtonSpriteSheetTexture;
	int height;
	int width;
public:
	//Initializes internal variables
	LButton() :LButton(BUTTON_WIDTH, BUTTON_HEIGHT, 0, 0)
	{
	}
	LButton(int width, int height, int x, int y)
	{
		setHeight(height);
		setWidth(width);
		setPosition(x, y);
		mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	}

	/// <summary>
	/// Sets top left position
	/// </summary>
	/// <param name="x">X coordinate</param>
	/// <param name="y">Y coordinate</param>

	void SetMCurrentSprite(LButtonSprite sprite)
	{
		if (sprite >= LButtonSprite::BUTTON_SPRITE_MOUSE_OUT && sprite <= LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
		{
			mCurrentSprite = sprite;
		}
	}
	void setPosition(int x, int y)
	{
		mPosition.x = x;
		mPosition.y = y;
	}
	void setHeight(int height)
	{
		this->height = height;
	}
	void setWidth(int width)
	{
		this->width = width;
	}
	/// <summary>
	/// Handles mouse event
	/// </summary>
	/// <param name="e">mouse event</param>
	/// <returns>returns button sprite number</returns>
	int handleEvent(SDL_Event* e)
	{
		int button_sprite = BUTTON_SPRITE_MOUSE_OUT;
		//If mouse event happened
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_KEYDOWN)
		{
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (x < mPosition.x
				|| x > mPosition.x + width
				|| y < mPosition.y
				|| y > mPosition.y + height)
				inside = false;

			//Mouse is outside button
			if (!inside)
			{
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				button_sprite = BUTTON_SPRITE_MOUSE_OUT;
			}
			//Mouse is inside button
			else
			{
				//Set mouse over sprite
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					button_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					button_sprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					button_sprite = BUTTON_SPRITE_MOUSE_UP;
					break;
				}
			}

		}
		return button_sprite;
	}

	//Shows button sprite
	void render()
	{
		//Show current button sprite
		gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
	}
	void free()
	{
		gButtonSpriteSheetTexture.free();
	}
	void Set_gSpriteClips(SDL_Rect sprite, int index)
	{
		gSpriteClips[index] = sprite;
	}
	SDL_Rect* Get_gSpriteClips(int index)
	{
		return &gSpriteClips[index];
	}
	bool loadButton(string link)
	{
		//Loading success flag
		bool success = true;

		//Load sprites
		if (!gButtonSpriteSheetTexture.loadFromFile(link))
		{
			printf("Failed to load button sprite texture!\n");
			success = false;
		}
		else
		{
			//Set sprites
			for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
			{
				gSpriteClips[i].x = 0;
				gSpriteClips[i].y = i * height;
				gSpriteClips[i].w = width;
				gSpriteClips[i].h = height;
			}
		}

		return success;
	}


};
class CurrentWindow abstract
{
protected:
	Picture texture_bg;
	int count_of_but;
	vector <LButton> button;
	 int window;
public:
	
	CurrentWindow(int x_but, int y_but, int margin, int height_but, int width_but, int count_of_but,int window, bool vert_but = true)
	{
		this->window = window;
		this->count_of_but = count_of_but;
		for (int i = 0; i < count_of_but; i++)
		{
			LButton butt;
			button.push_back(butt);
		}
		for (int i = 0, y = y_but; i < count_of_but; i++, y += margin)
		{
			if (vert_but)
			{
				button[i].setPosition(x_but, y);
			}
			else
			{
				button[i].setPosition(x_but, y_but);
				x_but += margin ;
			}
			button[i].setHeight(height_but);
			button[i].setWidth(width_but);
		}
	}
	//void SetPositionForGorizBut(int x_but, int y_but, int margin)
	//{
	//	for (int i = 0, j = x_but; i < count_of_but; i++, j += margin)
	//	{
	//		button[i].setPosition(j, y_but);
	//	}
	//}

	Picture GetTextureBG()
	{
		return texture_bg;
	}
	LButton* GetButton(int index)
	{
		return &button[index];
	}
	virtual bool LoadButton() = 0;
	virtual bool LoadBg() = 0;
	void Show_window()
	{
		static int count = 0;
		if (!LoadBg()||!LoadButton())
		{
			printf("Failed to initialize!\n");
		}
		
		texture_bg.renderP();
		for (int i = 0; i < count_of_but; i++)
		{
			button[i].render();
		}
		SDL_RenderPresent(gRenderer);
		count++;
	}
	virtual int SelectedButtonInCurrentWindow(int button) = 0;
	int click_window()
	{
		vector <int> event_buttons;

		for (int i = 0; i < count_of_but; i++)
		{
			event_buttons.push_back(0);
		}
		bool quit = false;
		SDL_Event e;
		Show_window();
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				if (x_or_esc_is_pressed(e))
				{
					if (is_exit(window, e))
						return CLOSE_GAME;
				}
					//return SelectedButtonInCurrentWindow(EXIT_WIND);
				Show_window();
				for (int i = 0; i < count_of_but; i++)
				{
					event_buttons[i] = button[i].handleEvent(&e);
				}
			}
			for (int i = 0; i < count_of_but; i++)
			{
				if (event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
				{
					for (int i = 0; i < count_of_but; i++)
					{
						button[i].render();
					}
					SDL_RenderPresent(gRenderer);
					SDL_Delay(200);
					//SDL_RenderClear(gRenderer);

					//texture_bg.free();
					//for (int i = 0; i < count_of_but; i++)
					//{
					//	button[i].free();
					//}
					button[i].SetMCurrentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
					return SelectedButtonInCurrentWindow(i);
				}
				else if (event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION || event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OUT)
				{
					//SDL_RenderClear(gRenderer);
					texture_bg.renderP();

					for (int i = 0; i < count_of_but; i++)
					{
						button[i].render();
					}
				}
			}
			//Update screen
			SDL_RenderPresent(gRenderer);
		}
	}
};
class Exit : public CurrentWindow
{
private:
	int selected_window;
	static int count_of_wind;
public:
	Exit() :CurrentWindow(306, 670, 460, 107, 430, ExitButtons::COUNT_OF_BUT,Window::EXIT_WIND, false)
	{
		count_of_wind++;
	}
	~Exit()
	{
		count_of_wind--;
	}
	int GetCountOfWind()
	{
		return count_of_wind;
	}
	int GetSelectedWindow()
	{
		return  selected_window;
	}
	void SetSelectedWindow(int selected_window)
	{
		this->selected_window = selected_window;
	}
	bool LoadButton()override
	{
		bool success = true;
		if (!button[YES].loadButton("img\\Exit\\yes.png"))
		{
			success = false;
		}
		if (!button[NO].loadButton("img\\Exit\\no.png"))
		{
			success = false;
		}
		return success;
	}
	bool LoadBg()override
	{
		bool success = true;
		if (!texture_bg.loadFromFile("img\\Exit\\bg.png"))
		{
			success = false;
		}
		return success;
	}
	int SelectedButtonInCurrentWindow(int button)override
	{
		switch (button)
		{
		case YES:
			return CLOSE_GAME;
			break;
		case NO:
			return selected_window;
			break;
		case EXIT_WIND:
			return CLOSE_GAME;
		}
		//switch (button)
		//{
		//case YES:
		//	return CLOSE_GAME;
		//	break;
		//case NO:
		//	return selected_window;
		//	break;
		//case EXIT_WIND:
		//	return CLOSE_GAME;
		//}
	}

};
int Exit::count_of_wind = 0;
class Menu : public CurrentWindow
{
public:
	Menu(int x_but, int y_but, int margin,
		int height_but, int width_but,
		int count_of_but = ButtonNameInMenu::COUNT_OF_BUTTONS) :CurrentWindow(x_but, y_but, margin, height_but, width_but, count_of_but, Window::EXIT_WIND)
	{

	}
	bool LoadButton()override
	{
		bool success = true;
		if (!button[EXIT].loadButton("img\\Menu\\Exit.png"))
		{
			success = false;
		}
		if (!button[GALLERY].loadButton("img\\Menu\\Gallery.png"))
		{
			success = false;
		}
		if (!button[GAME].loadButton("img\\Menu\\Play.png"))
		{
			success = false;
		}
		return success;
	}
	bool LoadBg()override
	{
		bool success = true;
		if (!texture_bg.loadFromFile("img\\Menu\\Background.png"))
		{
			success = false;
		}
		return success;
	}
	int SelectedButtonInCurrentWindow(int button)override
	{
		switch (button)
		{
		case GAME:
			return GAME_WIND;
			break;
		case GALLERY:
			return GALLERY_WIND;
			break;
		case EXIT:
		case EXIT_WIND:
			return EXIT_WIND;
			break;

		}
	}
};
class LevelSelection : public CurrentWindow
{
public:
	LevelSelection(int x_but, int y_but, 
		int margin, int height_but, 
		int width_but,
		int count_of_but = COUNT_OF_LEVELSELECTION_BUTTONS)
		:CurrentWindow(x_but, y_but, margin, height_but, width_but, count_of_but, Window::GAME_WIND)
	{

	}
	bool LoadButton()override
	{
		bool success = true;
		if (!button[LEVEL1].loadButton("img\\Level\\Level1.png"))
		{
			success = false;
		}
		if (!button[LEVEL2].loadButton("img\\Level\\Level2.png"))
		{
			success = false;
		}
		if (!button[LEVEL3].loadButton("img\\Level\\Level3.png"))
		{
			success = false;
		}
		if (!button[BACK].loadButton("img\\Level\\Back.png"))
		{
			success = false;
		}
		return success;
	}
	bool LoadBg()override
	{
		bool success = true;
		if (!texture_bg.loadFromFile("img\\Background_Gallery.png"))
		{
			success = false;
		}
		return success;
	}
	int SelectedButtonInCurrentWindow(int button)override
	{
		switch (button)
		{
		case EXIT_WIND:
			return EXIT_WIND;
			break;
		}
	}
};
class Gallery : public CurrentWindow
{
private:
	Picture pictures[COUNT_OF_PIC];
	int selected_pic = FIRST;
public:
	Gallery(int x_but, int y_but, int margin,
		int height_but, int width_but,
		int count_of_but = COUNT_BUT_GAL)
		:CurrentWindow(x_but, y_but, margin, height_but, width_but, count_of_but, Window::GALLERY_WIND, false)
	{
		for (int i = 0; i < COUNT_OF_PIC; i++)
		{
			pictures[i].setPosition(281, 45);
		}
	}
	bool LoadButton()override
	{
		bool success = true;
		if (!button[NEXT].loadButton("img\\Buttons Gallery\\next.png"))
		{
			success = false;
		}
		if (!button[ButtonInGallery::BACK_GAL].loadButton("img\\Buttons Gallery\\back.png"))
		{
			success = false;
		}
		return success;
	}
	bool LoadBg()override
	{
		bool success = true;
		if (!texture_bg.loadFromFile("img\\Background_Gallery.png"))
		{
			success = false;
		}
		return success;
	}
	bool Load_Pics()
	{
		bool success = true;
		if (!pictures[FIRST].loadFromFile("img\\Gallery\\1.png"))
		{
			success = false;
		}
		if (!pictures[SECOND].loadFromFile("img\\Gallery\\2.png"))
		{
			success = false;
		}
		if (!pictures[THIRD].loadFromFile("img\\Gallery\\2.png"))
		{
			success = false;
		}
		return success;
	}
	int SelectedButtonInCurrentWindow(int button)override
	{
		switch (button)
		{
		case ButtonInGallery::BACK_GAL:
			switch (selected_pic)
			{
			case FIRST:
				selected_pic = FIRST;
				return Window::MENU_WIND;
				break;
			case SECOND:
			case THIRD:
				selected_pic--;
				return GALLERY_WIND;
				break;
			}
			break;
		case NEXT:
			switch (selected_pic)
			{
			case FIRST:
			case SECOND:
				selected_pic++;
				return GALLERY_WIND;
				break;
			case THIRD:
				selected_pic = FIRST;
				return Window::MENU_WIND;
				break;
			}
			break;
		case EXIT_WIND:

			return EXIT_WIND;

			break;
		}
	}
	void ShowPics()
	{
		pictures[selected_pic].renderP();
	}
};
class PuzzlePiece
{
private:
	//Top left position
	SDL_Point mPosition;

	SDL_Rect PuzzlepieceClip;
	LTexture PuzzleSpriteSheetTexture;

	bool selected = false;

public:
	//Initializes internal variables
	PuzzlePiece()
	{
		mPosition.x = 0;
		mPosition.y = 0;
	}

	//Sets top left position
	void setPosition(int x, int y)
	{
		mPosition.x = x;
		mPosition.y = y;
	}

	//Shows button sprite
	void render()
	{
		//Show current button sprite
		PuzzleSpriteSheetTexture.render(mPosition.x, mPosition.y, &PuzzlepieceClip);
	}

	void Set_PuzzlepieceClip(int x_coor, int y_coor, int width, int height)
	{
		PuzzlepieceClip.x = x_coor;
		PuzzlepieceClip.y = y_coor;
		PuzzlepieceClip.w = width;
		PuzzlepieceClip.h = height;
	}

	bool loadPuzzlepiece(string link)
	{
		//Loading success flag
		bool success = true;

		//Load sprites
		if (!PuzzleSpriteSheetTexture.loadFromFile(link))
		{
			printf("Failed to load button sprite texture!\n");
			success = false;
		}
		return success;
	}

};
class Puzzle
{
private:
	//Top left position
	SDL_Point mPosition;

	PuzzlePiece Puzzlepieces[PUZZLEPIECES_VERT][PUZZLEPIECES_HOR];

public:
	//Initializes internal variables
	Puzzle()
	{
		mPosition.x = 0;
		mPosition.y = 0;
	}

	//Sets top left position
	void setPosition(int x, int y)
	{
		mPosition.x = x;
		mPosition.y = y;
	}

	//Shows button sprite
	void render(int x_coor, int y_coor, int vert, int hor)
	{
		//Show current button sprite
		Puzzlepieces[vert][hor].setPosition(x_coor, y_coor);
		Puzzlepieces[vert][hor].render();
	}

	bool loadPuzzle(string link)
	{
		//Loading success flag
		bool success = true;

		//Load puzzle pieces textures and set sprites
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				Puzzlepieces[i][j].loadPuzzlepiece("img/Puzzle1.png");
				Puzzlepieces[i][j].Set_PuzzlepieceClip(j * PUZZLEPIECE_WIDTH, i * PUZZLEPIECE_HEIGHT, PUZZLEPIECE_WIDTH, PUZZLEPIECE_HEIGHT);
			}
		}
		return success;
	}
};

bool is_exit(int selected_window, SDL_Event ev)
{
	Exit e;

	if (e.GetCountOfWind() > 1)
	{

		if (x_is_pressed(ev))
			return true;
		return false;

	}
	e.SetSelectedWindow(selected_window);
	if (e.click_window() == CLOSE_GAME)
		return true;
	else
		return false;
}

