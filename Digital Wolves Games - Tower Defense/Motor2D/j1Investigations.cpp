#include "j1Investigations.h"
#include "j1App.h"
#include "Resources.h"
#include "j1Scene.h"

//TEST
#include "j1Input.h"
#include "SDL/include/SDL.h"
//--

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
	CreateInvestigation(INV_FOOD, true, 300, 1.0);
	CreateInvestigation(INV_WOOD, true, 300, 1.0);
	CreateInvestigation(INV_GOLD, true, 300, 1.0);
	CreateInvestigation(INV_STONE, true, 300, 1.0);
	//TROPS
	CreateInvestigation(INV_CAVALRY_UNLOCK, false, 150, 15.0);
	CreateInvestigation(INV_CAVALRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_CAVALRY_DEFENSE, false, 300, 25.0);
	CreateInvestigation(INV_ARCHERS_UNLOCK, false, 150, 15.0);
	CreateInvestigation(INV_ARCHERS_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_ARCHERS_DEFENSE, false, 300, 25.0);
	CreateInvestigation(INV_INFANTRY_UNLOCK, false, 100, 15.0);
	CreateInvestigation(INV_INFANTRY_ATTACK, false, 300, 30.0);
	CreateInvestigation(INV_INFANTRY_DEFENSE, false, 300, 25.0);
	//TOWERS
	CreateInvestigation(INV_ELEMENTAL, false, 100, 10.0);
	CreateInvestigation(INV_FIRE_TOWER, false, 200, 10.0);
	CreateInvestigation(INV_ICE_TOWER, false, 250, 10.0);
	CreateInvestigation(INV_AIR_TOWER, false, 300, 10.0);
	//--

	return ret;
}

bool j1Investigations::Update(float dt)
{
	bool ret = true;

	//IMPORTANT: When activating the investigation upgrade-> USE THIS:	investigations[i]->upgrade_timer.Start();
	//TEST
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		DoInvestigationUpgrade(investigations[0]);
	//--

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

Investigation* j1Investigations::CreateInvestigation(INVESTIGATION_TYPE investigation, bool has_lvls, uint cost, float time_to_upgrade)
{
	Investigation* new_inv = new Investigation(investigation, has_lvls, cost, time_to_upgrade);
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

bool j1Investigations::CanInvestigate(Investigation* investigation)
{
	Resources* current_gold = App->scene->GetResource(GOLD);
	if (current_gold->CanUseResource(investigation->cost))	//TODO: Multiply by level upgrade????
		return true;

	return false;
}

bool j1Investigations::DoInvestigationUpgrade(Investigation* investigation)
{
	if (this->CanInvestigate(investigation) && investigation->investigation_on_course == false)
	{
		Resources* gold = App->scene->GetResource(GOLD);
		gold->UseResource(investigation->cost);

		if (investigation->has_levels == true)
			investigation->cost += COST_INCREASE_BY_LVL;

		investigation->investigation_on_course = true;
		investigation->upgrade_timer.Start();
		return true;
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
			investigation->invest_state = L_UNLOCKED;
			return true;
		}

		switch (investigation->invest_state)
		{
		case L_LOCKED:
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
