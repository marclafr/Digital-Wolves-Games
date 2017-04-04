#ifndef _WALLS
#define _WALLS

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1PerfTimer.h"
#include <vector>

enum WALL_TYPE
{
	STONE_WALL,
	NO_WALL
};

class Wall : public Entity
{
private:
	enum WALL_TYPE wall_type;
	float build_time;
	j1PerfTimer buildtimer;
	bool totallybuilded = false;
public:

	Wall(WALL_TYPE b_type, fPoint pos);

	void Update(); // defines order

	virtual void AI();
	virtual void Draw();

	const WALL_TYPE GetWallType() const;
	const double GetBuildTime() const;
};

#endif _WALLS


