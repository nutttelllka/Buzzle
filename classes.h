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

bool is_exit(SDL_Event ev);

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

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	//Load PNG texture
	if (!IntroTextures[0].loadFromFile("img\\logo1.png"))
	{
		printf("Failed to load intro logo1 texture!\n");
		success = false;
	}

	//Load PNG texture
	if (!IntroTextures[1].loadFromFile("img\\logo2.png"))
	{
		printf("Failed to load intro logo2 texture!\n");
		success = false;
	}
	return success;
}
bool stop_timer = true;
DWORD WINAPI Intro(void* param)
{
	//Clear screen
	SDL_RenderClear(gRenderer);

	//Render texture to screen
	IntroTextures[0].render(0, 0);
	//Update screen
	SDL_RenderPresent(gRenderer);

	SDL_Delay(2000);

	//Clear screen
	SDL_RenderClear(gRenderer);
	//Render texture to screen
	IntroTextures[1].render(0, 0);
	//Update screen
	SDL_RenderPresent(gRenderer);
	SDL_Delay(2000);
	return 0;
}
class Picture : public LTexture
{
private:
	int x;
	int y;
	int height;
	int width;
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
	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}
	void setWidth(int width)
	{
		this->width = width;
	}
	void setHeight(int height)
	{
		this->height = height;
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
	void render_with_sprite(SDL_Rect* clip = NULL)
	{
		SDL_Rect renderQuad = { x, y, width, height };
		if (clip != NULL)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}
		render(x, y, &renderQuad);
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
	bool is_clicked = true;
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

	LTexture* getButtonSpriteSheetTexture()
	{
		return &gButtonSpriteSheetTexture;
	}

	void SetBlock(bool is_clicked)
	{
		this->is_clicked = is_clicked;
	}

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

	int handleEvent(SDL_Event* e)
	{
		int button_sprite = BUTTON_SPRITE_MOUSE_OUT;
		//If mouse event happened
		if (is_clicked)
		{
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
	bool loadButtonWithOrNotClip(string link)
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
			if (!is_clicked)
			{
				//Set sprites
				for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
				{
					gSpriteClips[i].x = 0;
					gSpriteClips[i].y = 0;
					gSpriteClips[i].w = width;
					gSpriteClips[i].h = height;
				}
				
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
		}

		return success;
	}

};
class Cutting
{
private:
	//Top left position
	SDL_Point Position;
	LTexture pic;
	//Currently used sprite
	int CurrentSprite;
	SDL_Rect sprite_clips[Number_sprite::COUNT_OF_DIG];
	int height;
	int width;
public:
	Cutting() :Cutting(WIDTH_OF_PIC, HEIGHT_OF_PIC, 0, 61)
	{
	}
	Cutting(int width, int height, int x, int y)
	{
		setHeight(height);
		setWidth(width);
		setPosition(x, y);

		CurrentSprite = ZERO;
	}
	~Cutting()
	{
		free();
	}
	void  free()
	{
		pic.free();
		cout << "Ya ochistilsa\n";
	}
	void setHeight(int height)
	{
		this->height = height;
	}
	void setWidth(int width)
	{
		this->width = width;
	}
	void SetCurrentSprite(int CurrentSprite)
	{
		this->CurrentSprite = CurrentSprite;
	}
	/// <summary>
	/// Sets top left position
	/// </summary>
	/// <param name="x">X coordinate</param>
	/// <param name="y">Y coordinate</param>

	void SetMCurrentSprite(Number_sprite sprite)
	{
		CurrentSprite = sprite;
	}
	void setPosition(int x, int y)
	{
		Position.x = x;
		Position.y = y;
	}
	bool loadNumber(string link)
	{
		//Loading success flag
		bool success = true;

		//Load sprites
		if (!pic.loadFromFile(link))
		{
			printf("Failed to load button sprite texture!\n");
			success = false;
		}
		else
		{
			//Set sprites
			for (int i = 0; i < Number_sprite::COUNT_OF_DIG; ++i)
			{
				sprite_clips[i].x = i * WIDTH_OF_PIC;
				sprite_clips[i].y = 0;
				sprite_clips[i].w = WIDTH_OF_PIC;
				sprite_clips[i].h = HEIGHT_OF_PIC;
			}
		}

		return success;
	}
	void show_with_shift_x(int x, int current_num)
	{
		CurrentSprite = current_num;
		pic.render(x, Position.y, &sprite_clips[CurrentSprite]);
		//SDL_RenderPresent(gRenderer);
	}
};
class Timer
{
	int size;
	int min;//ìèíóòû
	int sec;
	int count_of_sec = 0;
	int x = 0, y = 0, z = 0, w = 0; //èçìåíåíèå îïðåäåëåííûõ öèôð âî âðåìåíè

	Cutting numbers;
public:
	Timer() : Timer(90, 30) {}
	Timer(int count_of_sec, int width_of_numb)
	{
		this->count_of_sec = count_of_sec;
		size = width_of_numb;
		min = 0;
		//texture_of_number.y = 35;
		sec = this->count_of_sec;
		//loadNumb();
	}
	bool loadNumb()
	{
		bool success = true;
		if (!numbers.loadNumber("img\\Gameplay\\number.png"))
		{
			success = false;
		}
		return success;
	}
	void Converter()
	{
		while (sec > 60)
		{
			min++;
			sec -= 60;
		}
	}
	void PrintAll()
	{
		x = sec % 10;
		numbers.show_with_shift_x(SCREEN_WIDTH / 2 + size / 2 + size, x);

		y = (sec / 10);
		numbers.show_with_shift_x(SCREEN_WIDTH / 2 + size / 2, y);

		numbers.show_with_shift_x(SCREEN_WIDTH / 2 - size / 2, Number_sprite::TWO_DOT);

		z = min % 10;
		numbers.show_with_shift_x(SCREEN_WIDTH / 2 - size / 2 - size, z);

		w = (min / 10);
		numbers.show_with_shift_x(SCREEN_WIDTH / 2 - size / 2 - size * 2, w);

	}
	void timeGo()
	{
		//loadNumb();
		Converter();
		PrintAll();
		while (!stop_timer)
		{
			if (sec == 0) {
				if (min - 1 >= 0) {
					min--;
					sec = 60;
					z = min % 10;
					numbers.show_with_shift_x(SCREEN_WIDTH / 2 - size / 2 - size, z);

					if ((min / 10) - 1 >= 0) {
						w = (min / 10);
						numbers.show_with_shift_x(SCREEN_WIDTH / 2 - size / 2 - size * 2, w);


					}
				}
			}
			cout << w << z << ":" << y << x << endl;
			sec--;
			if (x == 0) {
				y = (sec / 10) % 10;
				numbers.show_with_shift_x(SCREEN_WIDTH / 2 + size / 2, y);

			}
			x = sec % 10;
			numbers.show_with_shift_x(SCREEN_WIDTH / 2 + size / 2 + size, x);

			count_of_sec--;
			if (min == 0 && sec == 0) 
			{
				stop_timer = true;
				return;
			}

			this_thread::sleep_for(chrono::milliseconds(1000));
		}
		//numbers.free();
		
		//Спроси как делать очистку от картинок
	}
	void Reset()
	{
		sec = this->count_of_sec;
		min = 0;
		Converter();
	}
	//Getters
	int GetCountSec()
	{
		return count_of_sec;
	}
	//Setters
	void SetCountSec(unsigned int digit)
	{

		count_of_sec = digit;
	}
};
class File
{
	string path ;
	
public:

	File():File("level.txt")
	{

	}
	File(string path)
	{
		this->path = path;
	}
	File(string path, string to_write)
	{
		this->path = path;
		rewrite_in_file(to_write);
	}
	void rewrite_in_file(string to_write)
	{
		ofstream myfile(path);
		if (myfile.is_open())
		{
			myfile << to_write;
			myfile.close();
		}
		else
			cout << "couldn't open file\n";
		myfile.close();
	}
	void rewrite_in_file_num(int num)
	{
		stringstream stream;
		stream << num;
		string str;
		stream >> str;
		rewrite_in_file(str);

	}
	string inf_from_file()
	{
		string in_file;
		ifstream myfile(path);
		if (myfile.is_open())
		{
			myfile >> in_file;
			myfile.close();
			cout << in_file;
		}
		else
			cout << "couldn't open file\n";
		myfile.close();
	return in_file;
	}
	int num_from_file()
	{
		return stoi(inf_from_file());
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
		vector <int> previos_click;
			for (int i = 0; i < count_of_but; i++)
			{
				previos_click.push_back(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
			}
		SDL_Event e;
		Show_window();
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				stop_timer = true;
				if (x_or_esc_is_pressed(e))
				{

					if (is_exit(e))
						return CLOSE_GAME;
					Show_window();
				}
					//return SelectedButtonInCurrentWindow(EXIT_WIND);

				for (int i = 0; i < count_of_but; i++)
				{
					event_buttons[i] = button[i].handleEvent(&e);
				}
			}
			for (int i = 0; i < count_of_but; i++)
			{
				if (event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
				{
					//for (int i = 0; i < count_of_but; i++)
					//{
						button[i].render();
					//}
					SDL_RenderPresent(gRenderer);
					SDL_Delay(200);
					//SDL_RenderClear(gRenderer);

					texture_bg.free();
					for (int i = 0; i < count_of_but; i++)
					{
						button[i].free();
					}

					button[i].SetMCurrentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
					return SelectedButtonInCurrentWindow(i);
				}
				else if (event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION 
					//|| event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OUT
					)
				{
					//SDL_RenderClear(gRenderer);
					//texture_bg.renderP();

					//for (int i = 0; i < count_of_but; i++)
					//{
						button[i].render();
					//}
						previos_click[i] = LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION;
				}
				else if (event_buttons[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OUT 
					&& previos_click[i] == LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION)
				{
						button[i].render();
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
	Exit() :CurrentWindow(306, 670, 460, 108, 430, ExitButtons::COUNT_OF_BUT,Window::EXIT_WIND, false)
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
int level()
{
	File level_in_file;
	return stoi(level_in_file.inf_from_file());
}
bool level_is_open(int current_level)
{
	if (level() > current_level)
		return true;
	else
		return false;
}
class Menu : public CurrentWindow
{
public:
	Menu() :CurrentWindow(535, 429, 125, 107, 430, COUNT_OF_BUTTONS, Window::EXIT_WIND)
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
	string getPathOfSelectedButton(int level, bool open)
	{
		string path;
		button[level].SetBlock(open);
		switch (level)
		{
		case LEVEL1:
				return "img\\Level\\Level1.png";
		case LEVEL2:
			if (open)
			{
				return "img\\Level\\Level2.png";
			}
			else
			{
				
				return "img\\Level\\Block.png";
			}
		case LEVEL3:
			if (open)
			{
				return "img\\Level\\Level3.png";
			}
			else
				return "img\\Level\\Block.png";
		}
	}
	bool LoadButton()override
	{
		bool success = true;
		if (!button[LEVEL1].loadButton("img\\Level\\Level1.png"))
		{
			success = false;
		}
		if (!button[LEVEL2].loadButtonWithOrNotClip(getPathOfSelectedButton(LEVEL2, level_is_open(LEVEL2-1))))
		{
			success = false;
		}
		if (!button[LEVEL3].loadButtonWithOrNotClip
		(getPathOfSelectedButton(LEVEL3, level_is_open(LEVEL3-1))))
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
		if (!texture_bg.loadFromFile("img\\Level\\Background.png"))
		{
			success = false;
		}
		return success;
	}
	int SelectedButtonInCurrentWindow(int button)override
	{
		switch (button)
		{
		case LevelSelectionButtonName::LEVEL1:
			return Window::LEVEL1_WIND;
		case LevelSelectionButtonName::LEVEL2:
			return Window::LEVEL2_WIND;
		case LevelSelectionButtonName::LEVEL3:
			return Window::LEVEL3_WIND;
		case LevelSelectionButtonName::BACK:
			return MENU_WIND;
		}
	}
};
class Gallery : public CurrentWindow
{
private:
	//File level_in_file;
	Picture pictures[COUNT_OF_PIC];
	static int selected_pic;
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

	string getPathOfSelectedPic(int level,bool open)
	{
		string path;
		switch (level)
		{
		case Pictures::FIRST:
			if (open)
			{
				return "img\\Gallery\\1opened.png";
			}
			else
				return "img\\Gallery\\1not_opened.png";
		case Pictures::SECOND:
			if (open)
			{
				return "img\\Gallery\\2opened.png";
			}
			else
				return "img\\Gallery\\2not_opened.png";
		case Pictures::THIRD:
			if (open)
			{
				return "img\\Gallery\\3opened.png";
			}
			else
				return "img\\Gallery\\3not_opened.png";
		}
	}
	bool LoadBg()override
	{
		bool success = true;
		if (!texture_bg.loadFromFile(getPathOfSelectedPic(selected_pic, level_is_open(selected_pic))))
		{
			success = false;
		}
		return success;
	}
	bool Load_Pics()
	{
		bool success = true;

		if (!pictures[FIRST].loadFromFile("img\\Gallery\\1 not_opened.png"))
		{
			success = false;
		}
		if (!pictures[SECOND].loadFromFile("img\\Gallery\\1 not_opened.png"))
		{
			success = false;
		}
		if (!pictures[THIRD].loadFromFile("img\\Gallery\\1 not_opened.png"))
		{
			success = false;
		}
		if(level() >= Pictures::SECOND)
		{
			if (!pictures[FIRST].loadFromFile("img\\Gallery\\1opened.png"))
			{
				success = false;
			}
		}
		else if (level() >= Pictures::THIRD)
		{
			if (!pictures[SECOND].loadFromFile("img\\Gallery\\1opened.png"))
			{
				success = false;
			}
		}
		else if (level() > Pictures::THIRD)
		{
			if (!pictures[THIRD].loadFromFile("img\\Gallery\\1opened.png"))
			{
				success = false;
			}
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
			case THIRD:
				selected_pic = FIRST;
				return Window::MENU_WIND;
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
int Gallery::selected_pic = FIRST;
//bool is_exit(int selected_window, SDL_Event ev)
//{
//	Exit e;
//
//	if (e.GetCountOfWind() > 1)
//	{
//		if (x_is_pressed(ev))
//			return true;
//		return false;
//	}
//	e.SetSelectedWindow(selected_window);
//	if (e.click_window() == CLOSE_GAME)
//		return true;
//	else
//		return false;
//}
bool is_exit( SDL_Event ev)
{
	Exit e;

	if (e.GetCountOfWind() > 1)
	{
		if (x_is_pressed(ev))
			return true;
		return false;
	}
	//e.SetSelectedWindow(selected_window);
	if (e.click_window() == CLOSE_GAME)
		return true;
	else
		return false;
}

class PuzzlePiece;

class Slot
{
private:
	int SLOT_WIDTH;
	int SLOT_HEIGHT;
	//Top left position
	SDL_Point mPosition;
	bool selected = false;
	//bool filled = false;
	int number;
	PuzzlePiece* contained_puzzlepiece = nullptr;

public:
	Slot()
	{
		SLOT_WIDTH = 0;
		SLOT_HEIGHT = 0;
		mPosition.x = 0;
		mPosition.y = 0;
	}

	//Sets top left position
	void setPosition(int x, int y)
	{
		mPosition.x = x;
		mPosition.y = y;
	}

	SDL_Point getPosition()
	{
		return mPosition;
	}

	int getNumber()
	{
		return number;
	}

	void setNumber(int number)
	{
		this->number = number;
	}

	PuzzlePiece* getContainedPuzzlepiece()
	{
		return contained_puzzlepiece;
	}

	void setContainedPuzzlepiece(PuzzlePiece* puzzlepiece)
	{
		contained_puzzlepiece = puzzlepiece;
	}

	int getSlotWidth()
	{
		return SLOT_WIDTH;
	}

	int getSlotHeight()
	{
		return SLOT_HEIGHT;
	}

	void setSlotDimensions(int width, int height)
	{
		SLOT_WIDTH = width;
		SLOT_HEIGHT = height;
	}

	bool IsSelected()
	{
		return selected;
	}

	void setSelected(bool selected)
	{
		this->selected = selected;
	}

	int handleEvent(SDL_Event* e)
	{
		//If mouse event happened
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
		{
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (x < mPosition.x
				|| x > mPosition.x + SLOT_WIDTH
				|| y < mPosition.y
				|| y > mPosition.y + SLOT_HEIGHT)
				inside = false;

			//Mouse is inside button
			if (inside)
			{
				if (e->type == SDL_MOUSEBUTTONDOWN)
				{
					if (contained_puzzlepiece == nullptr)
					{
						selected = true;
						cout << "Slot selected. contained_puzzlepiece == nullptr\n";
					}
					else if (contained_puzzlepiece != nullptr)
					{
						cout << "Slot contains puzzlepiece\n";
					}
					/*
					// if it's not filled, then fill it
					if (!filled)
					{
						filled = true;

					}
					// if it's already been filled, then cancel filling
					else
					{
						filled = false;
						cout << "Slot unclicked";
					}
					*/
				}
			}
		}
		return selected;
	}
};

class PuzzlePiece
{
private:
	int PUZZLEPIECE_WIDTH;
	int PUZZLEPIECE_HEIGHT;
	//Top left position
	SDL_Point mPosition;
	SDL_Point mDefaultPosition;
	PuzzlepieceSprites CurrentSprite;
	SDL_Rect PuzzlepieceClip[PUZZLEPIECE_SPRITE_TOTAL];
	LTexture PuzzleSpriteSheetTexture;
	bool selected = false;
	bool in_slot = false;
	int number;

public:
	//Initializes internal variables
	PuzzlePiece()
	{
		PUZZLEPIECE_WIDTH = 0;
		PUZZLEPIECE_HEIGHT = 0;
		mPosition.x = 0;
		mPosition.y = 0;
		CurrentSprite = NOT_SELECTED;
	}

	LTexture* getPuzzleSpriteSheetTexture()
	{
		return &PuzzleSpriteSheetTexture;
	}

	int getPuzzlepieceWidth()
	{
		return PUZZLEPIECE_WIDTH;
	}

	int getPuzzlepieceHeight()
	{
		return PUZZLEPIECE_HEIGHT;
	}

	void setPuzzlepieceDimensions(int width, int height)
	{
		PUZZLEPIECE_WIDTH = width;
		PUZZLEPIECE_HEIGHT = height;
	}

	//Sets top left position
	void setPosition(int x, int y)
	{
		mPosition.x = x;
		mPosition.y = y;
	}
	// Sets to default position
	void setPosition()
	{
		mPosition.x = mDefaultPosition.x;
		mPosition.y = mDefaultPosition.y;
	}

	void setDefaultPosition(int x, int y)
	{
		mDefaultPosition.x = x;
		mDefaultPosition.y = y;
	}

	void setNumber(int number)
	{
		this->number = number;
	}

	int getNumber()
	{
		return number;
	}

	// 'selected' field getter
	bool IsSelected()
	{
		return selected;
	}

	void setInSlot(bool in_slot)
	{
		this->in_slot = in_slot;
	}

	void SetSelected(bool selected)
	{
		this->selected = selected;
	}

	void setCurrentSprite(PuzzlepieceSprites sprite)
	{
		CurrentSprite = sprite;
	}

	//Shows button sprite
	void render()
	{
		//Show current button sprite
		PuzzleSpriteSheetTexture.render(mPosition.x, mPosition.y, &PuzzlepieceClip[CurrentSprite]);
	}

	void Set_PuzzlepieceClip(int x_coor, int y_coor, int width, int height)
	{
		PuzzlepieceClip[0].x = x_coor;
		PuzzlepieceClip[0].y = y_coor;
		PuzzlepieceClip[0].w = width;
		PuzzlepieceClip[0].h = height;

		PuzzlepieceClip[1].x = x_coor;
		PuzzlepieceClip[1].y = y_coor + PuzzleSpriteSheetTexture.getHeight() / 2;
		PuzzlepieceClip[1].w = width;
		PuzzlepieceClip[1].h = height;
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

	//Handles mouse event
	bool handleEvent(SDL_Event* e, PuzzlePiece** selected_puzzlepiece, Slot** selected_slot)
	{
		bool event_caught = false;
		//If mouse event happened
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
		{

			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (x < mPosition.x
				|| x > mPosition.x + PUZZLEPIECE_WIDTH
				|| y < mPosition.y
				|| y > mPosition.y + PUZZLEPIECE_HEIGHT)
				inside = false;

			//Mouse is inside button
			if (inside)
			{
				if (e->type == SDL_MOUSEBUTTONDOWN)
				{
					// If we're working with the puzzlepiece we have prevously selected (else we don't handle the event)
					if (!in_slot)
					{
						if (selected && *selected_puzzlepiece == this)
						{
							selected = false;
							CurrentSprite = NOT_SELECTED;
							cout << "Puzzlepiece " << number << " selected && selected_puzzlepiece == this\n";
						}
						else if (!selected && *selected_puzzlepiece == this)
						{
							selected = true;
							CurrentSprite = SELECTED;
							cout << "Puzzlepiece " << number << " !selected && selected_puzzlepiece == this\n";
						}
						else if (selected && *selected_puzzlepiece != this)
						{
							selected = false;
							CurrentSprite = NOT_SELECTED;
							cout << "Puzzlepiece " << number << " selected && selected_puzzlepiece != this\n";
						}
						else if (!selected && *selected_puzzlepiece != this)
						{
							if (*selected_puzzlepiece != nullptr)
							{
								(*selected_puzzlepiece)->SetSelected(false);
								(*selected_puzzlepiece)->setCurrentSprite(NOT_SELECTED);
							}
							selected = true;
							CurrentSprite = SELECTED;
							*selected_puzzlepiece = this;
							cout << "Puzzlepiece " << number << " !selected && selected_puzzlepiece != this\n";
						}

					}
					else if (in_slot)
					{
						//selected_puzzlepiece = nullptr;
						/*
						if (*selected_puzzlepiece == this)
						{
							cout << "Selected puzzlepiece dropped\n";
							*selected_puzzlepiece = nullptr;
						}
						*/
						cout << "Puzzlepiece moved to default position\n";
						setPosition();
						in_slot = false;
						if (*selected_slot != nullptr)
						{
							(*selected_slot)->setSelected(false);
							(*selected_slot)->setContainedPuzzlepiece(nullptr);
							*selected_slot = nullptr;
							cout << "Puzzlepiece dropped from slot\n";
						}
					}
					event_caught = true;
				}
			}
		}
		return event_caught;
	}

	bool UpdatePosition(Slot** selected_slot)
	{
		bool event_caught = false;
		if (*selected_slot != nullptr)
		{
			(*selected_slot)->setSelected(false);
			//cout << "Slot unselected\n";
			if (!in_slot)
			{
				//cout << "Puzzlepiece not in slot\n";
				// If slot is empty
				if ((*selected_slot)->getContainedPuzzlepiece() == nullptr)
				{
					cout << "Puzzlepiece moved to slot\n";
					mPosition = (*selected_slot)->getPosition();
					in_slot = true;
					selected = false;
					CurrentSprite = NOT_SELECTED;
					(*selected_slot)->setContainedPuzzlepiece(this);
					//*selected_slot = nullptr;
					event_caught = true;
				}
			}
		}
		return event_caught;
	}

};

class Puzzle
{
private:
	int level;

	LTexture field_texture;
	int PUZZLEPIECES_HOR;
	int PUZZLEPIECES_VERT;
	int MARGIN;
	vector <vector <PuzzlePiece>> Puzzlepieces;
	vector <vector <Slot>> Slots;
	//PuzzlePiece** Puzzlepieces;
	//Slot** Slots;
	LButton back;
	LButton reset;
public:
	//Initializes internal variables
	Puzzle(int level)
	{
		this->level = level;
		switch (level)
		{
		case 1:
			PUZZLEPIECES_HOR = 3;
			PUZZLEPIECES_VERT = 3;
			break;
		case 2:
			PUZZLEPIECES_HOR = 4;
			PUZZLEPIECES_VERT = 4;
			break;
		case 3:
			PUZZLEPIECES_HOR = 5;
			PUZZLEPIECES_VERT = 5;
			break;
		default:
			break;
		}
		MARGIN = 10;
		back.setPosition(85, 30);
		back.setWidth(106);
		back.setHeight(114);
		reset.setHeight(114);
		reset.setWidth(106);
		reset.setPosition(1309,30);
		// Initializing puzzlepieces 2d vector 
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			vector <PuzzlePiece> pzz;
			Puzzlepieces.push_back(pzz);
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				PuzzlePiece p;
				Puzzlepieces[i].push_back(p);
			}
		}
		// Initializing slots 2d vector
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			vector <Slot> sl;
			Slots.push_back(sl);
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				Slot s;
				Slots[i].push_back(s);
			}
		}
	}
	// Deallocates all textures
	void Clean()
	{
		field_texture.free();
		back.getButtonSpriteSheetTexture()->free();
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				Puzzlepieces[i][j].getPuzzleSpriteSheetTexture()->free();
			}
		}
	}

	void Load_Bg()
	{
		switch (level)
		{
		case 1:
			field_texture.loadFromFile("img\\Gameplay\\Level1_bg.png");
			break;
		case 2:
			field_texture.loadFromFile("img\\Gameplay\\Level2_bg.png");
			break;
		case 3:
			field_texture.loadFromFile("img\\Gameplay\\Level3_bg.png");
			break;
		default:
			break;
		}
	}

	void RenderBG()
	{
		field_texture.render(0, 0);
		
	}

	void render()
	{

		//back.render();

		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				Puzzlepieces[i][j].render();
			}
		}
	}

	bool loadPuzzle()
	{
		string link;
		int width;
		int height;
		int start_def_x;
		int start_def_y;

		switch (level)
		{
		case 1:
			link = "img\\Gameplay\\Level1_puzzle.png";
			width = 200;
			height = 200;
			start_def_x = 65;
			start_def_y = 276;
			break;
		case 2:
			link = "img\\Gameplay\\Level2_puzzle.png";
			width = 150;
			height = 150;
			start_def_x = 60;
			start_def_y = 271;
			break;
		case 3:
			link = "img\\Gameplay\\Level3_puzzle.png";
			width = 120;
			height = 120;
			start_def_x = 55;
			start_def_y = 266;
			break;
		default:
			break;
		}

		for (int i = 0, number = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++, number++)
			{
				// Load puzzlepieces textures and set sprites
				Puzzlepieces[i][j].setPuzzlepieceDimensions(width, height);
				Puzzlepieces[i][j].loadPuzzlepiece(link);
				Puzzlepieces[i][j].Set_PuzzlepieceClip(j * Puzzlepieces[i][j].getPuzzlepieceWidth(), i * Puzzlepieces[i][j].getPuzzlepieceHeight(), Puzzlepieces[i][j].getPuzzlepieceWidth(), Puzzlepieces[i][j].getPuzzlepieceHeight());
				Puzzlepieces[i][j].setNumber(number);
				Puzzlepieces[i][j].setDefaultPosition(start_def_x + j * (Puzzlepieces[i][j].getPuzzlepieceWidth() + MARGIN), start_def_y + i * (Puzzlepieces[i][j].getPuzzlepieceHeight() + MARGIN));
				Puzzlepieces[i][j].setPosition();
				// Set slots configurations
				Slots[i][j].setSlotDimensions(width, height);
				Slots[i][j].setPosition(825 + j * Slots[i][j].getSlotWidth(), 286 + i * Slots[i][j].getSlotHeight());
				Slots[i][j].setNumber(number);
			}
		}
		return true;
	}
	bool loadButton()
	{
		bool success = true;
		if (!back.loadButton("img\\Gameplay\\Back.png"))
		{
			success = false;
		}
		if (!reset.loadButton("img\\Gameplay\\Reset.png"))
		{
			success = false;
		}
		return success;
	}
	bool AllSlotsFilled()
	{
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				if (Slots[i][j].getContainedPuzzlepiece() == nullptr)
				{
					return false;
				}
			}
		}
		return true;
	}

	bool IsOrderCorrect()
	{
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				if (Slots[i][j].getNumber() != Slots[i][j].getContainedPuzzlepiece()->getNumber())
				{
					return false;
				}
			}
		}
		return true;
	}

	void Reset(PuzzlePiece** selected_puzzlepiece, Slot** selected_slot)
	{
		for (int i = 0; i < PUZZLEPIECES_VERT; i++)
		{
			for (int j = 0; j < PUZZLEPIECES_HOR; j++)
			{
				Puzzlepieces[i][j].SetSelected(false);
				Puzzlepieces[i][j].setInSlot(false);
				Puzzlepieces[i][j].setPosition();
				Slots[i][j].setContainedPuzzlepiece(nullptr);
				Slots[i][j].setSelected(false);
			}
		}
		*selected_puzzlepiece = nullptr;
		*selected_slot = nullptr;

	}

	int Game()
	{
		loadPuzzle();
		Load_Bg();
		loadButton();
		RenderBG();
		back.render();
		render();
		SDL_RenderPresent(gRenderer);
		bool quit = false;
		SDL_Event e;

		Slot* selected_slot = nullptr;
		PuzzlePiece* selected_puzzlepiece = nullptr;
		Timer timer;
		timer.loadNumb();
		stop_timer = false;
		thread th(&Timer::timeGo, ref(timer));//ref - используется 
		th.detach();
		

		int event[2] = { LButtonSprite::BUTTON_SPRITE_MOUSE_OUT,LButtonSprite::BUTTON_SPRITE_MOUSE_OUT };
		bool pzz_event = false; // any puzzlepieces events flag (selection, moving, etc)
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{

				//User requests quit
				if (x_or_esc_is_pressed(e))
				{
					stop_timer = true;
					if (is_exit(e))
					{
						Clean();
						return CLOSE_GAME;
					}
						
					stop_timer = false;
					thread th(&Timer::timeGo, ref(timer));//ref - используется 
					th.detach();
					RenderBG();
					render();
					back.render();
					timer.PrintAll();
					
				}
				
				//Handlling events on slots and puzzlepoieces
				for (int i = 0; i < PUZZLEPIECES_VERT; i++)
				{
					for (int j = 0; j < PUZZLEPIECES_HOR; j++)
					{
						Slots[i][j].handleEvent(&e);
						if (Puzzlepieces[i][j].handleEvent(&e, &selected_puzzlepiece, &selected_slot))
						{
							pzz_event = true;
						}
					}
				}
				event[ButtonsInGame::BACK_TO_CH_LEV] = back.handleEvent(&e);
				event[ButtonsInGame::RESET] = reset.handleEvent(&e);
			}
			if (event[ButtonsInGame::BACK_TO_CH_LEV] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
			{

				back.render();
				stop_timer = true;
				SDL_RenderPresent(gRenderer);
				back.SetMCurrentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
				SDL_Delay(200);
				if (is_exit(e))
				{
					Clean();
					return Window::GAME_WIND;
				}
					
				stop_timer = false;
				thread th(&Timer::timeGo, ref(timer));//ref - используется 
				th.detach();
				RenderBG();
				back.render();
				render();
				timer.PrintAll();
				SDL_RenderPresent(gRenderer);
			}
			else if (event[ButtonsInGame::BACK_TO_CH_LEV] == LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION
				|| event[ButtonsInGame::BACK_TO_CH_LEV]== LButtonSprite::BUTTON_SPRITE_MOUSE_OUT)
			{
				//SDL_RenderClear(gRenderer);
				//RenderBG();
				back.render();
				SDL_RenderPresent(gRenderer);
			}

			if (event[ButtonsInGame::RESET] == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN)
			{
				reset.render();
				SDL_RenderPresent(gRenderer);
				SDL_Delay(200);
				//reset.SetMCurrentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
				reset.SetMCurrentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT);
				
				Reset(&selected_puzzlepiece, &selected_slot);
				//timer.Reset();
				RenderBG();
				render();
				back.render();
				reset.render();
				SDL_Delay(100);
				timer.PrintAll();
				SDL_RenderPresent(gRenderer);
			}
			else if (event[ButtonsInGame::RESET] == LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION
				|| event[ButtonsInGame::RESET] == LButtonSprite::BUTTON_SPRITE_MOUSE_OUT)
			{
				//SDL_RenderClear(gRenderer);
				//RenderBG();
				reset.render();
				//timer.PrintAll();
				SDL_RenderPresent(gRenderer);
			}
			//Finding a selected slot
			for (int i = 0; i < PUZZLEPIECES_VERT; i++)
			{
				for (int j = 0; j < PUZZLEPIECES_HOR; j++)
				{
					if (Slots[i][j].IsSelected())
					{
						selected_slot = &Slots[i][j];
						//cout << "Slot " << Slots[i][j].getNumber() << " selected\n";
						i = PUZZLEPIECES_VERT;
						break;
					}
				}
			}

			//Updating selected puzzlepiece
			for (int i = 0; i < PUZZLEPIECES_VERT; i++)
			{
				for (int j = 0; j < PUZZLEPIECES_HOR; j++)
				{
					if (Puzzlepieces[i][j].IsSelected())
					{
						selected_puzzlepiece = &Puzzlepieces[i][j];
						if (Puzzlepieces[i][j].UpdatePosition(&selected_slot))
						{
							pzz_event = true;
						}
						i = PUZZLEPIECES_VERT;
						break;
					}
				}
			}
			//Nuta Here is Exit, because timer is over.
			if (timer.GetCountSec() == 0) 
			{
				return Window::GAME_WIND;
			}
			if (AllSlotsFilled())
			{
				if (IsOrderCorrect())
				{
					File current_level_f;
					int current_lev = current_level_f.num_from_file();
					if (level - 1 == current_lev)
					{
						current_lev++;
					}
					stop_timer = true;
					current_level_f.rewrite_in_file_num(current_lev);
					cout << "Kruto\n";
					switch (level)
					{
					case 1:
						Clean();
						
						return Window::LEVEL2_WIND;
						break;
					case 2:
						Clean();
						return Window::LEVEL3_WIND;
						break;
					case 3:
						Clean();
						return Window::GAME_WIND;
						break;
					default:
						break;
					}
					quit = true;
				}
				else
				{
					cout << "Ne kruto\n";
					Reset(&selected_puzzlepiece, &selected_slot);
				}

			}
			if (pzz_event)
			{
				//SDL_RenderClear(gRenderer);
				RenderBG();
				render();
				back.render();
				reset.render();
				timer.PrintAll();
				SDL_Delay(100);
				SDL_RenderPresent(gRenderer);
			}
			pzz_event = false;
		}
		//Update screen
		//SDL_RenderPresent(gRenderer);
		
	}
};