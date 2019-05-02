#include "stdafx.h"
#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::LoadFromFile(SDL_Renderer* _rend,std::string _path)
{
	SDL_Surface* dummy = IMG_Load(_path.c_str());
	if (dummy == NULL)
	{
		fprintf_s(stderr, "Surface at path %s could not be loaded! %s", _path.c_str(), SDL_GetError());
	}
	else
	{
		image = SDL_CreateTextureFromSurface(_rend, dummy);
		if (image == NULL)
		{
			fprintf_s(stderr, "Surface loaded fine but texture could not be created from it! %s",SDL_GetError());
		}
		w = dummy->w;
		h = dummy->h;
		SDL_FreeSurface(dummy);
	}
}

SDL_Texture* Texture::getImage()
{
	return image;
}

int Texture::getWidth()
{
	return w;
}

int Texture::getHeight()
{
	return h;
}
