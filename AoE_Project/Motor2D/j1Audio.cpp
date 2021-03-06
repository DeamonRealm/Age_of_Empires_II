#include "j1Audio.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1SoundManager.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name = "audio";
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}
	
	return ret;
}

bool j1Audio::PostUpdate()
{
	if (go_down && go_down_time.Read() > 250)
	{
		volume = 1;
		Mix_Volume(-1, volume);
		Mix_VolumeMusic(volume);
		go_down = false;
	}

	return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	if(!active)
		return true;

	//LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for(std::list<Mix_Chunk*>::iterator item = fx.begin(); item != fx.end();  item++)
		Mix_FreeChunk(item._Ptr->_Myval);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS_RW(App->fs->Load(path), 1);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(App->fs->Load(path), 1);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id < fx.size())
	{
		//Iterate audio fx list to find the correct fx
		std::list<Mix_Chunk*>::iterator item = fx.begin();
		for (uint k = 0; k < id; k++)
			item++;

		Mix_PlayChannel(-1, item._Ptr->_Myval, repeat);
	}

	return ret;
}

void j1Audio::VolumeUp()
{
	volume = MIN(volume + 10, 128);
	Mix_Volume(-1, volume);
	Mix_VolumeMusic(volume);
	if (volume == 128)
	{
		App->sound->PlayGUIAudio(SOUND_TYPE::ERROR_SOUND);
	}
}

void j1Audio::VolumeDown()
{
	volume = MAX(volume - MIN(volume,10), 1);
	Mix_Volume(-1, volume);
	Mix_VolumeMusic(volume);
	if (volume == 1)
	{
		go_down = true;
		Mix_Volume(-1, 120);
		App->sound->PlayGUIAudio(SOUND_TYPE::ERROR_SOUND);
		go_down_time.Start();
	}
}

void j1Audio::ResetAudioSystem()
{
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
	}

	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
	}
}
