#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1UIManager.h"
#include "UIHUDPanelInfo.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1WaveManager.h"
#include "Resources.h"
#include "j1Score.h"
#include "Camera.h"
#include "j1Map.h"
#include "ResourceManager.h"
#include "j1Audio.h"
#include "IsoPrimitives.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::CleanUp() { // not done

	//if(sprites != nullptr)
	//App->tex->UnLoad(sprites);

	std::vector<Entity*>::iterator item = entity_array.begin();
	for (int i = 0; i < entity_array.size(); i++)
		RELEASE(entity_array[i]);

	entity_array.clear();

	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side)
{

	Entity* new_entity = (Entity*) new Unit(u_type, pos, side, priority);
	entity_array.push_back(new_entity);
	priority++;
	return new_entity;
}

Entity * j1EntityManager::CreateBuilding(BUILDING_TYPE b_type, fPoint pos,bool builded)
{
	Entity* new_entity = (Entity*) new Building(b_type, pos, builded);
	entity_array.push_back(new_entity);
	if (b_type == B_TOWNHALL)
		App->uimanager->SetTownHall((Building*)new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateTower(TOWER_TYPE t_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Tower(t_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos, int amount_collect, int time)
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos, amount_collect,time);
	entity_array.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection)
{
	bool selecting_units = false;
	int entity_x = 0;
	int entity_y = 0;

	selection.clear();

	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetSide() == S_ALLY && entity_array[i]->GetHp() > 0)
		{
			entity_x = entity_array[i]->GetX();
			entity_y = entity_array[i]->GetY();

			if ((entity_x > select_rect.x && entity_x < select_rect.w && entity_y > select_rect.y && entity_y < select_rect.h)
				|| (entity_x < select_rect.x && entity_x > select_rect.w && entity_y < select_rect.y && entity_y > select_rect.h)
				|| (entity_x > select_rect.x && entity_x < select_rect.w && entity_y < select_rect.y && entity_y > select_rect.h)
				|| (entity_x < select_rect.x && entity_x > select_rect.w && entity_y > select_rect.y && entity_y < select_rect.h))
			{
				if (entity_array[i]->GetEntityType() == E_UNIT && selecting_units == false)
				{
					selecting_units = true;

					for (std::vector<Entity*>::iterator it = selection.begin(); it != selection.end(); ++it)
						(*it)->SetEntityStatus(ST_NON_SELECTED);

					selection.clear();
				}

				if (entity_array[i]->GetEntityType() == E_UNIT)
				{
					entity_array[i]->SetEntityStatus(ST_SELECTED);
					selection.push_back(entity_array[i]);
				}

				if ((entity_array[i]->GetEntityType() == E_BUILDING || entity_array[i]->GetEntityType() == E_RESOURCE) && selecting_units == false)
				{					
					entity_array[i]->SetEntityStatus(ST_SELECTED);
					selection.push_back(entity_array[i]);
				}

				if (selection.size() >= 25)
					break;
			}
		}
	}

	App->uimanager->CreatePanelInfo(selection);
}

void j1EntityManager::UnselectEverything()
{
	for (int i = 0; i < App->scene->selection.size(); i++)
		App->scene->selection[i]->SetEntityStatus(ST_NON_SELECTED);

	App->scene->selection.clear();
	App->uimanager->DeleteSelectionPanelInfo();
	App->uimanager->DeletePanelButtons();
}

void j1EntityManager::Select(Entity * select)
{
	App->entity_manager->UnselectEverything();
	select->SetEntityStatus(ST_SELECTED);
	App->scene->selection.push_back(select);
	App->uimanager->CreatePanelInfo(App->scene->selection);
}

Entity * j1EntityManager::LookForEnemies(int range, iPoint pos)
{
	for (std::vector<Entity*>::iterator item = entity_array.begin(); item != entity_array.end(); ++item)
		if ((*item)->GetEntityType() == E_UNIT)
			if ((*item)->GetX() >= (pos.x - range) 
				&& (*item)->GetX() < (pos.x + range) 
				&& (*item)->GetY() >= (pos.y - range) 
				&& (*item)->GetY() < (pos.y + range) 
				&& (*item)->GetHp() > 0 
				&& (*item)->GetSide() == S_ENEMY)
				return *item;
	return nullptr;
}

