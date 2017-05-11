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


bool j1EntityManager::CleanUp() {

	DELETE_PTR(entity_quadtree);
	return true;
}

Entity * j1EntityManager::CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side)//TODO make const if we can take priority out
{
	Entity* new_entity = (Entity*) new Unit(u_type, pos, side, priority);
	entity_quadtree->PushBack(new_entity);
	priority++;
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

Entity * j1EntityManager::CreateTower(TOWER_TYPE t_type, fPoint pos) const
{
	Entity* new_entity = (Entity*) new Tower(t_type, pos);
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

	entity_quadtree->Search(select_rect, selection);

	for (std::vector<Entity*>::iterator it = selection.begin(); it != selection.end(); ++it)
	{
		(*it)->SetEntityStatus(ST_SELECTED);
		if ((*it)->GetEntityType() == E_UNIT && unit_found == false)
		{
			for(std::vector<Entity*>::iterator item = selection.begin(); item != it - 1; ++item)
			{
				(*item)->SetEntityStatus(ST_NON_SELECTED);
				selection.erase(item);
			}
			for (std::vector<Entity*>::iterator item = it; item != selection.end(); ++item)
				if ((*item)->GetEntityType() != E_UNIT)
				{
					(*item)->SetEntityStatus(ST_NON_SELECTED);
					selection.erase(item);
				}
				else
					(*it)->SetEntityStatus(ST_SELECTED);
		}
		break;
	}

	App->uimanager->CreatePanelInfo(selection);
}

void j1EntityManager::UnselectEverything() const
{
	for (int i = 0; i < App->scene->selection.size(); i++)
		App->scene->selection[i]->SetEntityStatus(ST_NON_SELECTED);

	App->scene->selection.clear();
	App->uimanager->DeleteSelectionPanelInfo();
	App->uimanager->DeletePanelButtons();
}

void j1EntityManager::Select(Entity * select) const
{
	App->entity_manager->UnselectEverything();
	select->SetEntityStatus(ST_SELECTED);
	App->scene->selection.push_back(select);
	App->uimanager->CreatePanelInfo(App->scene->selection);
}

Entity * j1EntityManager::LookForEnemies(int range, iPoint pos) const
{
	return entity_quadtree->SearchFirst(range, pos);
}

void j1EntityManager::CheckClick(int mouse_x, int mouse_y) const
{
	App->scene->selection.clear();
	iPoint click_point = App->render->ScreenToWorld(mouse_x, mouse_y);
	Entity* clicked = entity_quadtree->SearchFirst(1, click_point);
	if(clicked != nullptr)
		App->scene->selection.push_back(clicked);
}

bool j1EntityManager::Start()
{
	LoadAllFx();

	float m = App->map->data.height;
	float n = App->map->data.width;

	float map_h = (m + n) * 0.5f * App->map->data.tile_height + 75;//75 1 4 each tile
	float map_w = (m + n) * 0.5f * App->map->data.tile_width;

	float map_x = (m - n) * 0.5f * 0.5f * App->map->data.tile_width;
	float map_y = (m + n) * 0.5f * 0.5f * App->map->data.tile_height + 75 / 2; //75 1 4 each tile

	//IsoRect map(iPoint(map_x, map_y), map_w, map_h);
	IsoRect map(iPoint(map_x, map_y), map_w, map_h);
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

Entity * j1EntityManager::CheckForCombat(iPoint position, int range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(range, position, side);
}

Entity* j1EntityManager::CheckForObjective(iPoint position, int vision_range, Side side) const
{
	return entity_quadtree->SearchFirstEnemy(vision_range, position, side);
}

void j1EntityManager::CheckUnitCollisions(const Unit * ptr) const
{
	entity_quadtree->CheckUnitCollisions(ptr);
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	if (tile == App->map->WorldToMap(unit->GetX(), unit->GetY()))
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

void j1EntityManager::DrawQuadTree() const
{
	entity_quadtree->DrawRects();
}
