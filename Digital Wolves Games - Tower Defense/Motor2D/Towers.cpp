#include "Towers.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1EntityManager.h"
#include "ProjectileManager.h"
#include "j1Animation.h"
#include "j1Input.h"
#include "j1Map.h"
#include "Camera.h"

Tower::Tower(TOWER_TYPE t_type, fPoint pos, iPoint posintiles) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	SDL_Rect tower_rect;
	iPoint pivot;
	SDL_Texture* text;
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetHp(150);
		SetAttack(15);
		SetArmor(1);
		rate_of_fire = 0.90f;	//time between each attack in seconds
		range = 300;
		tower_type = T_BASIC_TOWER;
		projectile_type = P_BASIC_ARROW;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BASIC_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_BOMBARD_TOWER:
		SetHp(175);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 2.0f;
		range = 300;
		tower_type = T_BOMBARD_TOWER;
		projectile_type = P_CANNONBALL;
		SetBuildingType(B_CANNON);
		projectile_spd = 75;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;

	case T_FIRE_TOWER:
		SetHp(150);
		SetAttack(15);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_FIRE_TOWER;
		projectile_type = P_FIRE_ARROW;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_ICE_TOWER:
		SetHp(150);
		SetAttack(15);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_ICE_TOWER;
		projectile_type = P_ICE_ARROW;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_AIR_TOWER:
		SetHp(150);
		SetAttack(15);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_AIR_TOWER;
		projectile_type = P_AIR_ARROW;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_AIR_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_FIRE_TOWER:
		SetHp(175);
		SetAttack(40);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_BOMBARD_FIRE_TOWER;
		projectile_type = P_FIRE_CANNONBALL;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_FIRE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_ICE_TOWER:
		SetHp(175);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_BOMBARD_ICE_TOWER;
		projectile_type = P_ICE_CANNONBALL;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_ICE_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	case T_BOMBARD_AIR_TOWER:
		SetHp(175);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 1.0f;	//time between each attack in seconds
		range = 300;
		tower_type = T_BASIC_TOWER;
		projectile_type = P_AIR_CANNONBALL;
		SetBuildingType(B_TURRET);
		projectile_spd = 60;
		App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_AIR_TOWER);
		SetRect(tower_rect);
		SetPivot(pivot.x, pivot.y);
		break;
	default:
		LOG("Error BUILDING TYPE STATS NULL");
		tower_type = T_NO_TYPE;
		break;
	}
	SetTextureID(T_TURRET);
	AttackTimer.Start();

	

		App->pathfinding->MakeNoWalkable(iPoint(posintiles.x, posintiles.y));

		App->pathfinding->MakeNoConstruible_neutral(posintiles);
		App->pathfinding->MakeNoConstruible_ally(posintiles);
}

Tower::~Tower()
{
}

void Tower::Update(float dt)
{
	DT(dt);

	if (GetAIDT() >= dt * 15)
	{
		ResetDT();
		AI();
	}

	Draw();
}

void Tower::AI()
{
	if (Target != nullptr)
	{
		if (Target->GetX() < (GetX() - range) || Target->GetX() > (GetX() + range) || Target->GetY() < (GetY() - range) || Target->GetY() > (GetY() + range))
			Target = nullptr;
		if (Target != nullptr && Target->GetHp() <= 0)
		{
			Target = nullptr;
			attacking = false;
		}

	}
	else
		attacking = false;

	if (Target == nullptr && AttackTimer.ReadSec() >= rate_of_fire && attacking == false && IsBuilt() == true && IsAlive() == true)
	{
		Target = App->entity_manager->LookForEnemies(GetRange(), GetPosition(), GetSide());
		if (Target != nullptr)
			attacking = true;
	}

	if (attacking == true && Target != nullptr && AttackTimer.ReadSec() >= rate_of_fire)
	{
		App->projectile_manager->CreateProjectile(GetPosition(), Target, GetAttack(), projectile_spd, HEIGHT_BASIC_TOWER, 100, projectile_type);
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))
		{
			if (tower_type == T_BASIC_TOWER || tower_type == T_ICE_TOWER || tower_type == T_AIR_TOWER || tower_type == T_FIRE_TOWER)
				App->audio->PlayFx(App->audio->fx_arrow);
			else
				App->audio->PlayFx(App->audio->fx_cannon);
		}
		AttackTimer.Start();
	}

	if (IsAlive() == true && this->GetHp() <= 0)
	{
		Target = nullptr;
		ConvertToRubble();
	}
	if (IsAlive() == false && GetDieTime() >= 2)
		DestroyBuilding();
}

