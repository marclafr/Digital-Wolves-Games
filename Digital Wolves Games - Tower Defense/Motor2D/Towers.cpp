#include "Towers.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "ProjectileManager.h"
#include "j1Animation.h"

Tower::Tower(TOWER_TYPE t_type, fPoint pos) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	anim_fire = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
	//TODO: anim_ice_floor = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_FLOOR));
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetHp(250);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 100.0f;
		range = 300;
		SetTextureID(T_TURRET);
		tower_type = T_BASIC_TOWER;
		projectile_type = P_BASIC_ARROW;
		break;

	case T_BOMBARD_TOWER:
		SetHp(250);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 100.0f;
		range = 300;
		SetTextureID(T_TURRET);
		tower_type = T_BOMBARD_TOWER;
		projectile_type = P_CANNONBALL;
		break;

	default:
		LOG("Error BUILDING TYPE STATS NULL");
		tower_type = T_NO_TYPE;
		break;
	}
}

Tower::~Tower()
{
}

void Tower::Update()
{
	if (IsBuilt() == true && IsAlive() == true)
		AI();

	if (Target != nullptr) {
		if (Target->GetHp() <= 0) {
			Target = nullptr;
		}
		/*
		ResetArrowPos();
		Target->Damaged(GetAttack());
		element_terrain_pos = Target->GetPosition();
		PrintElementTerrainTimer.Start();*/
		else if (AttackTimer.ReadSec() > 0.5)
		{
			App->projectile_manager->CreateProjectile(GetPosition(), Target, GetAttack(), 100, HEIGHT_BASIC_TOWER, 100, projectile_type);
			App->audio->PlayFx(App->entity_manager->fx_arrow);
			AttackTimer.Start();
		}
	}

	if (IsAlive() == true && this->GetHp() <= 0)
	{
		Target = nullptr;
		ConvertToRubble();
	}
	if (IsAlive() == false && GetDieTime() >= 2)
		DestroyBuilding();
	
	Draw();
}

void Tower::AI()
{

	if (Target == nullptr)
	{
		std::vector<Entity*> EntityVector = App->entity_manager->GetEntityVector();
		std::vector<Entity*>::iterator item = EntityVector.begin();
		for (; item != EntityVector.end(); item++)
		{
			if ((*item)->GetEntityType() == E_UNIT)
			{
				//TODO:Use a function from entity instead
				if ((*item)->GetX() >= (GetX() - GetRange()) && (*item)->GetX() < (GetX() + GetRange()) && (*item)->GetY() >= (GetY() - GetRange()) && (*item)->GetY() < (GetY() + GetRange()) && (*item)->GetHp() > 0 && (*item)->GetSide() == S_ENEMY)
				{
					Target = *item;
					AttackTimer.Start();
					
				}
			}
		}
	}
	else 
	{
		if (Target->GetHp() <= 0)
		{
			Target = nullptr;
			attacking = false;
		}
		else
		{
			if (Target != nullptr && Target->GetSide() != S_ALLY && Target->GetHp() > 0)
			{
				if (Target->GetX() >= (GetX() - GetRange()) && Target->GetX() < (GetX() + GetRange()) && Target->GetY() >= (GetY() - GetRange()) && Target->GetY() < (GetY() + GetRange()))
				{
					arrowpos.x = GetX();
					arrowpos.y = GetY() - 100;
				}
				else
				{
					Target = nullptr;
				}
			}
		}
	}
}

