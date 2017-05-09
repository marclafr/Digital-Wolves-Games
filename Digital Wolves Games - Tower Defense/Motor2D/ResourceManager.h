#ifndef _RESOURCE_MANAGER
#define _RESOURCE_MANAGER

#define STARTING_WOOD 100
#define STARTING_FOOD 100
#define STARTING_GOLD 100
#define STARTING_STONE 100

#define BASIC_TOWER_WOOD_COST 75
#define BASIC_TOWER_STONE_COST 25
#define TWOHANDED_WOOD_COST 15
#define TWOHANDED_STONE_COST 15
#define BASIC_WALL_STONE_COST 50

#define STARTING_POS fPoint(-70, 420)

class Resources;
enum BUILDING_TYPE;
enum TOWER_TYPE;
enum RESOURCE_TYPE;
enum UNIT_TYPE;

class ResourceManager
{
private:
	Resources* resource_food = nullptr;
	Resources* resource_wood = nullptr;
	Resources* resource_gold = nullptr;
	Resources* resource_stone = nullptr;

	int wood = STARTING_WOOD;
	int food = STARTING_FOOD;
	int gold = STARTING_GOLD;
	int stone = STARTING_STONE;

public:
	ResourceManager();

	void SetWood(Resources* wood);
	void SetGold(Resources* gold);
	void SetStone(Resources* stone);
	void SetFood(Resources* food);

	void AddWood(int add);
	void AddStone(int add);
	void AddGold(int add);
	void AddFood(int add);

	bool ReduceWoodCollectTime(float amount) const;
	bool ReduceFoodCollectTime(float amount) const;
	bool ReduceGoldCollectTime(float amount) const;
	bool ReduceStoneCollectTime(float amount) const;

	void IncreaseResourceAmount(RESOURCE_TYPE type, float amount) const;

	void UseResource(RESOURCE_TYPE type, int amount);

	int GetWood() const;
	int GetStone() const;
	int GetGold() const;
	int GetFood() const;

	int GetResource(RESOURCE_TYPE type);

	//Entity creation
	bool CanBuildTower(TOWER_TYPE type);
	void BuildTower(TOWER_TYPE type, iPoint pos);
	bool CanBuildWall(BUILDING_TYPE type);
	bool CanBuildAmountOfWalls(int number_of_walls);
	void BuildWall(BUILDING_TYPE type);
	bool CanTrainSoldier(UNIT_TYPE type);
	void TrainSoldier(UNIT_TYPE type);
};


#endif
