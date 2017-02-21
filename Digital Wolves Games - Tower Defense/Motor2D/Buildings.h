#ifndef _BUILDINGS
#define _BUILDINGS

#include "Entity.h"

enum BUILDING_TYPE
{
	NO_BUILDING = 0,
	TURRET //PER TENIR ALGO
};

class Building
{
private:
	enum BUILDING_TYPE building_type;

public:

	void Update(); // defines order

	virtual void AI();
	virtual void Draw();

};

#endif


