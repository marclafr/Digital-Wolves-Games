#include "p2Log.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "Buildings.h"
#include "Towers.h"
#include "Units.h"
#include "Resources.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(): wood(STARTING_WOOD), food (STARTING_FOOD), gold (STARTING_GOLD), stone (STARTING_STONE)
{
	resource_food = (Resources*)App->entity_manager->CreateResource(R_FOOD, fPoint(1392, 858));
	resource_wood = (Resources*)App->entity_manager->CreateResource(R_WOOD, fPoint(1240, 979));
	resource_gold = (Resources*)App->entity_manager->CreateResource(R_GOLD, fPoint(1588, 952));
	resource_stone = (Resources*)App->entity_manager->CreateResource(R_STONE, fPoint(1419, 1031));	
}

void ResourceManager::SetWood(Resources * wood)
{
	resource_wood = wood;
}

void ResourceManager::SetGold(Resources * gold)
{
	resource_gold = gold;
}

void ResourceManager::SetStone(Resources * stone)
{
	resource_stone = stone;
}

void ResourceManager::SetFood(Resources * food)
{
	resource_food = food;
}

void ResourceManager::AddWood(int add)
{
	wood += add;
}

void ResourceManager::AddStone(int add)
{
	stone += add;
}

void ResourceManager::AddGold(int add)
{
	gold += add;
}

void ResourceManager::AddFood(int add)
{
	food += add;
}

