
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1EntityManager.h"
#include "j1Map.h"
#include "j1Audio.h"

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority): Entity(E_UNIT, pos, side), unit_type(u_type), direction(D_EAST), action_type(A_IDLE), changed(false), attacking(nullptr), priority(priority), state(VIGILANT)
{
	if (GetSide() == S_ENEMY)
	{
		this->action_type = A_WALK;
		GoTo(TOWN_HALL);
		state = MOVING;
		changed = true;
	}
		
	switch (u_type)
	{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case U_TWOHANDEDSWORDMAN:
		SetHp(60);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 30;
		vision_range = 300;
		unit_class = C_INFANTRY;
		unit_radius = 6;
		SetTextureID(T_TWOHANDEDSWORDMAN);
		fx_twohanded_die01 = App->audio->LoadFx("audio/fx/Male_Death01.wav");
		fx_twohanded_die02 = App->audio->LoadFx("audio/fx/Male_Death02.wav");
		fx_twohanded_die03 = App->audio->LoadFx("audio/fx/Male_Death03.wav");
		fx_twohanded_die04 = App->audio->LoadFx("audio/fx/Male_Death04.wav");
		fx_twohanded_die05 = App->audio->LoadFx("audio/fx/Male_Death05.wav");
		priority = 3;

		break;

	case U_CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4;
		rate_of_fire = 2;
		range = 300;
		vision_range = 350;
		unit_class = C_ARCHER;
		unit_radius = 8;
		SetTextureID(T_CAVALRYARCHER);
		priority = 2;
		break;

	case U_SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 30;
		vision_range = 100;
		unit_class = C_SIEGE;
		unit_radius = 15;
		SetTextureID(T_SIEGERAM);
		priority = 1;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = C_NO_CLASS;
		break;
	}

	animation = new Animation(App->anim->GetAnimationType(unit_type, action_type, direction));
}

Unit::~Unit()
{
	if (animation != nullptr)
		delete animation;
}

void Unit::Update()
{
	if (changed == true)
	{
		animation->ChangeAnimation(App->anim->GetAnimationType(unit_type, action_type, direction));
		changed = false;
	}

	if (state != DEAD)
	{
		AI();
		Move();
		Draw();

		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == ST_SELECTED)
		{
			SetHp(0);
		}
	}

	else
	{
		Draw();
		Die();
	}
}

void Unit::Move()
{
	if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == ST_SELECTED)
	{
		this->path_list.clear();

		App->input->GetMousePosition(destination.x, destination.y);
		destination.x -= App->render->camera->GetPosition().x;
		destination.y -= App->render->camera->GetPosition().y;

		if(GoTo(destination) == false)
		{
			state = VIGILANT;
			this->action_type = A_IDLE;
			changed = true;
		}
	}

	if (state == MOVING || state == MOVING_TO_FIGHT)
	{
		this->SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);

		if (path_objective.DistanceTo(iPoint(GetX(), GetY())) < 3)
		{
			//center the unit to the tile
			if (!GetNextTile())
			{
				state = VIGILANT;
				this->action_type = A_IDLE;
				changed = true;
			}
		}
	}
}

