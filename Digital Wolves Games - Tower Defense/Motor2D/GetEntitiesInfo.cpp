#define TWOHANDEDSWORDMAN_HP 170
#define CAVALRYARCHER_HP 50
#define SIEGERAM_HP 270

#define TURRET_HP 250
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
		ret = { 800, 962, 25, 25 };
		break;

	case U_SIEGERAM:
		ret = { 748, 962, 25, 25 };
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
	case U_TWOHANDEDSWORDMAN:
		ret = "Two Handed Sword Man";
		break;

	case U_CAVALRYARCHER:
		ret = "Cavalry Archer";
		break;

	case U_SIEGERAM:
		ret = "Siege Ram";
		break;

	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit";
		break;
	}

	return ret;
}

const char * GetUnitPrice(const UNIT_TYPE type)
{
	char* ret;

	switch (type)
	{
	case U_TWOHANDEDSWORDMAN:
		ret = "Wood:15 Stone:15";
		break;

	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error Unit Price";
		break;
	}

	return ret;
}

SDL_Rect GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case WOOD:
		ret = { 852, 858, 25, 25 };
		break;
	case STONE:
		ret = { 878, 858, 25, 25 };
		break;
	case FOOD:
		ret = { 0, 0, 0, 0 };
		break;
	case GOLD:
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
	case WOOD:
		ret = "Sawmill";
		break;
	case STONE:
		ret = "Quarry";
		break;
	case FOOD:
		ret = "Mill";
		break;
	case GOLD:
		ret = "Quarry";
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
		ret = { 878, 910, 25, 25 };
		break;

	case B_STONE_WALL:
		ret = { 774, 910, 25, 25 };
		break;

	case B_TOWNHALL:
		ret = { 852, 884, 25, 25 };
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
	case B_STONE_WALL:
		ret = "Stone Wall";
		break;
	case B_TOWNHALL:
		ret = "Town Hall";
		break;
	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Building";
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
	case WOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case STONE:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case FOOD:
		ret = hp * pixels / RESOURCES_HP;
		break;
	case GOLD:
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
	case WOOD:
		ret = 70;
		break;
	case STONE:
		ret = 70;
		break;
	case FOOD:
		ret = 70;
		break;
	case GOLD:
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
