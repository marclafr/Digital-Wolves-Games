#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1UIManager.h"
#include "UIHUDPanelInfo.h"
#include "j1Scene.h"
#include "j1Map.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() { return true; }

bool j1EntityManager::CleanUp() { // not done

	//if(sprites != nullptr)
	//App->tex->UnLoad(sprites);

	std::vector<Entity*>::iterator item = entity_array.begin();
	for (int i = 0; i < entity_array.size(); i++)
		RELEASE(entity_array[i]);

	entity_array.clear();

	//Borrar enemigos
	//for(uint i = 0;             ; ++i)
		//if(unit_list[i])
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
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect)
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		//TODO: Unselect for only ally selection if (entity_array[i]->GetSide() == ALLY)
		{
			int unit_x = entity_array[i]->GetX();
			int unit_y = entity_array[i]->GetY();
			if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				entity_array[i]->SetEntityStatus(E_SELECTED);
				App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				entity_array[i]->SetEntityStatus(E_SELECTED);
				App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				entity_array[i]->SetEntityStatus(E_SELECTED);
				App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				entity_array[i]->SetEntityStatus(E_SELECTED);
				App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
		}
	}
	if(!App->scene->panel_info->isSelectionTempEmpty())
		App->scene->panel_info->DefineSelection();
}

void j1EntityManager::UnselectEverything()
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		entity_array[i]->SetEntityStatus(E_NON_SELECTED);
	}
	if (!App->scene->panel_info->isSelectionEmpty())
		App->scene->panel_info->DeleteSelection();
}

void j1EntityManager::DeleteEntity(Entity* ptr)
{
	switch (ptr->GetEntityType())
	{
		case UNIT:
			DeleteUnit((Unit*)ptr);
			break;

		case BUILDING:
			DeleteBuilding(ptr);
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
		}
		else
			i++;
	}
}

void j1EntityManager::DeleteBuilding(Entity* ptr)
{
}


bool j1EntityManager::Update(float dt)
{
	int size = entity_array.size(); //error de compilador o algo, si es posa entity_array.size() dins el for canvia els valors dels rects de textures
	for (int i = 0; i < size; i++)
	{
		entity_array[i]->Update();
	}



	//a trav�s de la lista donde 
	//tenemos los enemigos ejecutamos los siguientes pasos

	//funcion de movimiento de cada enemigo

	//Draw de los enemigos

	//si han muerto y han acabado su animacion de muerte hacer delete
	return true;
}

bool j1EntityManager::PostUpdate()
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->ToDelete() == true)
		{
			DeleteEntity(entity_array[i]);
		}
	}
	return true;
}

//TODO: Why this function?
void j1EntityManager::GetUnitsPath(iPoint destination) 
{
	std::vector<Entity*>::iterator prove = entity_array.begin();

	while (prove != entity_array.end())
		//if (prove._Ptr->_Myval->GetEntityStatus() == E_SELECTED)
			prove++;

}

Entity * j1EntityManager::CheckForCombat(iPoint position, int range, Side side)
{
	for  (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetX() <= position.x + range && entity_array[i]->GetX() >= position.x - range &&
			entity_array[i]->GetY() <= position.y + range && entity_array[i]->GetY() >= position.y - range &&
			side != entity_array[i]->GetSide() && entity_array[i]->GetHp() >= 0)
			return entity_array[i];
	}
	return nullptr;
}

iPoint j1EntityManager::CheckForObjective(iPoint position, int vision_range, Side side)
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetX() <= position.x + vision_range && entity_array[i]->GetX() >= position.x - vision_range &&
			entity_array[i]->GetY() <= position.y + vision_range && entity_array[i]->GetY() >= position.y - vision_range &&
			side != entity_array[i]->GetSide() && entity_array[i]->GetHp() >= 0)
			return iPoint(entity_array[i]->GetX(), entity_array[i]->GetY());
	}
	return iPoint(-1, -1);
}

std::vector<Entity*> j1EntityManager::GetEntityVector()
{
	return entity_array;
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetEntityType() == UNIT)
		{
			if (entity_array[i] != unit)
			{
				if (tile == App->map->WorldToMap(entity_array[i]->GetX(), entity_array[i]->GetY()))
					return true;
			}
		}
	}

	return false;
}