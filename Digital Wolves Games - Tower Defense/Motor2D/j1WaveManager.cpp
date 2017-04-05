#include "j1WaveManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

j1WaveManager::j1WaveManager() : j1Module()
{
	name.assign("wave_manager");
}

j1WaveManager::~j1WaveManager() {}

bool j1WaveManager::Start()
{
	wave_timer.Start();
	return true;
}

bool j1WaveManager::Update(float dt)
{
	if (wave_timer.ReadSec() >= WAVE_TIMER)
	{
		CreateWave(wave_num);
		wave_timer.Start();
	}
	return true;
}

bool j1WaveManager::CleanUp()
{
	wave_num = 0;
	creation_pos_x = ENEMY_CREATION_POS_X;
	creation_pos_y = ENEMY_CREATION_POS_Y;
	return true;
}

void j1WaveManager::CreateWave(int wave)
{
	wave_num++;
	int row_num = 1;
	for (int i = 0; i < (wave * 5); i++)
	{
		creation_pos_x += space_between_x;
		creation_pos_y += space_between_y;

		if (!App->pathfinding->IsWalkable(App->map->WorldToMap(creation_pos_x, creation_pos_y)))
		{
			creation_pos_x = ENEMY_CREATION_POS_X + 48 * row_num;
			creation_pos_y = ENEMY_CREATION_POS_Y + 24 * row_num;
			row_num++;
		}

		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x, creation_pos_y), S_ENEMY);
	}
}