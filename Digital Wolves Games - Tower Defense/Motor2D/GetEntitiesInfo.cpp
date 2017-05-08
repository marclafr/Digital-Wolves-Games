#define TWOHANDEDSWORDMAN_HP 170
#define CAVALRYARCHER_HP 50
#define SIEGERAM_HP 270
#define MANATARMS_HP 100
#define PALADIN_HP 200

#define TURRET_HP 250
#define WOODWALL_HP 500
#define STONEWALL_HP 500
#define TOWNHALL_HP 1500

#define RESOURCES_HP 10

#include "UIGetEntitiesInfo.h"

SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case U_TWOHANDEDSWORDMAN:
		ret = { 774, 962, 25, 25 };
		break;

	case U_CAVALRYARCHER:
		break;
	case U_GOD:
		ret = { 956, 935, 25, 25 };
		break;

	case U_SIEGERAM:
		ret = { 748, 962, 25, 25 };
		break;

	case U_PALADIN:
		ret = { 930, 910, 25, 25 };
		break;
	default:
		//LOG("Error UNIT TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0, 0, 0 };
		break;
	}

	return ret;
}

const char* GetUnitName(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
			//Infantry
	case U_MILITIA:
		ret = "Militia";
		break;

	case U_MANATARMS:
		ret = "Man at Arms";
		break;

	case U_LONGSWORDMAN:
		ret = "Long Swordman";
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = "Two-Handed Swordman";
		break;

	case U_CHAMPION:
		ret = "Champion";
		break;

	case U_SPEARMAN:
		ret = "Spearman";
		break;
		
		//Archers
	case U_ARCHER:
		ret = "Archer";
		break;

	case U_ARBALEST:
		ret = "Arbalest";
		break;

	case U_CAVALRYARCHER:
		ret = "Cavalry Archer";
		break;

	case U_HEAVYCAVALRYARCHER:
		ret = "Heavy Calvalry Archer";
		break;
		
		//Cavalry
	case U_KNIGHT:
		ret = "Knight";
		break;

	case U_CAVALIER:
		ret = "Cavalier";
		break;

	case U_PALADIN:
		ret = "Paladin";
		break;
		
		//Siege
	case U_SIEGERAM:
		ret = "Siege Ram";
		break;

		//Dev Units
	case U_GOD:
		ret = "GOD";
		break;

	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit";
		break;
	}

	return ret;
}

const char* GetUnitPrice(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
		//Infantry
		/*
	case U_MILITIA:
		ret = "Food: 60 Gold: 20";
		break;

	case U_MANATARMS:
		ret = "Food: 60 Gold: 20";
		break;

	case U_LONGSWORDMAN:
		ret = "Food: 60 Gold: 20";
		break;

	case U_TWOHANDEDSWORDMAN:
		ret = "Food: 60 Gold: 20";
		break;
		*/
	case U_CHAMPION:
		ret = "Food: 60 Gold: 20";
		break;

		/*
	case U_SPEARMAN:
		ret = "Food: 60 Wood: 20";
		break;
		*/
		//Archers
		/*
	case U_ARCHER:
		ret = "Wood: 60 Gold: 20";
		break;
		*/
	case U_ARBALEST:
		ret = "Wood: 60 Gold: 20";
		break;
		/*
	case U_CAVALRYARCHER:
		ret = "Wood: 60 Gold: 20";
		break;
		*/
	case U_HEAVYCAVALRYARCHER:
		ret = "Wood: 60 Gold: 20";
		break;

		//Cavalry
		/*
	case U_KNIGHT:
		ret = "Food: 60 Gold: 20";

	case U_CAVALIER:
		ret = "Food: 60 Gold: 20";
		break;
		*/
	case U_PALADIN:
		ret = "Food: 60 Gold: 20";
		break;

		//Siege
		/*
	case U_SIEGERAM:
		ret = "Wood: 60 Gold: 20 Ram";
		break;
		*/
		//Dev Units
		/*
	case U_GOD:
		ret = "Better than others, because I'M FREE!";
		break;
		*/
	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit";
		break;
	}

	return ret;
}

SDL_Rect GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case R_WOOD:
		ret = { 852, 858, 25, 25 };
		break;
	case R_STONE:
		ret = { 878, 858, 25, 25 };
		break;
	case R_FOOD:
		ret = { 0, 0, 0, 0 };
		break;
	case R_GOLD:
		ret = { 878, 858, 25, 25 };
		break;
	default:
		//LOG("Error Resource TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0, 0, 0 };
		break;
	}

	return ret;
}

const char * GetResourceName(const RESOURCE_TYPE type)
{
	const char* ret;

	switch (type)
	{
	case R_WOOD:
		ret = "Sawmill";
		break;
	case R_STONE:
		ret = "Quarry";
		break;
	case R_FOOD:
		ret = "Mill";
		break;
	case R_GOLD:
		ret = "Gold Mine";
		break;
	default:
		//LOG("Error Resource TYPE SDL_Rect NULL (UIManager)");
		ret = "Error Resource";
		break;
	}

	return ret;
}

SDL_Rect GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case B_TURRET:
		ret = { 826, 910, 25, 25 };
		break;

	case B_WOOD_WALL:
		ret = { 774, 936, 25, 25 };
		break;

	case B_STONE_WALL:
		ret = { 774, 910, 25, 25 };
		break;

	case B_TOWNHALL:
		ret = { 800, 936, 25, 25 };
		break;

	default:
		//LOG("Error BUilding TYPE SDL_Rect NULL (UIManager)");
		ret = { 0, 0, 0, 0 };
		break;
	}

	return ret;
}

