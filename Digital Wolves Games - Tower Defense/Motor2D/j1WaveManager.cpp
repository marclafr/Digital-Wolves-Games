#include "j1WaveManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "j1Map.h"
#include "Units.h"
#include "j1Pathfinding.h"
#include "p2Log.h"

j1WaveManager::j1WaveManager() : j1Module()
{
	name.assign("wave_manager");
}

j1WaveManager::~j1WaveManager() {}

bool j1WaveManager::Start()
{
	//TODO create waves in grups
	Wave wave1;
	UnitGroup group1(U_TWOHANDEDSWORDMAN, 5, LEFT_UP);
	wave1.PushBack(group1);
	
	timer.Start();
	spawning = true;

	return true;
}

bool j1WaveManager::Update(float dt)
{
	if (timer.ReadSec() >= TIME_BETWEEN_WAVES)
	{
		timer.Start();
		wave_num++;
		group_num = 0;
		unit_num = 0;
		spawning = true;
	}

	if(spawning)
	{
		if (delay_timer.ReadSec() >= TIME_BETWEEN_UNITS)
		{
			delay_timer.Start();
			if (wave_num < waves.size())
			{
				if (unit_num < waves[wave_num].units_vec[group_num].amount)
				{
					waves[wave_num].units_vec[group_num].Create();
					unit_num++;
				}
				else
				{
					group_num++;
					waves[wave_num].units_vec[group_num].Create();
					unit_num = 1;
				}
			}
			else
				spawning = false;
		}
	}
	return true;
}

bool j1WaveManager::CleanUp()
{
	wave_num = 0;
	waves.clear();
	return true;
}

Wave::~Wave()
{
	units_vec.clear();
}

void Wave::PushBack(UnitGroup unit_group)
{
	units_vec.push_back(unit_group);
}

fPoint UnitGroup::GetStartingPos(STARTING_ENEMY_POS pos) const
{
	//TODO rellenar posiciones
	switch (pos)
	{
	case LEFT_UP:
		return fPoint(0, 0);
	case LEFT_DOWN:
		return fPoint(0, 0);
	case RIGHT_UP:
		return fPoint(0, 0);
	case RIGHT_DOWN:
		return fPoint(0, 0);
	}
}

UnitGroup::UnitGroup(UNIT_TYPE type, int amount, STARTING_ENEMY_POS start_pos): type(type), amount(amount), start_pos(start_pos)
{}

void UnitGroup::Create() const
{
	App->entity_manager->CreateUnit(type, GetStartingPos(start_pos), S_ENEMY);
}
