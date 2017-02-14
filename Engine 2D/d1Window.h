#ifndef __d1WINDOW_H__
#define __d1WINDOW_H__

#include "d1Module.h"

struct SDL_Window;
struct SDL_Surface;

class d1Window : public d1Module
{
public:

	d1Window();

	// Destructor
	virtual ~d1Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	c2SString	title;
	uint		width;
	uint		height;
	uint		scale;
};

#endif // __j1WINDOW_H__