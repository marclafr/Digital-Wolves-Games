#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1UIManager.h"
#include "j1Scene.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::Awake() { return true; }

bool j1EntityManager::CleanUp() { // not done

	if(sprites != nullptr)
	App->tex->UnLoad(sprites);

	std::vector<Entity*>::iterator item = entity_array.begin();
	for (int i = 0; i < entity_array.size(); i++)
		RELEASE(entity_array[i]);

	entity_array.clear();

	//Borrar enemigos
	//for(uint i = 0;             ; ++i)
		//if(unit_list[i])
	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Unit(u_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreatBuilding(BUILDING_TYPE b_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Building(b_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect)
{
	for (int i = 0; i < entity_array.size(); i++)
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
	if(!App->scene->panel_info->isSelectionEmpty())
		App->scene->panel_info->CreatePanel();
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

void j1EntityManager::DeleteEntity(Entity * ptr)
{
	switch (ptr->GetEntityType())
	{
		case UNIT:
			DeleteUnit(ptr);
			break;

		case BUILDING:
			DeleteBuilding(ptr);
			break;
	}
}

void j1EntityManager::DeleteUnit(Entity * ptr)
{
}

void j1EntityManager::DeleteBuilding(Entity * ptr)
{
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
	{
		if (entity_array[i]->ToDelete() == true)
		{
			DeleteEntity(entity_array[i]);
			entity_array.erase(entity_array.begin() + i);
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