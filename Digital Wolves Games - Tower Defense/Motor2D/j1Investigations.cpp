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
	CreateInvestigation("food", true, 300, 1.0);
	CreateInvestigation("wood", true, 300, 1.0);
	CreateInvestigation("gold", true, 300, 1.0);
	CreateInvestigation("stone", true, 300, 1.0);
	//TROPS
	CreateInvestigation("cavalry unlock", false, 150, 15.0);
	CreateInvestigation("cavalry attack", false, 300, 30.0);
	CreateInvestigation("cavalry defense", false, 300, 25.0);
	CreateInvestigation("archers unlock", false, 150, 15.0);
	CreateInvestigation("archers attack", false, 300, 30.0);
	CreateInvestigation("archers defense", false, 300, 25.0);
	CreateInvestigation("infantry unlock", false, 100, 15.0);
	CreateInvestigation("infantry attack", false, 300, 30.0);
	CreateInvestigation("infantry defense", false, 300, 25.0);
	//TOWERS
	CreateInvestigation("elemental", false, 100, 10.0);
	CreateInvestigation("fire tower", false, 200, 10.0);
	CreateInvestigation("ice tower", false, 250, 10.0);
	CreateInvestigation("air tower", false, 300, 10.0);
	//--

	return ret;
}

bool j1Investigations::Update(float dt)
{
	bool ret = true;

	//IMPORTANT: When activating the investigation upgrade-> USE THIS:	investigations[i]->upgrade_timer.Start();
	//TEST
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
	{
		if (this->CanInvestigate(investigations[0]))
		{
			Resources* gold = App->scene->GetResource(GOLD);
			gold->UseResource(investigations[0]->cost);
			investigations[0]->cost += 100;
			investigations[0]->investigation_on_course = true;
			investigations[0]->upgrade_timer.Start();
		}
	}
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

Investigation* j1Investigations::CreateInvestigation(const char* investigation_name, bool has_lvls, uint cost, float time_to_upgrade)
{
	Investigation* new_inv = new Investigation(investigation_name, has_lvls, cost, time_to_upgrade);
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
