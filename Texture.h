#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<stdio.h>

class Texture
{
public:
	Texture();
	~Texture();
	void LoadFromFile(SDL_Renderer* _rend,std::string _path);
	SDL_Texture* getImage();
	int getWidth();
	int getHeight();

private:
	SDL_Texture* image;
	int w, h;
};

