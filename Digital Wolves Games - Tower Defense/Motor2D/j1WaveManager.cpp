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
	wave_timer2.Start();
	wave_time_order.Start();
	totaltimer.Start();
	creation_pos_x1 = ENEMY_CREATION_POS_X1;
	creation_pos_y1 = ENEMY_CREATION_POS_Y1;
	creation_pos_x2 = ENEMY_CREATION_POS_X2;
	creation_pos_y2 = ENEMY_CREATION_POS_Y2;
	return true;
}

bool j1WaveManager::Update(float dt)
{
	if (wave_timer.ReadSec() >= WAVE_TIMER)
	{
		App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
		App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
		wave_timer.Start();
	}
	if (totaltimer.ReadSec() > 10 && wave_timer2.ReadSec() >= 3)
	{
		App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
		App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
		wave_timer2.Start();
	}
	if (totaltimer.ReadSec() > 20 && wave_timer2.ReadSec() >= 2)
	{
		App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
		App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
		App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
		wave_timer2.Start();
	}
	return true;
}

bool j1WaveManager::CleanUp()
{
	wave_num = 0;

	return true;
}

int j1WaveManager::CreateWave(int wave, int time)
{/*
 int row_num = 1;

 if (time > oldtime + 200) {
 App->map->WorldToMap(creation_pos_x, creation_pos_y)
 }
 App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x, creation_pos_y), S_ENEMY);
 }
 for (int i = 0; i < (wave * 5); i++)
 {
 creation_pos_x += space_between_x;
 creation_pos_y += space_between_y;


 App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x, creation_pos_y), S_ENEMY);

 }
 */
	return 1;
}