void j1EntityManager::CheckClick(int mouse_x, int mouse_y)
{
	App->scene->selection.clear();

	iPoint click_point = App->render->ScreenToWorld(mouse_x, mouse_y);

	for (std::vector<Entity*>::iterator item = entity_array.begin(); item != entity_array.end(); ++item)
	{
		SDL_Rect rect = (*item)->GetRect();
		if (click_point.x >= (*item)->GetX() - rect.w / 2 && click_point.x <= (*item)->GetX() + rect.w / 2 && click_point.y >= (*item)->GetY() - (rect.h - 20) && click_point.y <= (*item)->GetY() + 20)
		{
			(*item)->SetEntityStatus(ST_SELECTED);
			App->scene->selection.push_back(*item);
			App->uimanager->CreatePanelInfo(App->scene->selection);
			break;
		}
	}
}

void j1EntityManager::DeleteEntity(Entity* ptr)
{
	switch (ptr->GetEntityType())
	{
		case E_UNIT:
			DeleteUnit((Unit*)ptr);
			break;
		case E_BUILDING:
			DeleteBuilding((Building*)ptr);
			break;
		case E_RESOURCE:
			DeleteResource((Resources*)ptr);
			break;
	}
}

void j1EntityManager::DeleteUnit(Unit* ptr)
{
	for (std::vector<Entity*>::iterator i = entity_array.begin(); i != entity_array.end();)
	{
		if (*i == ptr)
		{
			entity_array.erase(i);
			delete ptr;
			return;
		}
		else
			i++;
	}
}

void j1EntityManager::DeleteBuilding(Building* ptr)
{
	for (std::vector<Entity*>::iterator i = entity_array.begin(); i != entity_array.end();)
	{
		if (*i == ptr)
		{
			entity_array.erase(i);
			delete ptr;
			return;
		}
		else
			i++;
	}
}

void j1EntityManager::DeleteResource(Resources* ptr)
{
	for (std::vector<Entity*>::iterator i = entity_array.begin(); i != entity_array.end();)
	{
		if (*i == ptr)
		{
			entity_array.erase(i);
			delete ptr;
			return;
		}
		else
			i++;
	}
}


bool j1EntityManager::Start()
{
	LoadAllFx();

	return true;
}

bool j1EntityManager::Update(float dt)
{
	int size = entity_array.size(); //error de compilador o algo, si es posa entity_array.size() dins el for canvia els valors dels rects de textures
	for (int i = 0; i < size; i++)
	{
		entity_array[i]->Update(dt);
	}
	
	//a través de la lista donde 
	//tenemos los enemigos ejecutamos los siguientes pasos

	//funcion de movimiento de cada enemigo

	//Draw de los enemigos

	//si han muerto y han acabado su animacion de muerte hacer delete
	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (int i = 0; i < entity_array.size(); i++)
		if (entity_array[i]->ToDelete() == true)
			DeleteEntity(entity_array[i]);
	return true;
}

Entity * j1EntityManager::CheckForCombat(iPoint position, int range, Side side)
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetEntityType() == E_BUILDING)
		{
			Building* building = (Building*)entity_array[i];
			//TODO: USE ISO RECT
			//if (entity_array[i]->GetX() <= position.x + range + building->GetBuildRectangle().GetWidth() && entity_array[i]->GetX() >= position.x - range - building->GetBuildRectangle().GetWidth() &&
			//	entity_array[i]->GetY() <= position.y + range + building->GetBuildRectangle().GetHeight() && entity_array[i]->GetY() >= position.y - range - building->GetBuildRectangle().GetHeight() &&
			if (building->GetBuildRectangle().Inside({ position.x, position.y }) &&
				side != entity_array[i]->GetSide() && entity_array[i]->GetHp() > 0)
				return entity_array[i];
		}
		if (entity_array[i]->GetEntityType() == E_UNIT)
		{
			Unit* unit = (Unit*)entity_array[i];
			Circle  unit_view(position, range);
			if (unit->GetUnitCircle().Overlap(&unit_view) == true &&	side != entity_array[i]->GetSide() && entity_array[i]->GetHp() > 0)
				return entity_array[i];
		}
	}
	return nullptr;
}

