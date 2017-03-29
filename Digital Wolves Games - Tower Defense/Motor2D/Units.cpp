
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

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side): Entity(UNIT, pos, side), unit_type(u_type), direction(EAST), action_type(IDLE), changed(false), fighting(false), attacking(nullptr)
{
	if (GetSide() == ENEMY)
	{
		this->action_type = WALK;
		GoTo(TOWN_HALL);
		moving = true;
		changed = true;
	}
		
	switch (u_type)
	{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TWOHANDEDSWORDMAN:
		SetHp(60);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 30;
		unit_class = INFANTRY;
		unit_radius = 6;
		SetTextureID(T_TWOHANDEDSWORDMAN);
		fx_twohanded_die01 = App->audio->LoadFx("audio/fx/Male_Death01.wav");
		fx_twohanded_die02 = App->audio->LoadFx("audio/fx/Male_Death02.wav");
		fx_twohanded_die03 = App->audio->LoadFx("audio/fx/Male_Death03.wav");
		fx_twohanded_die04 = App->audio->LoadFx("audio/fx/Male_Death04.wav");
		fx_twohanded_die05 = App->audio->LoadFx("audio/fx/Male_Death05.wav");

		break;

	case CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4;
		rate_of_fire = 2;
		range = 4;
		unit_class = ARCHER;
		unit_radius = 12;
		SetTextureID(T_CAVALRYARCHER);
		break;

	case SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 1;
		unit_class = SIEGE;
		unit_radius = 15;
		SetTextureID(T_SIEGERAM);
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
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
	if (this->GetHp() >= 0)
	{
		if (changed == true)
		{
			animation->ChangeAnimation(App->anim->GetAnimationType(unit_type, action_type, direction));
			changed = false;
		}

		AI();
		Move();
		Draw();

		//THIS DOES NOT BELONG HERE
		//TODO
		if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == E_SELECTED)
		{

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

			action_type = DIE;
			animation->ChangeAnimation(App->anim->GetAnimationType(unit_type, action_type, direction));
		}
	}
}

void Unit::Move()
{
	if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == E_SELECTED)
	{
		this->path_list.clear();

		App->input->GetMousePosition(destination.x, destination.y);
		destination.x -= App->render->camera->GetPosition().x;
		destination.y -= App->render->camera->GetPosition().y;

		if(GoTo(destination) == false)
		{
			this->moving = false;
			this->action_type = IDLE;
			changed = true;
		}
	}

	if (this->moving == true)
	{
		this->SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);

		if (path_objective.DistanceTo(iPoint(GetX(), GetY())) < 3)
		{
			//center the unit to the tile
			if (!GetNextTile())
			{
				moving = false;
				this->action_type = IDLE;
				changed = true;
			}
		}
	}
}

void Unit::AI()
{
	if (fighting == false)
	{
		attacking = App->entity_manager->CheckForEnemies(iPoint(GetX(), GetY()), GetRange(), GetSide());

		if (attacking != nullptr)
		{
			fighting = true;
			moving = false;
			this->action_type = ATTACK;
			this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
			changed = true;
		}
	}

	if (fighting)
	{
		if (animation->Finished())
			attacking->Damaged(attack);

		if (attacking->GetHp() <= 0)
		{
			fighting = false;

			if (GetSide() == ENEMY)
			{
				this->action_type = WALK;
				GoTo(TOWN_HALL);
				moving = true;
				changed = true;
			}
			else
			{
				this->action_type = IDLE;
				changed = true;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		action_type = ATTACK;
		changed = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		action_type = IDLE;
		changed = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		action_type = WALK;
		changed = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		action_type = DIE;
		changed = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		SetHp(0);
	}

	if (GetHp() <= 0)
		Die();
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
	if (changed == false && action_type != DIE && action_type != DISAPPEAR)
	{
		action_type = DIE;
		changed = true;
	}

	if (animation->Finished())
		//TODO
		App->entity_manager->DeleteUnit(this);
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

const int Unit::GetUnitRadius() const
{
	return unit_radius;
}

int Unit::GetPath(iPoint dest) {
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

const bool Unit::IsMoving() const
{
	return moving;
}

void Unit::LookAt(iPoint pos)
{

}

bool Unit::GoTo( iPoint destination)
{
	if (this->GetPath({ destination.x, destination.y }) != -1)
	{
		path_list.pop_front();
		GetNextTile();
		this->action_type = WALK;
		changed = true;
		this->moving = true;
		return true;
	}
	return false;
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (path_list.size() == 0)
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
		this->direction = EAST;
		changed = true;
	}

	else if (22.5 <= angle &&  angle <= 67.5)
	{
		this->direction = NORTH_EAST;
		changed = true;
	}

	else if (67.5 <= angle &&  angle <= 112.5)
	{
		this->direction = NORTH;
		changed = true;
	}

	else if (112.5 <= angle &&  angle <= 157.5)
	{
		this->direction = NORTH_WEST;
		changed = true;
	}

	else if (157.5 <= angle &&  angle <= 202.5)
	{
		this->direction = WEST;
		changed = true;
	}

	else if (202.5 <= angle &&  angle <= 247.5)
	{
		this->direction = SOUTH_WEST;
		changed = true;
	}

	else if (247.5 <= angle &&  angle <= 292.5)
	{
		this->direction = SOUTH;
		changed = true;
	}

	else if (292.5 <= angle &&  angle <= 337.5)
	{
		this->direction = SOUTH_EAST;
		changed = true;
	}

	else
		this->direction = NO_DIRECTION;

	return ret;
}


const int Unit::GetRandNum(int num)
{
	rand_num = rand() % num;
	rand_num++;
	return rand_num;
}