void Tower::Draw()
{
	if (PrintElementTerrainTimer.ReadSec() <= ELEMENT_TERRAIN_TIME)
		PrintElementTerrain(GetElementFromTower(tower_type), element_terrain_pos, 100);
	

	if (IsBuilt())
		App->render->PushEntity(this);

	else
	{
		if (GetBuildTime() <= 3)
		{
			SDL_Rect rect = { 0,0,96,65 };
			SetRect(rect);
			SetPivot(0.53125 * 96, 0.59375 * 65);
			
			
		}
		else if (GetBuildTime() > 3 && GetBuildTime() <= 6)
		{
			SDL_Rect rect = { 98,0,100,75 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 75);
			
		}
		else if (GetBuildTime() > 6 && GetBuildTime() <= 9)
		{
			SDL_Rect rect = { 202,0,100,75 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 75);


		}
		else if (GetBuildTime() > 9)
		{
			switch (tower_type)
			{
			case T_BASIC_TOWER:
				SetRect({ 302,0,107,208 });
				SetPivot(0.504673 * 107, 0.902913 * 208);
				SetRect({ 468,208,142,238 });
				SetPivot(0.485714 * 142, 0.910448 * 238);
				break;

			case T_BOMBARD_TOWER:
				SetRect({ 629,0,130,281 });
				SetPivot(0.5 * 130, 0.914591 * 281);
				break;

			case T_ICE_TOWER:
				SetRect({ 116,208,104,235 });
				SetPivot(0.485714 * 104, 0.910448 * 235);
				break;
			case T_BOMBARD_ICE_TOWER:
				SetRect({ 221,208,104,235 });
				SetPivot(0.485714 * 104, 0.910448 * 235);
				break;
			case T_AIR_TOWER:
				SetRect({ 326,208,142,238 });
				SetPivot(0.485714 * 142, 0.910448 * 238);
				break;
			case T_BOMBARD_AIR_TOWER:
				SetRect({ 468,208,142,238 });
				SetPivot(0.485714 * 142, 0.910448 * 238);
				break;
			case T_FIRE_TOWER:
				SetRect({ 924,283,119,237 });
				SetPivot(0.508621 * 119, 0.898305 * 237);
				break;
			case T_BOMBARD_FIRE_TOWER:
				SetRect({ 0,235,116,258 });
				SetPivot(0.508621 * 116, 0.898305 * 258);
				break;


			default:
				break;
			}
			BuildingComplete();
		}

		App->render->PushEntity(this);
	}
}

const TOWER_TYPE Tower::GetTowerType() const
{
	return tower_type;
}

const int Tower::GetRange() const
{
	return range;
}

void Tower::UpgradeTurret()
{
	/*SDL_Rect rect;
	switch (this->lvl)
	{
	case INV_LVL_1:
		rect = { 110,284,104,253 };
		SetRect(rect);
		this->SetAttack(200);
		SetPivot(0.519231 * 104, 0.905138 * 253);
		lvl = INV_LVL_2;
		break;
	case INV_LVL_2:
		rect = { 216,284,119,226 };
		SetRect(rect);
		SetPivot(0.504202 * 119, 0.893805 * 226);
		lvl = INV_LVL_3;
		break;
	case INV_LVL_3:
		//maxlvl¿?¿?¿?
		break;
	}*/
}

void Tower::PrintElementTerrain(TOWER_ELEMENT_TYPE element, fPoint center, int radius)
{
	SDL_Rect rect;
	iPoint pivot;
	anim_fire->Update(rect, pivot);
	App->render->Blit(App->tex->GetTexture(T_FIRE), center.x, center.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);

/*
	for (int i = 0; i < 3; i++)
	{
		int rand_distance = rand() % radius;
		float rand_angle = rand() % 360;
		rand_angle *= 3.14f / 360;
		int X = rand_distance*sin(rand_angle);
		int Y = rand_distance*cos(rand_angle);
		App->render->Blit(App->tex->GetTexture(T_FIRE), center.x + X, center.y + Y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
	}
*/
}

TOWER_ELEMENT_TYPE Tower::GetElementFromTower(TOWER_TYPE tower)
{
	switch (tower)
	{
	case T_BASIC_TOWER:
	case T_BOMBARD_TOWER:
		return TE_NO_ELEMENT;
		break;
	case T_FIRE_TOWER:
	case T_BOMBARD_FIRE_TOWER:
		return TE_FIRE;
		break;
	case T_ICE_TOWER:
	case T_BOMBARD_ICE_TOWER:
		return TE_ICE;
		break;
	case T_AIR_TOWER:
	case T_BOMBARD_AIR_TOWER:
		return TE_AIR;
		break;
	default:
		break;
	}
	return TE_NULL;
}