Entity* j1EntityManager::CheckForObjective(iPoint position, int vision_range, Side side)
{
	Entity* ret = nullptr;
	for (int i = 0; i < entity_array.size(); i++)
		if (entity_array[i]->GetX() <= position.x + vision_range && entity_array[i]->GetX() >= position.x - vision_range &&
			entity_array[i]->GetY() <= position.y + vision_range && entity_array[i]->GetY() >= position.y - vision_range &&
			side != entity_array[i]->GetSide() && entity_array[i]->GetHp() > 0 && entity_array[i]->GetSide() != S_NEUTRAL
			&& entity_array[i]->GetEntityType() == E_UNIT)
			if(ret == nullptr)
				ret = entity_array[i];
			else
				if (position.DistanceTo(iPoint(ret->GetX(), ret->GetY())) > position.DistanceTo(iPoint(entity_array[i]->GetX(),entity_array[i]->GetY())))
					ret = entity_array[i];			
	return ret;
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	for (int i = 0; i < entity_array.size(); i++)
		if (entity_array[i]->GetEntityType() == E_UNIT)
			if (entity_array[i] != unit)
				if (tile == App->map->WorldToMap(entity_array[i]->GetX(), entity_array[i]->GetY()))
					return true;
	return false;
}

void j1EntityManager::LoadAllFx()
{
	//UNITS
	fx_twohanded_die01 = App->audio->LoadFx("audio/fx/Male_Death01.wav");
	fx_twohanded_die02 = App->audio->LoadFx("audio/fx/Male_Death02.wav");
	fx_twohanded_die03 = App->audio->LoadFx("audio/fx/Male_Death03.wav");
	fx_twohanded_die04 = App->audio->LoadFx("audio/fx/Male_Death04.wav");
	fx_twohanded_die05 = App->audio->LoadFx("audio/fx/Male_Death05.wav");
	fx_attack01 = App->audio->LoadFx("audio/fx/Swordfight01.wav");
	fx_attack02 = App->audio->LoadFx("audio/fx/Swordfight02.wav");
	fx_attack03 = App->audio->LoadFx("audio/fx/Swordfight03.wav");

	//BUILDINGS
	fx_building_destroyed = App->audio->LoadFx("audio/fx/Building_destroyed01.wav");
	fx_arrow = App->audio->LoadFx("audio/fx/Arrow01.wav");
	fx_cannon = App->audio->LoadFx("audio/fx/Cannon01.wav");
	fx_construction = App->audio->LoadFx("audio/fx/Construction01.wav");
	fx_university = App->audio->LoadFx("audio/fx/University.wav");
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	App->entity_manager->CleanUp();
	
	pugi::xml_node Buildingsload = data.child("buildings").first_child();
	pugi::xml_node Unitsload = data.child("units").first_child();
	pugi::xml_node Turretsload = data.child("turrets").first_child();
	pugi::xml_node Resourcesload = data.child("resources").first_child();
	
	while (Buildingsload != NULL)
	{
		LoadBuilding(Buildingsload);
		Buildingsload = Buildingsload.next_sibling();
	}

	while (Unitsload != NULL)
	{
		LoadUnit(Unitsload);
		Unitsload = Unitsload.next_sibling();
	}

	while (Turretsload != NULL)
	{
		LoadTurret(Turretsload);
		Turretsload = Turretsload.next_sibling();
	}

	while (Resourcesload != NULL)
	{
		LoadResource(Resourcesload);
		Resourcesload = Resourcesload.next_sibling();
	}


	pugi::xml_node AmountOfResources = data.child("resources_amount").first_child();

	App->scene->resources->LoadResourcesAmount(AmountOfResources);

	pugi::xml_node Score = data.child("score");

	App->score->Reset();
	App->score->SetScore(Score.attribute("points").as_int());
	App->score->SetEnemiesKilleds(Score.attribute("enemies_killeds").as_int());
	App->score->SetTimePassed(Score.attribute("time_passed").as_int());
	App->wave_manager->ResetWave();
	App->wave_manager->SetWaveNum(Score.attribute("wave_num").as_int());
	return true;
}

