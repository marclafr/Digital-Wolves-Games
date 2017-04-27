#ifndef _TASK
#define _TASK

#include "j1EntityManager.h"

class Task
{
private:	
	char* name;

public:
	virtual bool Execute() = 0;

};


//TASKS

class CreateTowerTask : public Task
{
private:
	BUILDING_TYPE type;
	fPoint position;
	Side side;

public:
	CreateTowerTask() : type(B_NO_BUILDING), position(fPoint(0.0f,0.0f)), side(S_NO_SIDE)
	{}
	CreateTowerTask(BUILDING_TYPE type,	fPoint position, Side side): type(type), position(position), side(side)
	{}

	void Set(BUILDING_TYPE type, fPoint position, Side side)
	{
		this->type = type;
		this->position = position;
		this->side = side;
	}

	bool Execute()
	{
		App->entity_manager->CreateBuilding(type, position, side);
	}
};
#endif