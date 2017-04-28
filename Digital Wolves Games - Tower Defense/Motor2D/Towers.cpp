#include "Towers.h"
#include "j1App.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "j1EntityManager.h"

Tower::Tower(TOWER_TYPE t_type, fPoint pos) : Building(B_TURRET, pos, S_ALLY), tower_type(t_type)
{
	SDL_Rect rect;
	switch (t_type)
	{
	case T_BASIC_TOWER:
		SetHp(250);
		SetAttack(30);
		SetArmor(1);
		rate_of_fire = 100.0f;
		range = 250;
		SetRect({ 610,1,107,206 });
		SetPivot(0.504673 * 107, 0.902913 * 206);
		SetTextureID(T_TURRET);
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
			attacking = false;
			Target = nullptr;
		}
		else
		ArrowCreation();
	}


	if (attacking == true) {
		iPoint targetpos;
		targetpos.x = Target->GetX();
		targetpos.y = Target->GetY() - 35;
		UpdateArrow(arrowpos, targetpos);
		if (arrowpos.x > targetpos.x - 3 && arrowpos.x < targetpos.x + 3 && arrowpos.y < targetpos.y + 3 && arrowpos.y > targetpos.y - 3)
		{
			attacking = false;
			Target->Damaged(GetAttack());
		}
		SDL_Rect rect = { 1,45,25,4 };
		iPoint pivots(0.5 * 25, 0.25 * 4);
		UpdateArrowSprite(targetpos, rect, pivots);
		App->render->Blit(App->tex->GetTexture(T_ARROW), arrowpos.x, arrowpos.y, &rect, SDL_FLIP_NONE, pivots.x, pivots.y);
	}

	if (IsAlive() == true && this->GetHp() <= 0)
		ConvertToRubble();

	if (IsAlive() == false && GetDieTime() >= 2)
		DestroyBuilding();
	
	Draw();
}

void Tower::AI()
{
	if (GetEntityStatus() == ST_SELECTED)
	{
		/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		{
			if (this->GetBuildingType() == B_TURRET)		UpgradeTurret();
		}
		if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		{
			this->SetHp(0);
		}*/
	}
	if (Target == nullptr)
	{
		attacking = false;
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
					App->audio->PlayFx(App->entity_manager->fx_arrow);
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
			if (Target != nullptr && Target->GetSide() != S_ALLY && Target->GetHp() > 0 && attacking == false)
			{
				if (Target->GetX() >= (GetX() - GetRange()) && Target->GetX() < (GetX() + GetRange()) && Target->GetY() >= (GetY() - GetRange()) && Target->GetY() < (GetY() + GetRange()))
				{
					arrowpos.x = GetX();
					arrowpos.y = GetY() - 100;
					attacking = true;
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
	if (IsBuilt())
		App->render->PushEntity(this);
	
	else
	{
		if (GetBuildTime() <= 3)
		{
			SDL_Rect rect = { 394,1,96,64 };
			SetRect(rect);
			SetPivot(0.53125 * 96, 0.59375 * 64);

		}
		else if (GetBuildTime() > 3 && GetBuildTime() <= 6)
		{
			SDL_Rect rect = { 376,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);

		}
		else if (GetBuildTime() > 6 && GetBuildTime() <= 9)
		{
			SDL_Rect rect = { 478,539,100,73 };
			SetRect(rect);
			SetPivot(0.55 * 100, 0.643836 * 73);


		}
		else if (GetBuildTime() > 9 && GetBuildTime() <= 11)
		{
			switch (tower_type)
			{
			case T_BASIC_TOWER:
				SetRect({ 610,1,107,206 });
				SetPivot(0.504673 * 107, 0.902913 * 206);
				break;

			case T_BOMBARD_TOWER:
				//TODO
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
void Tower::ArrowCreation() 
{
	iPoint pos;
	SDL_Rect rect = { 350,268,5,5};
	//\mathbf{ B }(t) = (1 - t) ^ { 2 }\mathbf{ P }_0 + 2t(1 - t)\mathbf{ P }_1 + t^{ 2 }\mathbf{ P }_2 \mbox{ , } t \in[0, 1].
	for (float i = 0; i < 1; i += 0.05)
	{
		fPoint initial_point = { GetPosition().x,GetPosition().y - ARROW_TOWER_HEIGHT};
		fPoint last_point = Target->GetPosition();
		fPoint mid_point = { (initial_point.x + last_point.x) / 2,((initial_point.y + last_point.y) / 2) - 100 };

		pos.x = ((1 - i)*(1 - i)*initial_point.x) + ((2 * i)*(1 - i)*mid_point.x) + ((i*i)*last_point.x);
		pos.y = ((1 - i)*(1 - i)*initial_point.y) + ((2 * i)*(1 - i)*mid_point.y) + ((i*i)*last_point.y);
		App->render->Blit(App->tex->GetTexture(T_WALL), pos.x, pos.y, &rect,SDL_FLIP_NONE,0,0,1,0,false);
	}


}
