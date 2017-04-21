#ifndef _j1Investigations_
#define _j1Investigations_

#include "j1Module.h"
#include "j1Timer.h"
#include <vector>

#define COST_INCREASE_BY_LVL 100

enum LEVEL
{
	L_LOCKED,
	L_UNLOCKED,
	LVL1,
	LVL2,
	LVL3,
	LVL4,
	LVL5,
	LVL6
	//look for: "ADD MORE JUST IN CASE" in case you add more levels and add more cases to upgrade them.
};

enum INVESTIGATION_TYPE
{
//RESOURCES
	INV_FOOD,
	INV_WOOD,
	INV_GOLD,
	INV_STONE,
//TROPS
	INV_CAVALRY_UNLOCK,
	INV_CAVALRY_ATTACK,
	INV_CAVALRY_DEFENSE,
	INV_ARCHERS_UNLOCK,
	INV_ARCHERS_ATTACK,
	INV_ARCHERS_DEFENSE,
	INV_INFANTRY_UNLOCK,
	INV_INFANTRY_ATTACK,
	INV_INFANTRY_DEFENSE,
//TOWERS
	INV_ELEMENTAL,
	INV_FIRE_TOWER,
	INV_ICE_TOWER,
	INV_AIR_TOWER
};

class Investigation
{
public:
	Investigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time) : investigation_type(investigation), has_levels(has_lvls), cost(cost), time_to_upgrade(time){}
	~Investigation(){}

public:
	INVESTIGATION_TYPE investigation_type;
	bool has_levels = true;
	LEVEL invest_state = L_LOCKED;
	bool investigation_on_course = false;
	float time_to_upgrade;
	j1Timer upgrade_timer;
	uint cost = 0;
};

class j1Investigations : public j1Module
{
public:

	j1Investigations();
	~j1Investigations();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Investigation* CreateInvestigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time_to_upgrade);
	bool DeleteInvestigation(Investigation* ptr);

	bool CanInvestigate(Investigation* investigation);
	bool DoInvestigationUpgrade(Investigation* investigation);

private:
	std::vector<Investigation*> investigations;
	bool UpgradeInvestigation(Investigation* investigation);

};
#endif //_j1Investigations_