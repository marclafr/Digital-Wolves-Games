
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

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority): Entity(E_UNIT, pos, side), unit_type(u_type), direction(D_EAST), action(A_IDLE), changed(false), attacking(nullptr), priority(priority)
{
	if (GetSide() == S_ENEMY)
	{
		this->action = A_WALK;
		GoTo(TOWN_HALL);
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

	animation = new Animation(App->anim->GetAnimationType(unit_type, action, direction));
}

Unit::~Unit()
{
	if (animation != nullptr)
		delete animation;
}

void Unit::Update()
{
	AI();

	if (changed == true)
	{
		animation->ChangeAnimation(App->anim->GetAnimationType(unit_type, action, direction));
		changed = false;
	}

	Draw();

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == ST_SELECTED)
	{
		SetHp(0);
	}
}

bool Unit::Move()
{
	this->SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);

	if (path_objective.DistanceTo(iPoint(GetX(), GetY())) < 3)
	{
		//center the unit to the tile
		if (!GetNextTile())
			return false;
	}

	return true;
}

void Unit::AI()
{
	switch (action)
	{
	case A_IDLE:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			changed = true;
			PlayDeathSound();
			break;
		}

		attacking = App->entity_manager->CheckForCombat(iPoint(GetX(), GetY()), range, GetSide());
		if (attacking != nullptr)
		{
			this->action = A_ATTACK;
			this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
			changed = true;
		}

		else if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == ST_SELECTED)
		{
			iPoint objective;
			App->input->GetMousePosition(objective.x, objective.y);
			objective.x -= App->render->camera->GetPosition().x;
			objective.y -= App->render->camera->GetPosition().y;

			GoTo(objective) != false;
		}
		break;

	case A_WALK:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			changed = true;
			PlayDeathSound();
			break;
		}

		attacking = App->entity_manager->CheckForCombat(iPoint(GetX(), GetY()), range, GetSide());
		if (attacking != nullptr)
		{
			this->action = A_ATTACK;
			this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
			changed = true;
		}

		if (Move() == false)
		{
			this->action = A_IDLE;
			changed = true;
		}

		break;

	case A_ATTACK:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			changed = true;
			PlayDeathSound();
			break;
		}

		if (attacking != nullptr)
		{
			if (animation->Finished())
				attacking->Damaged(attack);

			if (attacking->GetHp() <= 0)
			{
				if (GetSide() == S_ENEMY)
				{
					this->action = A_WALK;
					GoTo(TOWN_HALL);
					changed = true;
				}
				else
				{
					this->action = A_IDLE;
					changed = true;
				}
			}
		}

		break;

	case A_DIE:

		if (animation->Finished())
		{
			action = A_DISAPPEAR;
			changed = true;
		}
		break;

	case A_DISAPPEAR:

		if (animation->Finished())
		{
			this->SetToDelete();
		}

		break;
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

const ACTION Unit::GetAction() const
{
	return action;
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
	if (action == A_WALK)
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

void Unit::SetAction(const ACTION action)
{
	this->action = action;
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
		this->action = A_WALK;
		changed = true;
		this->destination.x = destination.x;
		this->destination.y = destination.y;
		return true;
	}
	return false;
}

void Unit::PlayDeathSound() const
{
	int rand_num = rand() % 5;

	switch (rand_num)
	{
		case 0:
			App->audio->PlayFx(fx_twohanded_die01);
			break;
		case 1:
			App->audio->PlayFx(fx_twohanded_die02);
			break;
		case 2:
			App->audio->PlayFx(fx_twohanded_die03);
			break;
		case 3:
			App->audio->PlayFx(fx_twohanded_die04);
			break;
		case 4:
			App->audio->PlayFx(fx_twohanded_die05);
			break;
	}
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