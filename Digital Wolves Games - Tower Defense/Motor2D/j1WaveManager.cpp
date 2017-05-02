#include "j1WaveManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Entity.h"
#include "j1Map.h"
#include "Units.h"
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
	wave_timer5.Start();
	wave_timer6.Start();
	wavetime.Start();
	creation_pos_x1 = ENEMY_CREATION_POS_X1;
	creation_pos_y1 = ENEMY_CREATION_POS_Y1;
	creation_pos_x2 = ENEMY_CREATION_POS_X2;
	creation_pos_y2 = ENEMY_CREATION_POS_Y2;
	return true;
}

bool j1WaveManager::Update(float dt)
{
	Entity* hey = nullptr;
	if (totaltimer.ReadSec() > 10) {
		if (wave_timer.ReadSec() >= WAVE_TIMER)
		{
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				App->entity_manager->CreateUnit(U_MILITIA, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
				App->entity_manager->CreateUnit(U_MANATARMS, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
				wave_timer.Start();
		}
		if (wave_timer2.ReadSec() >= 2.5)
		{
	
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				App->entity_manager->CreateUnit(U_LONGSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
				App->entity_manager->CreateUnit(U_CHAMPION, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
				
				//TESTING
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_CAVALRYARCHER, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_HEAVYCAVALRYARCHER, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_KNIGHT, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_CAVALIER, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_PALADIN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_SIEGERAM, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				//--
				wave_timer2.Start();
	
		}
		if (wave_timer2.ReadSec() >= 2)
		{
				App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
				hey = App->entity_manager->CreateUnit(U_SPEARMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
				App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
				hey = App->entity_manager->CreateUnit(U_PIKEMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
				wave_timer2.Start();
			
		
		}
		if (totaltimer.ReadSec() > 150 && wave_timer3.ReadSec() >= 1.5)
		{

			App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
			hey = App->entity_manager->CreateUnit(U_ARCHER, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);
			
			App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
			hey = App->entity_manager->CreateUnit(U_ARBALEST, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);
		
			wave_timer3.Start();
		}
		if (totaltimer.ReadSec() > 300 && wave_timer5.ReadSec() >= 1.2)
		{

			App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);

			App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);

			wave_timer5.Start();
		}
		if (totaltimer.ReadSec() > 500 && wave_timer4.ReadSec() >= 1)
		{

			App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);

			App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);

			wave_timer4.Start();
		}
		if (totaltimer.ReadSec() > 800 && wave_timer6.ReadSec() >= 0.9)
		{

			App->map->WorldToMap(creation_pos_x1, creation_pos_y1);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x1, creation_pos_y1), S_ENEMY);

			App->map->WorldToMap(creation_pos_x2, creation_pos_y2);
			hey = App->entity_manager->CreateUnit(U_TWOHANDEDSWORDMAN, fPoint(creation_pos_x2, creation_pos_y2), S_ENEMY);

			wave_timer6.Start();
		}
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