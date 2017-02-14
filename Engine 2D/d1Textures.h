#ifndef __d1TEXTURES_H__
#define __d1TEXTURES_H__

#include "d1Module.h"
#include "c2List.h"

struct SDL_Texture;
struct SDL_Surface;

class d1Textures : public d1Module
{
public:

	d1Textures();

	// Destructor
	virtual ~d1Textures();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	c2List<SDL_Texture*>	textures;
};


#endif // __d1TEXTURES_H__