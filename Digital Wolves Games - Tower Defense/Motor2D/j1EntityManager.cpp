#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1UIManager.h"
#include "UIHUDPanelInfo.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "Camera.h"
#include "j1Map.h"
#include "j1Audio.h"

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

Entity * j1EntityManager::CreateBuilding(BUILDING_TYPE b_type, fPoint pos, Side side)
{
	Entity* new_entity = (Entity*) new Building(b_type, pos, side);
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

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos);
	entity_array.push_back(new_entity);
	App->uimanager->SetResource((Resources*)new_entity);
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

void j1EntityManager::SetOneSelection(Entity * entity, std::vector<Entity*>& selection)
{
	selection.clear();
	selection.push_back(entity);
	entity->SetEntityStatus(ST_SELECTED);
	App->uimanager->CreatePanelInfo(selection);
}

void j1EntityManager::UnselectEverything()
{
	for (int i = 0; i < entity_array.size(); i++)
		entity_array[i]->SetEntityStatus(ST_NON_SELECTED);

	App->uimanager->DeleteSelectionPanelInfo();
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
		entity_array[i]->Update();
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
			if (entity_array[i]->GetX() <= position.x + range + building->GetWidth() && entity_array[i]->GetX() >= position.x - range - building->GetWidth() &&
				entity_array[i]->GetY() <= position.y + range + building->GetHeight() && entity_array[i]->GetY() >= position.y - range - building->GetHeight() &&
				side != entity_array[i]->GetSide() && entity_array[i]->GetHp() > 0)
				return entity_array[i];
		}
		if (entity_array[i]->GetEntityType() == E_UNIT)
			if (entity_array[i]->GetX() <= position.x + range && entity_array[i]->GetX() >= position.x - range &&
				entity_array[i]->GetY() <= position.y + range && entity_array[i]->GetY() >= position.y - range &&
				side != entity_array[i]->GetSide() && entity_array[i]->GetHp() > 0)
				return entity_array[i];
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

std::vector<Entity*> j1EntityManager::GetEntityVector()
{
	return entity_array;
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
	fx_construction = App->audio->LoadFx("audio/fx/Construction01.wav");
}