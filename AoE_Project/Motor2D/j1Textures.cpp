#include "j1Textures.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1FileSystem.h"


#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

j1Textures::j1Textures() : j1Module()
{
	name = "textures";
}

// Destructor
j1Textures::~j1Textures()
{}

// Called before render is available
bool j1Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Textures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool j1Textures::CleanUp()
{
	//LOG("Freeing textures and Image library");
	int i = 0;
	std::list<SDL_Texture*>::iterator texture = textures.begin();
	uint w = 0, h = 0;

	/*while (texture != textures.end())
	{
		
		SDL_Texture* current_tex = texture._Ptr->_Myval;
		GetSize(current_tex, w, h);
		LOG("texture number %i || %p", i++, &current_tex);
		texture++;
		SDL_DestroyTexture(current_tex);
	}*/

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const j1Textures::Load(const char* path)
{
	LOG("%s ||%i", path,textures.size());
	
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(App->fs->Load(path), 1);
	

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format, 0xFF, 0, 0xFF));
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}
	LOG("%p", &texture);

	return texture;
}

// Unload texture
bool j1Textures::UnLoad(SDL_Texture* texture)
{
	std::list<SDL_Texture*>::iterator item = textures.begin();
	
	while(item != textures.end())
	{
		if(texture == item._Ptr->_Myval)
		{
			textures.remove(texture);
			SDL_DestroyTexture(texture);			
			return true;
		}
		item++;
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const j1Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if(texture == nullptr)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void j1Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
