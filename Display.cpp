#include "stdafx.h"
#include "Display.h"


Display::Display()
{

}


Display::~Display()
{
}

void Display::create(std::string _name, int _width, int _height)
{
	bool success = 1;
	main_window = SDL_CreateWindow(_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_SHOWN);
	if (main_window == NULL)
	{
		success = 0;
		fprintf_s(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create renderer for window
		main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
		if (main_renderer == NULL)
		{
			fprintf_s(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Initialize renderer color
			SDL_SetRenderDrawColor(main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags))
			{
				fprintf_s(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				success = false;
			}
		}
	}
	if (!success)
	{
		fprintf_s(stderr, "Display not created correctl! %s\n",SDL_GetError());
	}
}

void Display::draw(Texture * _texture, int _x, int _y)
{
	SDL_Rect dummy = { _x,_y,_texture->getWidth(),_texture->getHeight() };
	printf("%d %d %d %d", dummy.x, dummy.y, dummy.w, dummy.h);
	SDL_RenderCopy(main_renderer, _texture->getImage(),NULL,&dummy);
}

void Display::update()
{
	SDL_RenderPresent(main_renderer);
}

SDL_Renderer* Display::getRenderer()
{
	return main_renderer;
}

