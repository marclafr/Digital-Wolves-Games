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
	//UnitGroup group20(U_SIEGERAM, 3, LEFT_UP);
	UnitGroup group1(U_MANGONEL, 15, LEFT_UP);
	//wave1.PushBack(group20);
	wave1.PushBack(group1, LEFT_UP);
	Wave wave2;
	UnitGroup group2(U_TWOHANDEDSWORDMAN, 10, LEFT_DOWN);
	UnitGroup group3(U_LONGSWORDMAN, 7, LEFT_DOWN);
	UnitGroup group4(U_CAVALRYARCHER, 5, LEFT_DOWN);
	wave2.PushBack(group2, LEFT_DOWN);
	wave2.PushBack(group3, LEFT_DOWN);
	wave2.PushBack(group4, LEFT_DOWN);

	Wave wave3;
	UnitGroup group5(U_TWOHANDEDSWORDMAN, 20, RIGHT_UP);
	UnitGroup group6(U_LONGSWORDMAN, 12, RIGHT_UP);
	UnitGroup group7(U_CAVALRYARCHER, 8, RIGHT_UP);
	UnitGroup group8(U_HEAVYCAVALRYARCHER, 4, RIGHT_UP);
	UnitGroup group9(U_KNIGHT, 8, RIGHT_UP);
	wave3.PushBack(group5, RIGHT_UP);
	wave3.PushBack(group6, RIGHT_UP);
	wave3.PushBack(group7, RIGHT_UP);
	wave3.PushBack(group8, RIGHT_UP);
	wave3.PushBack(group9, RIGHT_UP);


	Wave wave4;
	UnitGroup group10(U_TWOHANDEDSWORDMAN, 20, RIGHT_DOWN);
	UnitGroup group11(U_LONGSWORDMAN, 12, RIGHT_DOWN);
	UnitGroup group12(U_CAVALRYARCHER, 8, RIGHT_DOWN);
	UnitGroup group13(U_HEAVYCAVALRYARCHER, 4, RIGHT_DOWN);
	UnitGroup group14(U_KNIGHT, 8, RIGHT_DOWN);
	wave4.PushBack(group10, RIGHT_DOWN);
	wave4.PushBack(group11, RIGHT_DOWN);
	wave4.PushBack(group12, RIGHT_DOWN);
	wave4.PushBack(group13, RIGHT_DOWN);
	wave4.PushBack(group14, RIGHT_DOWN);


	Wave wave5;
	UnitGroup group15(U_TWOHANDEDSWORDMAN, 32, LEFT_UP);
	UnitGroup group16(U_LONGSWORDMAN, 22, LEFT_UP);
	UnitGroup group17(U_CAVALRYARCHER, 15, LEFT_UP);
	UnitGroup group18(U_HEAVYCAVALRYARCHER, 12, LEFT_UP);
	UnitGroup group19(U_KNIGHT, 10, LEFT_UP);
	UnitGroup group20(U_SIEGERAM, 3, LEFT_UP);
	wave5.PushBack(group15, LEFT_UP);
	wave5.PushBack(group16, LEFT_UP);
	wave5.PushBack(group17, LEFT_UP);
	wave5.PushBack(group18, LEFT_UP);
	wave5.PushBack(group19, LEFT_UP);
	wave5.PushBack(group20, LEFT_UP);
	/*
	Wave wave6;
	UnitGroup group21(U_TWOHANDEDSWORDMAN, 43, LEFT_UP);
	UnitGroup group22(U_LONGSWORDMAN, 34, LEFT_UP);
	UnitGroup group23(U_CAVALRYARCHER, 10, LEFT_UP);
	UnitGroup group24(U_HEAVYCAVALRYARCHER, 19, LEFT_UP);
	UnitGroup group25(U_KNIGHT, 18, LEFT_UP);
	UnitGroup group26(U_PALADIN, 13, LEFT_UP);
	UnitGroup group27(U_CHAMPION, 18, LEFT_UP);
	wave6.PushBack(group21);
	wave6.PushBack(group22);
	wave6.PushBack(group23);
	wave6.PushBack(group24);
	wave6.PushBack(group25);
	wave6.PushBack(group26);
	wave6.PushBack(group27);

	Wave wave7;
	UnitGroup group28(U_TWOHANDEDSWORDMAN, 52, LEFT_UP);
	UnitGroup group29(U_LONGSWORDMAN, 40, LEFT_UP);
	UnitGroup group(U_CAVALRYARCHER, 19, LEFT_UP);
	UnitGroup group30(U_HEAVYCAVALRYARCHER, 25, LEFT_UP);
	UnitGroup group31(U_KNIGHT, 28, LEFT_UP);
	UnitGroup group32(U_PALADIN, 33, LEFT_UP);
	UnitGroup group33(U_CHAMPION, 29, LEFT_UP);
	UnitGroup group34(U_SIEGERAM, 10, LEFT_UP);
	UnitGroup group35(U_CAVALRYARCHER, 19, LEFT_UP);
	wave7.PushBack(group27);
	wave7.PushBack(group28);
	wave7.PushBack(group29);
	wave7.PushBack(group30);
	wave7.PushBack(group31);
	wave7.PushBack(group32);
	wave7.PushBack(group33);
	wave7.PushBack(group34);
	wave7.PushBack(group35);


	Wave wave8;

	UnitGroup group36(U_LONGSWORDMAN, 51, LEFT_UP);
	UnitGroup group37(U_CAVALRYARCHER, 26, LEFT_UP);
	UnitGroup group38(U_HEAVYCAVALRYARCHER, 30, LEFT_UP);
	UnitGroup group39(U_KNIGHT, 34, LEFT_UP);
	UnitGroup group40(U_PALADIN, 33, LEFT_UP);
	UnitGroup group41(U_CHAMPION, 29, LEFT_UP);
	UnitGroup group42(U_MILITIA, 18, LEFT_UP);
	UnitGroup group43(U_SIEGERAM, 10, LEFT_UP);
	UnitGroup group44(U_TWOHANDEDSWORDMAN, 65, LEFT_UP);

	wave8.PushBack(group36);
	wave8.PushBack(group37);
	wave8.PushBack(group38);
	wave8.PushBack(group39);
	wave8.PushBack(group40);
	wave8.PushBack(group41);
	wave8.PushBack(group42);
	wave8.PushBack(group43);
	wave8.PushBack(group44);
	*/
	waves.push_back(wave1);
	waves.push_back(wave2);
	waves.push_back(wave3);
	waves.push_back(wave4);
	waves.push_back(wave5);
	//waves.push_back(wave6);
	//waves.push_back(wave7);
	//waves.push_back(wave8);

	timer.Start();
	spawning = true;

	return true;
}