void Unit::AI()
{

	if (GetHp() <= 0)
		state = DEAD;

	if (state != DEAD)
	{
		if (state == VIGILANT || state == MOVING || state == MOVING_TO_FIGHT && animation->Finished())
		{
			iPoint new_obj = App->entity_manager->CheckForObjective(iPoint(GetX(), GetY()), GetVisionRange(), GetSide());
			if (new_obj.x != -1)
			{
				if (!App->pathfinding->IsWalkable(App->map->WorldToMap(new_obj.x, new_obj.y)))
				{
					App->pathfinding->MakeWalkable(App->map->WorldToMap(new_obj.x, new_obj.y));
					GoTo(new_obj);
					App->pathfinding->MakeNoWalkable(App->map->WorldToMap(new_obj.x, new_obj.y));
				}
				
				else
					GoTo(new_obj);

				state = MOVING_TO_FIGHT;
			}
		}

		if (state == MOVING_TO_FIGHT)
		{
			attacking = App->entity_manager->CheckForCombat(iPoint(GetX(), GetY()), GetRange(), GetSide());

			if (attacking != nullptr)
			{
				state = FIGHTING;
				this->action_type = A_ATTACK;
				this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
				changed = true;
			}
		}

		if (state == FIGHTING)
		{
			if (animation->Finished())
				attacking->Damaged(attack);

			if (attacking->GetHp() <= 0)
			{
				state = VIGILANT;

				if (GetSide() == S_ENEMY)
				{
					this->action_type = A_WALK;
					GoTo(TOWN_HALL);
					state = MOVING;
					changed = true;
				}
				else
				{
					this->action_type = A_IDLE;
					changed = true;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			action_type = A_ATTACK;
			changed = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
		{
			action_type = A_IDLE;
			changed = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			action_type = A_WALK;
			changed = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		{
			action_type = A_DIE;
			changed = true;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			SetHp(0);
		}
	}
}

void Unit::Draw()
{
	SDL_Rect rect;
	iPoint pivot;

	animation->Update(rect, pivot);
	
	SetPivot(pivot.x, pivot.y);
	SetRect(rect);

	App->render->PushEntity(this);

}

void Unit::Die()
{

	if (animation->Finished() && action_type == A_DIE)
	{
		action_type = A_DISAPPEAR;
		changed = true;
	}

	else if (animation->Finished() && action_type == A_DISAPPEAR)
	{
		this->SetToDelete();
	}
	
	if (changed == false && action_type != A_DIE && action_type != A_DISAPPEAR)
	{
		action_type = A_DIE;
		changed = true;

		//THIS DOES NOT BELONG HERE
		//TODO
		if (GetRandNum(5) == 1)
		{
			App->audio->PlayFx(fx_twohanded_die01);
		}
		else if (GetRandNum(5) == 2)
		{
			App->audio->PlayFx(fx_twohanded_die02);
		}
		else if (GetRandNum(5) == 3)
		{
			App->audio->PlayFx(fx_twohanded_die03);
		}
		else if (GetRandNum(5) == 4)
		{
			App->audio->PlayFx(fx_twohanded_die04);
		}
		else if (GetRandNum(5) == 5)
		{
			App->audio->PlayFx(fx_twohanded_die05);
		}
	}
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const UNIT_CLASS Unit::GetUnitClass() const
{
	return unit_class;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}

const UNIT_STATE Unit::GetUnitState() const
{
	return state;
}

const int Unit::GetUnitRadius() const
{
	return unit_radius;
}

int Unit::GetPath(iPoint dest)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	return App->pathfinding->CreatePath(ori, destinat, path_list);
}

const int Unit::GetAttack() const
{
	return attack;
}

const int Unit::GetRange() const
{
	return range;
}

const int Unit::GetVisionRange() const
{
	return vision_range;
}

const bool Unit::IsMoving() const
{
	if (state == MOVING || state == MOVING_TO_FIGHT)
		return true;
	return false;
}

const int Unit::GetPriority() const
{
	return priority;
}

void Unit::PopFirstPath()
{
	path_list.pop_front();
}

void Unit::SetAction(const ACTION_TYPE action)
{
	action_type = action;
}

void Unit::SetIsMoving()
{
	state = MOVING;
}

void Unit::LookAt(iPoint pos)
{

}

bool Unit::GoTo( iPoint destination)
{
	path_list.clear();
	if (this->GetPath({ destination.x, destination.y }) != -1)
	{
		path_list.pop_front();
		GetNextTile();
		action_type = A_WALK;
		changed = true;
		state = MOVING;
		return true;
	}
	return false;
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (path_list.size() <= 0)
		return false;

	path_objective = App->map->MapToWorld(path_list.front().x, path_list.front().y);
	path_list.pop_front();

	move_vector.x = (float)path_objective.x - GetX();
	move_vector.y = (float)path_objective.y - GetY();
	float modul = (sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y));
	move_vector.x = move_vector.x / modul;
	move_vector.y = move_vector.y / modul;

	iPoint direction_vec;
	direction_vec.x = path_objective.x - GetX();
	direction_vec.y = GetY() - path_objective.y;
	angle = (float)57.29577951 * atan2(direction_vec.y, direction_vec.x);

	if (angle < 0)
		angle += 360;


	if ((0 <= angle &&  angle <= 22.5) || (337.5 <= angle&& angle <= 360))
	{
		this->direction = D_EAST;
		changed = true;
	}

	else if (22.5 <= angle &&  angle <= 67.5)
	{
		this->direction = D_NORTH_EAST;
		changed = true;
	}

	else if (67.5 <= angle &&  angle <= 112.5)
	{
		this->direction = D_NORTH;
		changed = true;
	}

	else if (112.5 <= angle &&  angle <= 157.5)
	{
		this->direction = D_NORTH_WEST;
		changed = true;
	}

	else if (157.5 <= angle &&  angle <= 202.5)
	{
		this->direction = D_WEST;
		changed = true;
	}

	else if (202.5 <= angle &&  angle <= 247.5)
	{
		this->direction = D_SOUTH_WEST;
		changed = true;
	}

	else if (247.5 <= angle &&  angle <= 292.5)
	{
		this->direction = D_SOUTH;
		changed = true;
	}

	else if (292.5 <= angle &&  angle <= 337.5)
	{
		this->direction = D_SOUTH_EAST;
		changed = true;
	}

	else
		this->direction = D_NO_DIRECTION;

	return ret;
}


const int Unit::GetRandNum(int num)
{
	rand_num = rand() % num;
	rand_num++;
	return rand_num;
}