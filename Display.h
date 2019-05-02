#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<stdio.h>
#include<string>
#include "Texture.h"

class Display
{
public:
	Display();
	~Display();
	void create(std::string _name, int _width, int _height);
	void draw(Texture* _texture,int _x,int _y);
	void update();
	SDL_Renderer* getRenderer();
private:
	SDL_Window* main_window;
	SDL_Renderer* main_renderer;
};