bool ResourceManager::ReduceWoodCollectTime(float amount) const
{
	return resource_wood->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceFoodCollectTime(float amount) const
{
	return resource_food->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceGoldCollectTime(float amount) const
{
	return resource_gold->ReduceCollectTime(amount);
}

bool ResourceManager::ReduceStoneCollectTime(float amount) const
{
	return resource_stone->ReduceCollectTime(amount);
}

void ResourceManager::IncreaseResourceAmount(RESOURCE_TYPE type, float amount) const
{
	switch (type)
	{
	case R_WOOD:
		resource_wood->IncreaseResourceAmount(amount);
		break;
	case R_STONE:
		resource_stone->IncreaseResourceAmount(amount);
		break;
	case R_GOLD:
		resource_gold->IncreaseResourceAmount(amount);
		break;
	case R_FOOD:
		resource_food->IncreaseResourceAmount(amount);
		break;
	}
}

void ResourceManager::UseResource(RESOURCE_TYPE type, int amount)
{
	switch (type)
	{
	case R_WOOD:
		wood -= amount;
		break;
	case R_STONE:
		stone -= amount;
		break;
	case R_GOLD:
		gold -= amount;
		break;
	case R_FOOD:
		food -= amount;
		break;
	}
}

int ResourceManager::GetWood() const
{
	return wood;
}

int ResourceManager::GetStone() const
{
	return stone;
}

int ResourceManager::GetGold() const
{
	return gold;
}

int ResourceManager::GetFood() const
{
	return food;
}

int ResourceManager::GetResource(RESOURCE_TYPE type)
{
	switch (type)
	{
	case R_FOOD:
		return food;
	case R_WOOD:
		return wood;
	case R_GOLD:
		return gold;
	case R_STONE:
		return stone;
	default:
		LOG("Invalid Resource");
		return 0;
	}
}

bool ResourceManager::CanBuildTower(TOWER_TYPE type)
{
	//TODO fill defines and cases for all towers
	switch (type)
	{
	case T_NO_TYPE:
		return false;
		break;

	case T_BASIC_TOWER:
		return wood > BASIC_TOWER_WOOD_COST && stone > BASIC_TOWER_STONE_COST;
		break;

	case T_FIRE_TOWER:
		return false;
		break;

	case T_ICE_TOWER:
		return false;
		break;

	case T_AIR_TOWER:
		return false;
		break;

	case T_BOMBARD_TOWER:
		return wood > BASIC_TOWER_WOOD_COST && stone > BASIC_TOWER_STONE_COST;
		break;

	case T_BOMBARD_FIRE_TOWER:
		return false;
		break;

	case T_BOMBARD_ICE_TOWER:
		return false;
		break;

	case T_BOMBARD_AIR_TOWER:
		return false;
		break;

	}
	return false;
}

void ResourceManager::BuildTower(TOWER_TYPE type, iPoint pos)
{
	//TODO fill defines and cases for all towers && all creates here. use this funct with a task
	switch (type)
	{
	case T_NO_TYPE:
		break;

	case T_BASIC_TOWER:
		wood -= BASIC_TOWER_WOOD_COST;
		stone -= BASIC_TOWER_STONE_COST;
		break;

	case T_FIRE_TOWER:
		break;

	case T_ICE_TOWER:
		break;

	case T_AIR_TOWER:
		break;

	case T_BOMBARD_TOWER:
		wood -= BASIC_TOWER_WOOD_COST;
		stone -= BASIC_TOWER_STONE_COST;
		break;

	case T_BOMBARD_FIRE_TOWER:
		break;

	case T_BOMBARD_ICE_TOWER:
		break;

	case T_BOMBARD_AIR_TOWER:
		break;
	}
	App->entity_manager->CreateTower(type, fPoint(pos.x, pos.y - 9));
}

bool ResourceManager::CanBuildWall(BUILDING_TYPE type)
{
	//TODO create defines and fill cases for all walls
	switch (type)
	{
	case B_WOOD_WALL:
		return stone >= BASIC_WALL_STONE_COST;
		break;
	case B_STONE_WALL:
		return stone >= BASIC_WALL_STONE_COST;
		break;
	}
	return false;
}

bool ResourceManager::CanBuildAmountOfWalls(int number_of_walls)
{
	return stone >= BASIC_WALL_STONE_COST*number_of_walls;
	return false;
}

void ResourceManager::BuildWall(BUILDING_TYPE type)
{
	//TODO create defines and fill cases for all walls && all creates here. use this funct with a task
	switch (type)
	{
	case B_WOOD_WALL:
		stone -= BASIC_WALL_STONE_COST;
		break;

	case B_STONE_WALL:
		stone -= BASIC_WALL_STONE_COST;
		break;
	}
}

bool ResourceManager::CanTrainSoldier(UNIT_TYPE type)
{
	//TODO  create defines and fill cases for all units
	switch (type)
	{
	case U_NO_UNIT:
		return false;
	case U_VILLAGER:
		return false;
	case U_GOD:
		return false;
	case U_MILITIA:
		return false;
	case U_MANATARMS:
		return false;
	case U_LONGSWORDMAN:
		return false;
	case U_TWOHANDEDSWORDMAN:
		return wood > TWOHANDED_WOOD_COST && stone > TWOHANDED_STONE_COST;
	case U_CHAMPION:
		return wood > TWOHANDED_WOOD_COST && stone > TWOHANDED_STONE_COST;
		return false;
	case U_SPEARMAN:
		return false;
	case U_PIKEMAN:
		return false;
	case U_ARCHER:
		return false;
	case U_ARBALEST:
		return wood > TWOHANDED_WOOD_COST && stone > TWOHANDED_STONE_COST;
		return false;
	case U_CAVALRYARCHER:
		return false;
	case U_HEAVYCAVALRYARCHER:
		return wood > TWOHANDED_WOOD_COST && stone > TWOHANDED_STONE_COST;
		return false;
	case U_KNIGHT:
		return false;
	case U_CAVALIER:
		return false;
	case U_PALADIN:
		return wood > TWOHANDED_WOOD_COST && stone > TWOHANDED_STONE_COST;
		return false;
	case U_SIEGERAM:
		return false;
	}
	return false;
}

void ResourceManager::TrainSoldier(UNIT_TYPE type)
{
	//TODO  create defines and fill cases for all units && all creates here. use this funct with a task
	switch (type)
	{
	case U_NO_UNIT:
		break;
	case U_VILLAGER:
		break;
	case U_GOD:
		break;
	case U_MILITIA:
		break;
	case U_MANATARMS:
		break;
	case U_LONGSWORDMAN:
		break;
	case U_TWOHANDEDSWORDMAN:
		wood -= TWOHANDED_WOOD_COST;
		stone -= TWOHANDED_STONE_COST;
		break;
	case U_CHAMPION:
		wood -= TWOHANDED_WOOD_COST;
		stone -= TWOHANDED_STONE_COST;
		break;
	case U_SPEARMAN:
		break;
	case U_PIKEMAN:
		break;
	case U_ARCHER:
		break;
	case U_ARBALEST:
		wood -= TWOHANDED_WOOD_COST;
		stone -= TWOHANDED_STONE_COST;
		break;
	case U_CAVALRYARCHER:
		break;
	case U_HEAVYCAVALRYARCHER:
		wood -= TWOHANDED_WOOD_COST;
		stone -= TWOHANDED_STONE_COST;
		break;
	case U_KNIGHT:
		break;
	case U_CAVALIER:
		break;
	case U_PALADIN:
		wood -= TWOHANDED_WOOD_COST;
		stone -= TWOHANDED_STONE_COST;
		break;
	case U_SIEGERAM:
		break;
	}
	App->entity_manager->CreateUnit(type, STARTING_POS, S_ALLY);
}