bool j1WaveManager::Update(float dt)
{
	if (all_waves_ended == false)
	{
		if (timer.ReadSec() >= TIME_BETWEEN_WAVES && wave_ended == true)
		{
			timer.Start();
			wave_num++;

			if (waves[wave_num].units_vec_left_up.size() <= 0)
				left_up_finished = true;

			if (waves[wave_num].units_vec_left_down.size() <= 0)
				left_down_finished = true;

			if (waves[wave_num].units_vec_right_up.size() <= 0)
				right_up_finished = true;

			if (waves[wave_num].units_vec_right_down.size() <= 0)
				right_down_finished = true;

			group_num = 0;
			unit_num = 0;
			spawning = true;
			wave_ended = false;
		}

		if (spawning)
		{
			if (delay_timer.ReadSec() >= TIME_BETWEEN_UNITS && wave_ended == false)
			{
				delay_timer.Start();
				//LEFT_UP
				if (wave_num < waves.size() && left_up_finished == false && waves[wave_num].units_vec_left_up.size() > 0)
				{
					if (unit_num < waves[wave_num].units_vec_left_up[group_num].amount)
					{
						waves[wave_num].units_vec_left_up[group_num].Create();
						unit_num++;
					}
					else
					{
						if (group_num < waves[wave_num].units_vec_left_up.size() - 1)
						{
							group_num++;
							unit_num = 0;
						}
						else
							left_up_finished = true;
					}
				}
				//--
				//LEFT_DOWN
				if (wave_num < waves.size() && left_down_finished == false && waves[wave_num].units_vec_left_down.size() > 0)
				{
					if (unit_num < waves[wave_num].units_vec_left_down[group_num].amount)
					{
						waves[wave_num].units_vec_left_down[group_num].Create();
						unit_num++;
					}
					else
					{
						if (group_num < waves[wave_num].units_vec_left_down.size() - 1)
						{
							group_num++;
							unit_num = 0;
						}
						else
							left_down_finished = true;
					}
				}
				//--
				//RIGHT_UP
				if (wave_num < waves.size() && right_up_finished == false && waves[wave_num].units_vec_right_up.size() > 0)
				{
					if (unit_num < waves[wave_num].units_vec_right_up[group_num].amount)
					{
						waves[wave_num].units_vec_right_up[group_num].Create();
						unit_num++;
					}
					else
					{
						if (group_num < waves[wave_num].units_vec_right_up.size() - 1)
						{
							group_num++;
							unit_num = 0;
						}
						else
							right_up_finished = true;
					}
				}
				//--
				//RIGHT_DOWN
				if (wave_num < waves.size() && right_down_finished == false && waves[wave_num].units_vec_right_down.size() > 0)
				{
					if (unit_num < waves[wave_num].units_vec_right_down[group_num].amount)
					{
						waves[wave_num].units_vec_right_down[group_num].Create();
						unit_num++;
					}
					else
					{
						if (group_num < waves[wave_num].units_vec_right_down.size() - 1)
						{
							group_num++;
							unit_num = 0;
						}
						else
							right_down_finished = true;
					}
				}
				//IF ALL PATHS ENDED
				if (left_up_finished == true && left_down_finished == true
					&& right_up_finished == true && right_down_finished == true)
				{
					if (wave_num > waves.size())
						all_waves_ended = true;
					else
					{
						left_up_finished = false;
						left_down_finished = false;
						right_up_finished = false;
						right_down_finished = false;
						spawning = false;
						wave_ended = true;
						timer.Start();
					}
				}
				//--
			}
		}
	}
	return true;
}