const char* GetBuildingName(const BUILDING_TYPE type)
{
	char* ret;

	switch (type)
	{
	case B_TURRET:
		ret = "Basic Tower";
		break;
	case B_CANNON:
		ret = "Bombard Tower";
		break;
	case B_WOOD_WALL:
		ret = "Wood Wall";
		break;
	case B_STONE_WALL:
		ret = "Stone Wall";
		break;
	case B_TOWNHALL:
		ret = "Town Hall";
		break;
	case B_UNIVERSITY:
		ret = "University";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Building";
		break;
	}

	return ret;
}

const char * GetBuildingPriceChar(const BUILDING_TYPE type)
{
	char* ret;

	switch (type)
	{
	case B_TURRET:
		ret = "";
		break;
	case B_CANNON:
		ret = "Bombard Tower";
		break;
	case B_WOOD_WALL:
		ret = "Wood Wall";
		break;
	case B_STONE_WALL:
		ret = "Stone Wall";
		break;
	}
	return ret;
}

const char * GetBuildingPrice(const BUILDING_TYPE type)
{
	char* ret;

	switch (type)
	{
	case B_TURRET:
		ret = "Wood: 75 Stone: 25";
		break;
	case B_WOOD_WALL:
		ret = "Stone: 50";
		break;
	case B_STONE_WALL:
		ret = "Stone: 50";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Building Price";
		break;
	}

	return ret;
}

const char * GetTowerName(const TOWER_TYPE type)
{
	char* ret;

	return ret;
}

const char * GetTowerElementName(const TOWER_ELEMENT_TYPE type)
{
	char* ret;

	return ret;
}

const char* GetSideName(const Side type)
{
	char* ret;

	switch (type)
	{
	case S_ALLY:
		ret = "";
		break;
	case S_ENEMY:
		ret = "Enemy";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Side";
		break;
	}

	return ret;
}

int ReturnValueBarHPUnit(const UNIT_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{
	case U_TWOHANDEDSWORDMAN:
		ret = hp * pixels / TWOHANDEDSWORDMAN_HP;
		break;

	case U_CAVALRYARCHER:
		ret = hp * pixels / CAVALRYARCHER_HP;
		break;

	case U_SIEGERAM:
		ret = hp * pixels / SIEGERAM_HP;
		break;

	case U_MANATARMS:
		ret = hp * pixels / MANATARMS_HP;
		break;

	case U_PALADIN:
		ret = hp * pixels / PALADIN_HP;
		break;

		//TODO: ADD UNIT


	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueBarHPBuilding(const BUILDING_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{
	case B_TURRET:
		ret = hp * pixels / TURRET_HP;
		break;
	case B_WOOD_WALL:
		ret = hp * pixels / WOODWALL_HP;
		break;
	case B_STONE_WALL:
		ret = hp * pixels / STONEWALL_HP;
		break;
	case B_TOWNHALL:
		ret = hp * pixels / TOWNHALL_HP;
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueBarHPResource(const RESOURCE_TYPE type, const uint hp, const uint pixels)
{
	if (hp <= 0)
		return 1;

	int ret = 0;

	switch (type)
	{
	case R_WOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_STONE:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_FOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case R_GOLD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionUnit(const UNIT_CLASS u_class)
{
	int ret = 0;

	switch (u_class)
	{
	case C_INFANTRY:
		ret = 47;
		break;

	case C_ARCHER:
		ret = 47;
		break;

	case C_CAVALRY:
		ret = 50;
		break;
			
	case C_SIEGE:
		ret = 50;
		break;

	case C_SUICIDAL:
		ret = 47;
		break;

	default:
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionBuilding(const BUILDING_TYPE type)
{
	int ret = 0;

	switch (type)
	{
	case B_TURRET:
		ret = 180;
		break;
	case B_STONE_WALL:
		ret = 90;
		break;
	case B_TOWNHALL:
		ret = 210;
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

int ReturnValueHeightCorrectionResource(const RESOURCE_TYPE type)
{
	int ret = 0;

	switch (type)
	{
	case R_WOOD:
		ret = 70;
		break;
	case R_STONE:
		ret = 70;
		break;
	case R_FOOD:
		ret = 70;
		break;
	case R_GOLD:
		ret = 70;
		break;
	}

	return ret;
}

const uint GetBuildingPercentage(const Entity * build)
{
	Building* building = (Building*)build;

	double actual_build_time = building->GetBuildTime();

	double time_build = GetBuildTotalTime(building->GetBuildingType());

	if (actual_build_time > time_build)
		return 100;

	return actual_build_time*100/time_build;
}

const double GetBuildTotalTime(const BUILDING_TYPE type)
{
	double ret;

	switch (type)
	{
	case B_TURRET:
		ret = TURRET_BUILD;
		break;

	case B_STONE_WALL:
		ret = STONEWALL_BUILD;
		break;

	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = 0;
		break;
	}

	return ret;
}

const bool isBuilded(const Entity * build)
{
	Building* building = (Building*)build;

	double actual_build_time = building->GetBuildTime();

	double time_build = GetBuildTotalTime(building->GetBuildingType());

	if (actual_build_time > time_build)
		return true;

	return false;
}

SDL_Rect GetInvestigationIconPositionFromAtlas(const INVESTIGATION_TYPE type)
{
	SDL_Rect ret;


	return ret;
}

const uint GetInvestigationPercentage(const Investigation * type)
{
	uint ret;


	return ret;
}

const char* GetInvestigationName(const INVESTIGATION_TYPE type)
{
	char* ret;

	return ret;
}

const char * GetInvestigationPriceChar(const INVESTIGATION_TYPE type)
{
	char* ret;

	return ret;
}