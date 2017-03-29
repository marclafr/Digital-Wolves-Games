#include "UIGetEntitiesInfo.h"

SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case TWOHANDEDSWORDMAN:
		ret = { 774, 962, 25, 25 };
		break;

	case CAVALRYARCHER:
		ret = { 800, 962, 25, 25 };
		break;

	case SIEGERAM:
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
	case TWOHANDEDSWORDMAN:
		ret = "Two Handed Sword Man";
		break;

	case CAVALRYARCHER:
		ret = "Cavalry Archer";
		break;

	case SIEGERAM:
		ret = "Siege Ram";
		break;

	default:
		//LOG("Error UNIT TYPE NAME NULL (UIManager)");
		ret = "Error";
		break;
	}

	return ret;
}

SDL_Rect GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type)
{
	SDL_Rect ret;

	switch (type)
	{
	case TURRET:
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
	case TURRET:
		ret = "Turret";
		break;

	default:
		//LOG("Error BUILDING TYPE NAME NULL (UIManager)");
		ret = "Error";
		break;
	}

	return ret;
}