void Tower::Draw()
{
	if (IsBuilt())
	{
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY())) App->render->PushInGameSprite(this);
	}
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
				break;

			case T_BOMBARD_TOWER:
				SetRect({ 629,0,130,281 });
				SetPivot(0.5 * 130, 0.914591 * 281);
				break;

			default:
				break;
			}
			BuildingComplete();
			AttackTimer.Start();
		}
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + GetX(), App->render->camera->GetPosition().y + GetY()))   App->render->PushInGameSprite(this);
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

void Tower::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("turrets");
	pugi::xml_node ActualTurret = build.append_child("turret");
	ActualTurret.append_attribute("tower_type") = GetTowerType();
	ActualTurret.append_attribute("posx") = GetX();
	ActualTurret.append_attribute("posy") = GetY();
	ActualTurret.append_attribute("tilex") = posintiles.x;
	ActualTurret.append_attribute("tiley") = posintiles.y;
	ActualTurret.append_attribute("hp") = GetHp();
}

float Tower::GetSpeed()
{
	return rate_of_fire;

}

void Tower::SetSpeed(float new_speed)
{
	if (new_speed<rate_of_fire)
		rate_of_fire -= new_speed;
}

void Tower::SetRange(float new_range)
{
	range += new_range;
}

void Tower::UpgradeTurret(TURRET_UPGRADE type)
{
	if (this->IsBuilt())
	{
		SDL_Rect tower_rect;
		iPoint pivot;
		SDL_Texture* text;
		if (GetTowerType() == T_BASIC_TOWER)
		{
			switch (type)
			{
			case TU_FIRE:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_FIRE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_FIRE_ARROW;
					tower_type = T_FIRE_TOWER;
					SetBuildingType(B_TURRET_UPGRADED);
					SetAttack(GetAttack() + 10);
				}
				break;
			case TU_ICE:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_ICE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_ICE_ARROW;
					tower_type = T_ICE_TOWER;
					SetBuildingType(B_TURRET_UPGRADED);
				}
				break;
			case TU_AIR:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_AIR_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_AIR_ARROW;
					tower_type = T_AIR_TOWER;
					SetBuildingType(B_TURRET_UPGRADED);
					SetSpeed(0.5f);
					SetRange(2.0f);

				}
				break;
			default:
				break;
			}
		}
		else if (GetTowerType() == T_BOMBARD_TOWER)
		{
			switch (type)
			{
			case TU_FIRE:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_FIRE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_FIRE_CANNONBALL;
					tower_type = T_BOMBARD_FIRE_TOWER;
					SetBuildingType(B_CANNON_UPGRADED);
					SetAttack(GetAttack() + 3);
				}
				break;
			case TU_ICE:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_ICE_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_ICE_CANNONBALL;
					tower_type = T_BOMBARD_ICE_TOWER;
					SetBuildingType(B_CANNON_UPGRADED);
				}
				break;
			case TU_AIR:
				if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_FIRE_TOWER)) == INV_LVL_UNLOCKED)
				{
					App->tex->GetTowerTexture(text, tower_rect, pivot, T_BOMBARD_AIR_TOWER);
					SetRect(tower_rect);
					SetPivot(pivot.x, pivot.y);
					projectile_type = P_AIR_CANNONBALL;
					tower_type = T_BOMBARD_AIR_TOWER;
					SetBuildingType(B_CANNON_UPGRADED);
					SetSpeed(1.0f);
					SetRange(2.0f);
				}
				break;
			default:
				break;
			}
		}
	}
}