bool j1WaveManager::CleanUp()
{
	wave_num = -1;
	waves.clear();
	return true;
}

Wave::~Wave()
{
	units_vec_left_up.clear();
	units_vec_left_down.clear();
	units_vec_right_up.clear();
	units_vec_right_down.clear();
}

void Wave::PushBack(UnitGroup unit_group, STARTING_ENEMY_POS start_pos)
{
	switch (start_pos)
	{
	case LEFT_UP:
		units_vec_left_up.push_back(unit_group);
		break;
	case LEFT_DOWN:
		units_vec_left_down.push_back(unit_group);
		break;
	case RIGHT_UP:
		units_vec_right_up.push_back(unit_group);
		break;
	case RIGHT_DOWN:
		units_vec_right_down.push_back(unit_group);
		break;
	}
}

fPoint UnitGroup::GetStartingPos(STARTING_ENEMY_POS pos) const
{
	//TODO rellenar posiciones
	switch (pos)
	{
	case LEFT_UP:
		return fPoint(-1296, 2880);//2880 1032
		break;
	case LEFT_DOWN:
		return fPoint(-1020, 2900);
		break;
	case RIGHT_UP:
		return fPoint(-417, 3256);
		break;
	case RIGHT_DOWN:
		return fPoint(-275, 3325);
		break;
	default:
		break;
	}
}

UnitGroup::UnitGroup(UNIT_TYPE type, int amount, STARTING_ENEMY_POS start_pos) : type(type), amount(amount), start_pos(start_pos)
{}

void UnitGroup::Create() const
{
	App->entity_manager->CreateUnit(type, GetStartingPos(start_pos), S_ENEMY);
}