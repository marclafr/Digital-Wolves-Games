#include "j1Investigations.h"

j1Investigations::j1Investigations()
{
	name.assign("investigations");
}

j1Investigations::~j1Investigations()
{
}

bool j1Investigations::Start()
{
	bool ret = true;

	//RESOURCES
	CreateInvestigation("food", true, 5);
	CreateInvestigation("wood", true, 5.0);
	CreateInvestigation("gold", true, 5.0);
	CreateInvestigation("stone", true, 5.0);
	//TROPS
	CreateInvestigation("cavalry unlock", false, 5.0);
	CreateInvestigation("cavalry attack", false, 5.0);
	CreateInvestigation("cavalry defense", false, 5.0);
	CreateInvestigation("archers unlock", false, 5.0);
	CreateInvestigation("archers attack", false, 5.0);
	CreateInvestigation("archers defense", false, 5.0);
	CreateInvestigation("infantry unlock", false, 5.0);
	CreateInvestigation("infantry attack", false, 5.0);
	CreateInvestigation("infantry defense", false, 5.0);	//TODO: Should attack/defense have levels? in that case swap to true
	//TOWERS
	CreateInvestigation("elemental", false, 5.0);
	CreateInvestigation("fire tower", false, 5.0);
	CreateInvestigation("ice tower", false, 5.0);
	CreateInvestigation("air tower", false, 5.0);
	//--

	return ret;
}

bool j1Investigations::Update(float dt)
{
	bool ret = true;

	//IMPORTANT: When activating the investigation upgrade-> USE THIS:	investigations[i]->upgrade_timer.Start();

	for (int i = 0; i < investigations.size(); i++)
	{
		if (investigations[i]->investigation_on_course == true)
			UpgradeInvestigation(investigations[i]);
	}
	return ret;
}

bool j1Investigations::CleanUp()
{
	bool ret = true;

	for (int i = 0; i < investigations.size(); i++)
		DeleteInvestigation(investigations[i]);
	
	return ret;
}

Investigation* j1Investigations::CreateInvestigation(const char* investigation_name, bool has_lvls, float time_to_upgrade)
{
	Investigation* new_inv = new Investigation(investigation_name, has_lvls, time_to_upgrade);
	investigations.push_back(new_inv);

	return new_inv;
}

bool j1Investigations::DeleteInvestigation(Investigation* ptr)
{
	for (std::vector<Investigation*>::iterator it = investigations.begin(); it != investigations.end();)
	{
		if (*it == ptr)
		{
			investigations.erase(it);
			delete ptr;
			return true;
		}
		else
			it++;
	}
	return false;
}

bool j1Investigations::UpgradeInvestigation(Investigation* investigation)
{

	if (investigation->upgrade_timer.ReadSec() >= investigation->time_to_upgrade)
	{
		investigation->investigation_on_course = false;

		if (investigation->has_levels == false)
		{
			investigation->invest_state = UNLOCKED;
			return true;
		}

		switch (investigation->invest_state)
		{
		case LOCKED:
			investigation->invest_state = LVL1;
			break;
		case LVL1:
			investigation->invest_state = LVL2;
			break;
		case LVL2:
			investigation->invest_state = LVL3;
			break;
		case LVL3:
			investigation->invest_state = LVL4;
			break;
		case LVL4:
			investigation->invest_state = LVL5;
			break;
		case LVL5:
			investigation->invest_state = LVL6;
			break;
			//ADD MORE JUST IN CASE
		default:
			break;
		}
		return true;
	}
	return false;
}
