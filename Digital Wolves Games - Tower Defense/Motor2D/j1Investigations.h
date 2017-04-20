#ifndef _j1Investigations_
#define _j1Investigations_

#include "j1Module.h"
#include "j1Timer.h"
#include <vector>

enum LEVEL
{
	LOCKED,
	UNLOCKED,
	LVL1,
	LVL2,
	LVL3,
	LVL4,
	LVL5,
	LVL6
	//look for: "ADD MORE JUST IN CASE" in case you add more levels and add more cases to upgrade them.
};

class Investigation
{
public:
	Investigation(const char* name, bool has_lvls, uint cost, float time) : inv_name(name), has_levels(has_lvls), cost(cost), time_to_upgrade(time){}
	~Investigation(){}

public:
	std::string inv_name;
	bool has_levels = true;
	LEVEL invest_state = LOCKED;
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

	Investigation* CreateInvestigation(const char* investigation_name, bool has_lvls, uint cost, float time_to_upgrade);
	bool DeleteInvestigation(Investigation* ptr);

	bool CanInvestigate(Investigation* investigation);

private:
	std::vector<Investigation*> investigations;
	bool UpgradeInvestigation(Investigation* investigation);

};
#endif //_j1Investigations_