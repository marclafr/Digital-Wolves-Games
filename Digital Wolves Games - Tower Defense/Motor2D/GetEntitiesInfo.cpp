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

SDL_Rect GetResourceIconPositionFromAtlas(const RESOURCE_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case WOOD:
		ret = { 0, 0, 0, 0 };
		break;
	case STONE:
		ret = { 0, 0, 0, 0 };
		break;
	case FOOD:
		ret = { 0, 0, 0, 0 };
		break;
	case GOLD:
		ret = { 0, 0, 0, 0 };
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
		ret = "Turret";
		break;

	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error Building";
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
		ret = "Ally";
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

	double time_build = building->GetBuildingType();

	if (actual_build_time > time_build)
		return true;

	return false;
}

SDL_Rect GetAtlasBarBuilding(const uint percentage)
{
	SDL_Rect test = { 0,0,0,0 };

	return test;
}
