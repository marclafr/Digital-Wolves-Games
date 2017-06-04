#ifndef __j1AUDIO_H__
#define __j1AUDIO_H__

#include "j1Module.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class j1Audio : public j1Module
{
public:

	j1Audio();

	// Destructor
	virtual ~j1Audio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void LoadAllFX();

	//FX UNITS
	unsigned int fx_twohanded_die01;
	unsigned int fx_twohanded_die02;
	unsigned int fx_twohanded_die03;
	unsigned int fx_twohanded_die04;
	unsigned int fx_twohanded_die05;
	unsigned int fx_attack01;
	unsigned int fx_attack02;
	unsigned int fx_attack03;

	//FX BUILDING
	unsigned int fx_building_destroyed;
	unsigned int fx_arrow;
	unsigned int fx_cannon;
	unsigned int fx_construction;

	//FX UI
	uint fx_click_btn;
	uint fx_cancelclick_btn;
private:

	_Mix_Music*				music = NULL;
	std::list<Mix_Chunk*>	fx;

};

#endif // __j1AUDIO_H__