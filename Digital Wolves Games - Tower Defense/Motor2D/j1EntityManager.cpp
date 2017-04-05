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
	return new_entity;
}

Entity * j1EntityManager::CreateResource(RESOURCE_TYPE r_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Resources(r_type, pos);
	entity_array.push_back(new_entity);
	return new_entity;
}
Entity * j1EntityManager::CreateWall(WALL_TYPE w_type, fPoint pos)
{
	Entity* new_entity = (Entity*) new Wall(w_type, pos);
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
				entity_array[i]->SetEntityStatus(ST_SELECTED);
				if(entity_array[i]->GetEntityType() != ENTITY_TYPE:: E_WALL) App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				entity_array[i]->SetEntityStatus(ST_SELECTED);
				if(entity_array[i]->GetEntityType() != ENTITY_TYPE::E_WALL) App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x > select_rect.x && unit_x < select_rect.w && unit_y < select_rect.y && unit_y > select_rect.h)
			{
				entity_array[i]->SetEntityStatus(ST_SELECTED);
				if (entity_array[i]->GetEntityType() != ENTITY_TYPE::E_WALL) App->scene->panel_info->AddEntitySelection(entity_array[i]);
			}
			else if (unit_x < select_rect.x && unit_x > select_rect.w && unit_y > select_rect.y && unit_y < select_rect.h)
			{
				entity_array[i]->SetEntityStatus(ST_SELECTED);
				if (entity_array[i]->GetEntityType() != ENTITY_TYPE::E_WALL) App->scene->panel_info->AddEntitySelection(entity_array[i]);
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
		entity_array[i]->SetEntityStatus(ST_NON_SELECTED);
	}
	if (!App->scene->panel_info->isSelectionEmpty())
		App->scene->panel_info->DeleteSelection();
}

void j1EntityManager::DeleteEntity(Entity* ptr)
{
	switch (ptr->GetEntityType())
	{
		case E_UNIT:
			DeleteUnit((Unit*)ptr);
			break;
		case E_BUILDING:
			DeleteBuilding(ptr);
			break;
		case E_RESOURCE:
			DeleteResource(ptr);
			break;
		case E_WALL:
			DeleteWall(ptr);
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

void j1EntityManager::DeleteBuilding(Entity* ptr)
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

void j1EntityManager::DeleteResource(Entity* ptr)
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
void j1EntityManager::DeleteWall(Entity* ptr)
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

void j1EntityManager::BlitEnemyDeathCount()
{
	if (App->scene->active)
	{
		uint width = 0;
		uint height = 0;

		App->win->GetWindowSize(width, height);

		SDL_Rect rect{ width - 120, 25, 120, 60};

		int secs = WINNING_TIME - App->scene->game_time.ReadSec();
		int mins = secs / 60;
		secs -= mins * 60;
		
		sprintf_s(time_left, 256, " Time Left: %d:%d", mins, secs);
		
		if (enemy_killed)
		{
			SDL_DestroyTexture(num_kills_texture);
			SDL_DestroyTexture(score_texture);

			sprintf_s(text_num_kills, 256, " Enemies Killed: %d", enemies_killed);
			sprintf_s(text_score, 256, " Score: %d", score);

			num_kills_texture = App->font->Print(text_num_kills);
			score_texture = App->font->Print(text_score);

			enemy_killed = false;
		}

		time_texture = App->font->Print(time_left);

		SDL_RenderDrawRect(App->render->renderer, &rect);
		SDL_SetRenderDrawColor(App->render->renderer, 100, 100, 100, 100);
		SDL_RenderFillRect(App->render->renderer, &rect);

		App->render->Blit(num_kills_texture, -App->render->camera->GetPosition().x + rect.x, -App->render->camera->GetPosition().y + rect.y);
		App->render->Blit(score_texture, -App->render->camera->GetPosition().x + rect.x, -App->render->camera->GetPosition().y + rect.y + 20);
		App->render->Blit(time_texture, -App->render->camera->GetPosition().x + rect.x, -App->render->camera->GetPosition().y + rect.y + 40);

		SDL_DestroyTexture(time_texture);
	}
}

void j1EntityManager::EnemyDead()
{
	enemies_killed++;
	score += 100;
	enemy_killed = true;
}

bool j1EntityManager::Start()
{
	fx_twohanded_die01 = App->audio->LoadFx("audio/fx/Male_Death01.wav");
	fx_twohanded_die02 = App->audio->LoadFx("audio/fx/Male_Death02.wav");
	fx_twohanded_die03 = App->audio->LoadFx("audio/fx/Male_Death03.wav");
	fx_twohanded_die04 = App->audio->LoadFx("audio/fx/Male_Death04.wav");
	fx_twohanded_die05 = App->audio->LoadFx("audio/fx/Male_Death05.wav");
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
	{
		if (entity_array[i]->ToDelete() == true)
		{
			DeleteEntity(entity_array[i]);
		}
	}

	BlitEnemyDeathCount();

	return true;
}

Entity * j1EntityManager::CheckForCombat(iPoint position, int range, Side side)
{
	for  (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetX() <= position.x + range && entity_array[i]->GetX() >= position.x - range &&
			entity_array[i]->GetY() <= position.y + range && entity_array[i]->GetY() >= position.y - range &&
			side != entity_array[i]->GetSide() && entity_array[i]->GetHp() >= 0
			&& (entity_array[i]->GetEntityType() == E_UNIT || entity_array[i]->GetEntityType() == E_BUILDING))
			return entity_array[i];
	}
	return nullptr;
}

Entity* j1EntityManager::CheckForObjective(iPoint position, int vision_range, Side side)
{
	Entity* ret = nullptr;
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetX() <= position.x + vision_range && entity_array[i]->GetX() >= position.x - vision_range &&
			entity_array[i]->GetY() <= position.y + vision_range && entity_array[i]->GetY() >= position.y - vision_range &&
			side != entity_array[i]->GetSide() && entity_array[i]->GetHp() >= 0 && entity_array[i]->GetSide() != S_NEUTRAL
			&& entity_array[i]->GetEntityType() == E_UNIT)
		{
			if(ret == nullptr)
				ret = entity_array[i];
			else
			{
				if (position.DistanceTo(iPoint(ret->GetX(), ret->GetY())) > position.DistanceTo(iPoint(entity_array[i]->GetX(),entity_array[i]->GetY())))
				{
					ret = entity_array[i];
				}
			}
		}			
	}
	return ret;
}

std::vector<Entity*> j1EntityManager::GetEntityVector()
{
	return entity_array;
}

bool j1EntityManager::IsUnitInTile(const Unit* unit, const iPoint tile)const
{
	for (int i = 0; i < entity_array.size(); i++)
	{
		if (entity_array[i]->GetEntityType() == E_UNIT)
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

int j1EntityManager::GetScore()
{
	return score;
}

void j1EntityManager::AddScore(int points)
{
	score += points;
}

void j1EntityManager::ResetScores()
{
	score = 0;
	enemies_killed = 0;
	enemy_killed = true;
}
