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
bool j1EntityManager::Load(pugi::xml_node& data)
{
	delete entity_quadtree;

	float m = App->map->data.height;
	float n = App->map->data.width;

	float map_h = (m + n) * 0.5f * App->map->data.tile_height + 75;//75 1 4 each tile
	float map_w = (m + n) * 0.5f * App->map->data.tile_width;

	float map_x = (m - n) * 0.5f * 0.5f * App->map->data.tile_width;
	float map_y = (m + n) * 0.5f * 0.5f * App->map->data.tile_height + 75 / 2; //75 1 4 each tile

																			   //IsoRect map(iPoint(map_x, map_y), map_w, map_h);
	IsoRect map(fPoint(map_x, map_y), map_w, map_h);
	entity_quadtree = new QuadTree(map);

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
	App->score->SetTime(Score.attribute("time_passed").as_int());
	App->wave_manager->ResetWave();
	App->wave_manager->SetWaveNum(Score.attribute("wave_num").as_int());
	return true;
}

void j1EntityManager::LoadResource(pugi::xml_node& data)
{
	pugi::xml_node actualresource = data;
	fPoint pos(actualresource.attribute("posx").as_int(), actualresource.attribute("posy").as_int());
	//Resources* actualres = (Resources*)App->entity_manager->CreateResource(RESOURCE_TYPE(actualresource.attribute("resource_type").as_int()), pos, actualresource.attribute("amount_collected").as_int(), actualresource.attribute("collect_time").as_int());
	
	/*switch (actualres->GetResourceType())
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
	*/
}

void j1EntityManager::LoadBuilding(pugi::xml_node& data)
{
	pugi::xml_node Actualbuilding = data;
	fPoint pos(Actualbuilding.attribute("posx").as_int(), Actualbuilding.attribute("posy").as_int());
	//Building* actualbuild = (Building*)App->entity_manager->CreateBuilding(BUILDING_TYPE(Actualbuilding.attribute("building_type").as_int()), pos, true);
	//actualbuild->SetHp(Actualbuilding.attribute("hp").as_int());
	//actualbuild->BuildingComplete();
	/*if (actualbuild->GetBuildingType() == B_TOWNHALL)
	{
		App->scene->townhall = actualbuild;
	}
	*/
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
	Tower* actualturret = (Tower*)App->entity_manager->CreateTower(TOWER_TYPE(Actualturret.attribute("tower_type").as_int()), pos, iPoint(Actualturret.attribute("tilex").as_int(), Actualturret.attribute("tilyx").as_int()));
	actualturret->SetHp(Actualturret.attribute("hp").as_int());
	actualturret->BuildingComplete();
}

bool j1EntityManager::Save(pugi::xml_node &data) const
{
	pugi::xml_node Buildings = data.append_child("buildings");
	pugi::xml_node Units = data.append_child("units");
	pugi::xml_node Turrets = data.append_child("turrets");
	pugi::xml_node Resourcess = data.append_child("resources");

	entity_quadtree->SaveAll(data);


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
