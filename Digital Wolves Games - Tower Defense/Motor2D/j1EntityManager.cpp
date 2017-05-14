#include "j1App.h"
#include "j1EntityManager.h"
#include "Units.h"
#include "j1UIManager.h"
#include "UIHUDPanelInfo.h"
#include "j1Scene.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Window.h"
#include "j1Score.h"
#include "j1WaveManager.h"
#include "Camera.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1ScoreScene.h"

j1EntityManager::j1EntityManager() : j1Module()
{
	name.assign("Units");
}

j1EntityManager::~j1EntityManager() {}


bool j1EntityManager::CleanUp() {

	DELETE_PTR(entity_quadtree);
	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side)//TODO make const if we can take priority out
{
	Entity* new_entity = (Entity*) new Unit(u_type, pos, side, priority);
	entity_quadtree->PushBack(new_entity);
	priority++;
	App->score_scene->units_count++;
	return new_entity;
}

Entity * j1EntityManager::CreateBuilding(BUILDING_TYPE b_type, fPoint pos, Side side) const
{
	Entity* new_entity = (Entity*) new Building(b_type, pos, side);
	entity_quadtree->PushBack(new_entity);
	if (b_type == B_TOWNHALL)
		App->uimanager->SetTownHall((Building*)new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateTower(TOWER_TYPE t_type, fPoint pos, iPoint posintiles) const
{
	Entity* new_entity = (Entity*) new Tower(t_type, pos, posintiles);
	entity_quadtree->PushBack(new_entity);
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos) const
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos);
	entity_quadtree->PushBack(new_entity);
	return new_entity;
}

void j1EntityManager::SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection) const
{
	bool unit_found = false;

	selection.clear();

	entity_quadtree->Selection(select_rect, selection);

	for (std::vector<Entity*>::iterator it = selection.begin(); it != selection.end(); ++it)
	{
		(*it)->SetEntityStatus(ST_SELECTED);

		if ((*it)->GetEntityType() == E_UNIT && (*it)->GetHp() > 0 && unit_found == false)
		{
			unit_found = true;
			it = selection.begin();
		}

		if ((*it)->GetHp() > 0)
		{
			if (unit_found)
			{
				if ((*it)->GetEntityType() == E_UNIT && (*it)->GetHp() > 0)
					(*it)->SetEntityStatus(ST_SELECTED);
				else
				{
					(*it)->SetEntityStatus(ST_NON_SELECTED);
					selection.erase(it);
					it = selection.begin();
					if (selection.size() == 0)
						break;
				}
			}
			else
				(*it)->SetEntityStatus(ST_SELECTED);
		}
		else
		{
			(*it)->SetEntityStatus(ST_NON_SELECTED);
			selection.erase(it);
			it = selection.begin();
			if (selection.size() == 0)
				break;
		}
	}

	App->uimanager->CreatePanelInfo(selection);
}

void j1EntityManager::UnselectEverything() const
{
	for (int i = 0; i < App->scene->selection.size(); i++)
		App->scene->selection[i]->SetEntityStatus(ST_NON_SELECTED);

	App->scene->selection.clear();
	App->uimanager->DeleteSelectionPanelInfo();
	App->uimanager->SetPanelButtons(nullptr);
}

void j1EntityManager::Select(Entity * select) const
{
	App->entity_manager->UnselectEverything();
	select->SetEntityStatus(ST_SELECTED);
	App->scene->selection.push_back(select);
	App->uimanager->CreatePanelInfo(App->scene->selection);
}

Entity * j1EntityManager::LookForEnemies(int range, fPoint pos, Side side, ENTITY_TYPE entity_type) const
{
	IsoRect rect(pos,range * 2.0f, range*2.0f);
	return entity_quadtree->SearchFirstEnemy(rect, side, entity_type);
}

void j1EntityManager::CheckClick(int mouse_x, int mouse_y) const
{
	App->scene->selection.clear();
	iPoint click_point = App->render->ScreenToWorld(mouse_x, mouse_y);
	Entity* clicked = entity_quadtree->SearchFirst(1, fPoint(click_point.x,click_point.y));
	if(clicked != nullptr)
		App->scene->selection.push_back(clicked);
}

bool j1EntityManager::Start()
{

	float m = App->map->data.height;
	float n = App->map->data.width;

	float map_h = (m + n) * 0.5f * App->map->data.tile_height + 75;//75 1 4 each tile
	float map_w = (m + n) * 0.5f * App->map->data.tile_width;

	float map_x = (m - n) * 0.5f * 0.5f * App->map->data.tile_width;
	float map_y = (m + n) * 0.5f * 0.5f * App->map->data.tile_height + 75 / 2; //75 1 4 each tile

	//IsoRect map(iPoint(map_x, map_y), map_w, map_h);
	IsoRect map(fPoint(map_x, map_y), map_w, map_h);
	entity_quadtree = new QuadTree(map);

	return true;
}

bool j1EntityManager::Update(float dt)
{
	entity_quadtree->UpdateAll(dt);
	return true;
}

bool j1EntityManager::PostUpdate()
{
	entity_quadtree->DeleteEntities();
	return true;
}

Entity * j1EntityManager::CheckForCombat(fPoint position, int range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(range, position, side);
}

Entity* j1EntityManager::CheckForObjective(fPoint position, int vision_range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(vision_range, position, side);
}

Unit* j1EntityManager::CheckUnitCollisions(const Unit * ptr) const
{
	return entity_quadtree->CheckCollisions(ptr->GetUnitCircle());
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	if (tile == App->map->WorldToMap(unit->GetX(), unit->GetY()))
		return true;
	return false;
}



void j1EntityManager::DrawQuadTree() const
{
	entity_quadtree->DrawRects();
}

bool j1EntityManager::Save(pugi::xml_node &data) const
{
	pugi::xml_node Buildings = data.append_child("buildings");
	pugi::xml_node Units = data.append_child("units");
	pugi::xml_node Turrets = data.append_child("turrets");
	pugi::xml_node Resourcess = data.append_child("resources");

	/*for (int k = 0; k <entity_array.size(); k++) {
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
			if (unit->GetSide() == S_ALLY)	unit->SaveUnit(Units);
		}
		else if (entity_array[k]->GetEntityType() == E_RESOURCE)
		{
			Resources* rest = (Resources*)entity_array[k];
			rest->SaveResource(Resourcess);
		}
	}
	*/


	pugi::xml_node AmountOfResources = data.append_child("resources_amount");

	App->scene->resources->SaveResourcesAmount(AmountOfResources);

	pugi::xml_node Score = data.append_child("score");

	Score.append_attribute("points") = App->score->GetScore();
	Score.append_attribute("enemies_killeds") = App->score->GetEnemiesKilled();
	Score.append_attribute("time_passed") = App->score->GetTime();
	Score.append_attribute("wave_num") = App->wave_manager->GetWaveNum();
	return true;
	return true;
}