void j1EntityManager::LoadResource(pugi::xml_node& data)
{
	pugi::xml_node actualresource = data;
	fPoint pos(actualresource.attribute("posx").as_int(), actualresource.attribute("posy").as_int());
	Resources* actualres = (Resources*)App->entity_manager->CreateResource(RESOURCE_TYPE(actualresource.attribute("resource_type").as_int()), pos, actualresource.attribute("amount_collected").as_int(), actualresource.attribute("collect_time").as_int());
	switch (actualres->GetResourceType())
	{
	case R_WOOD:
		App->scene->resources->SetWood(actualres);
		break;
	case R_FOOD:
		App->scene->resources->SetFood(actualres);
		break;
	case R_GOLD:
		App->scene->resources->SetGold(actualres);
		break;
	case R_STONE:
		App->scene->resources->SetStone(actualres);
		break;
	}
}

void j1EntityManager::LoadBuilding(pugi::xml_node& data)
{
	pugi::xml_node Actualbuilding = data;
	fPoint pos(Actualbuilding.attribute("posx").as_int(), Actualbuilding.attribute("posy").as_int());
	Building* actualbuild = (Building*)App->entity_manager->CreateBuilding(BUILDING_TYPE(Actualbuilding.attribute("building_type").as_int()), pos, true);
	actualbuild->SetHp(Actualbuilding.attribute("hp").as_int());
	actualbuild->BuildingComplete();
}

void j1EntityManager::LoadUnit(pugi::xml_node& data)
{
	pugi::xml_node Actualunit = data;
		fPoint pos(Actualunit.attribute("posx").as_int(), Actualunit.attribute("posy").as_int());
		Unit* actualunit = (Unit*)App->entity_manager->CreateUnit(UNIT_TYPE(Actualunit.attribute("unit_type").as_int()), pos, Side(Actualunit.attribute("side").as_int()));
		actualunit->SetHp(Actualunit.attribute("hp").as_int());
}

void j1EntityManager::LoadTurret(pugi::xml_node& data)
{
	pugi::xml_node Actualturret = data;
	fPoint pos(Actualturret.attribute("posx").as_int(), Actualturret.attribute("posy").as_int());
	Tower* actualturret = (Tower*)App->entity_manager->CreateTower(TOWER_TYPE(Actualturret.attribute("tower_type").as_int()), pos);
	actualturret->SetHp(Actualturret.attribute("hp").as_int());
	actualturret->BuildingComplete();
}

bool j1EntityManager::Save(pugi::xml_node &data) const
{
	pugi::xml_node Buildings = data.append_child("buildings");
	pugi::xml_node Units = data.append_child("units");
	pugi::xml_node Turrets = data.append_child("turrets");
	pugi::xml_node Resourcess = data.append_child("resources");

	for (int k = 0; k <entity_array.size(); k++) {
		if (entity_array[k]->GetEntityType() == E_BUILDING)
		{
			Building* buildingptr = (Building*)entity_array[k];
			if (buildingptr->GetBuildingType() == B_TURRET || buildingptr->GetBuildingType() == B_CANNON)
			{
				Tower* towerptr = (Tower*)buildingptr;
				towerptr->SaveTurret(Turrets);
			}
			else buildingptr->SaveBuilding(Buildings);
		}
		else if (entity_array[k]->GetEntityType() == E_UNIT)
		{
			Unit* unit = (Unit*)entity_array[k];
			if(unit->GetSide() == S_ALLY)	unit->SaveUnit(Units);
		}
		else if (entity_array[k]->GetEntityType() == E_RESOURCE)
		{
			Resources* rest = (Resources*)entity_array[k];
			rest->SaveResource(Resourcess);
		}
	}


	pugi::xml_node AmountOfResources = data.append_child("resources_amount");

	App->scene->resources->SaveResourcesAmount(AmountOfResources);

	pugi::xml_node Score = data.append_child("score");

	Score.append_attribute("points") = App->score->GetScore();
	Score.append_attribute("enemies_killeds") = App->score->GetEnemiesKilled();
	Score.append_attribute("time_passed") = App->score->GetTime();
	Score.append_attribute("wave_num") = App->wave_manager->GetWaveNum();
	return true;
}
