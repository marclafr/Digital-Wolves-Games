
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

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority): Entity(E_UNIT, pos, side), unit_type(u_type), direction(D_EAST), action(A_IDLE), changed(false), attacking(nullptr), target (nullptr), priority(priority)
{
	if (side == S_ENEMY)
	{
		//if the bonuses start in true they are never applied
		bonus_attack = true;
		bonus_defense = true;
	}

	switch (u_type)
	{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case U_TWOHANDEDSWORDMAN:
		SetHp(170);
		attack = 12;
		SetArmor(1);
		speed = 1.0f;
		rate_of_fire = 200.0f;
		range = 30;
		vision_range = 300;
		unit_class = C_INFANTRY;
		unit_radius = 6;
		SetTextureID(T_TWOHANDEDSWORDMAN);
		priority = 3;

		break;

	case U_CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4f;
		rate_of_fire = 200.0f;
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
		speed = 0.6f;
		rate_of_fire = 500.0f;
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
		if (action == A_ATTACK)
			animation->ChangeAnimation(App->anim->GetAnimationType(unit_type, action, direction), this->rate_of_fire);
		else
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
	//Investigations bonuses
		//Bonus attack
	if (bonus_attack == false)
	{
		switch (unit_class)
		{
		case C_INFANTRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 6;
				bonus_attack = true;
			}
			break;
		case C_ARCHER:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 5;
				bonus_attack = true;
			}
			break;
		case C_CAVALRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_ATTACK)) == INV_LVL_UNLOCKED)
			{
				attack += 7;
				bonus_attack = true;
			}
			break;
		}
	}

		//Bonus defense
	if (bonus_defense == false)
	{
		switch (unit_class)
		{
		case C_INFANTRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_INFANTRY_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(4);
				bonus_defense = true;
			}
			break;
		case C_ARCHER:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_ARCHERS_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(3);
				bonus_defense = true;
			}
			break;
		case C_CAVALRY:
			if (App->investigations->GetLevel(App->investigations->GetInvestigation(INV_CAVALRY_DEFENSE)) == INV_LVL_UNLOCKED)
			{
				IncreaseArmor(4);
				bonus_defense = true;
			}
			break;
		}
	}
	//----------------------

	switch (action)
	{
	case A_IDLE:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			this->SetEntityStatus(ST_NON_SELECTED);
			if(GetSide() == S_ENEMY)
				App->entity_manager->EnemyDead();
			changed = true;
			PlayDeathSound();
			break;
		}

		if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == ST_SELECTED && this->GetSide() == S_ALLY)
		{
			iPoint objective;
			App->input->GetMousePosition(objective.x, objective.y);
			objective.x -= App->render->camera->GetPosition().x;
			objective.y -= App->render->camera->GetPosition().y;

			GoTo(objective);
			break;
		}

		if (GetSide() == S_ENEMY)
			GoTo(TOWN_HALL);

		attacking = App->entity_manager->CheckForCombat(iPoint(GetX(), GetY()), range, GetSide());
		if (attacking != nullptr)
		{
			this->action = A_ATTACK;
			this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
			changed = true;
			break;
		}

		target = App->entity_manager->CheckForObjective(iPoint(GetX(), GetY()), vision_range, GetSide());
		if (target != nullptr)
		{
			GoTo(iPoint(target->GetX(), target->GetY()));
		}
		
		break;

	case A_WALK:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			this->SetEntityStatus(ST_NON_SELECTED);
			if (GetSide() == S_ENEMY)
				App->entity_manager->EnemyDead();
			changed = true;
			PlayDeathSound();
			break;
		}

		if (App->input->GetMouseButtonDown(3) == KEY_DOWN && this->GetEntityStatus() == ST_SELECTED && this->GetSide() == S_ALLY)
		{
			iPoint objective;
			App->input->GetMousePosition(objective.x, objective.y);
			objective.x -= App->render->camera->GetPosition().x;
			objective.y -= App->render->camera->GetPosition().y;

			GoTo(objective);
			break;
		}

		attacking = App->entity_manager->CheckForCombat(iPoint(GetX(), GetY()), range, GetSide());
		if (attacking != nullptr)
		{
			this->action = A_ATTACK;
			this->LookAt(iPoint(attacking->GetX(), attacking->GetY()));
			changed = true;
			break;
		}

		if (Move() == false)
		{
			this->action = A_IDLE;
			changed = true;
		}
		
		if (target != App->entity_manager->CheckForObjective(iPoint(GetX(), GetY()), vision_range, GetSide()))
		{
			target = App->entity_manager->CheckForObjective(iPoint(GetX(), GetY()), vision_range, GetSide());	
		}
		if (target != nullptr && animation->Finished())
		{
			ChangeDirection(iPoint(target->GetX(), target->GetY()));
		}

		break;

	case A_ATTACK:

		if (GetHp() <= 0)
		{
			this->action = A_DIE;
			this->SetEntityStatus(ST_NON_SELECTED);
			if (GetSide() == S_ENEMY)
				App->entity_manager->EnemyDead();
			changed = true;
			PlayDeathSound();
			break;
		}

		if (attacking != nullptr)
		{
			if (animation->Finished()) {
				attacking->Damaged(attack);
				PlayAttackSound();
			}
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

bool Unit::GetPath(iPoint dest)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	if(App->pathfinding->CalculatePath(ori, destinat, path_vec) == false)
		return false;
	return true;
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

void Unit::SetAction(const ACTION action)
{
	this->action = action;
}

void Unit::LookAt(iPoint pos)
{
	iPoint direction_vec;
	direction_vec.x = pos.x - GetX();
	direction_vec.y = GetY() - pos.y;
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
}

bool Unit::GoTo( iPoint destination)
{
	if (this->GetPath({ destination.x, destination.y }) != false)
	{
		GetNextTile();
		this->action = A_WALK;
		changed = true;
		this->destination.x = destination.x;
		this->destination.y = destination.y;
		return true;
	}
	return false;
}

bool Unit::ChangeDirection(iPoint destination)
{
	if (this->GetPath(destination) != true)
	{
		GetNextTile();
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
		App->audio->PlayFx(App->entity_manager->fx_twohanded_die01);
		break;
	case 1:
		App->audio->PlayFx(App->entity_manager->fx_twohanded_die02);
		break;
	case 2:
		App->audio->PlayFx(App->entity_manager->fx_twohanded_die03);
		break;
	case 3:
		App->audio->PlayFx(App->entity_manager->fx_twohanded_die04);
		break;
	case 4:
		App->audio->PlayFx(App->entity_manager->fx_twohanded_die05);
		break;
	}	
}

bool Unit::GetNextTile()
{
	bool ret = true;

	if (path_vec.size() <= 0)
		return false;

	path_objective = App->map->MapToWorld(path_vec.back().x, path_vec.back().y);
	path_vec.pop_back();

	move_vector.x = (float)path_objective.x - GetX();
	move_vector.y = (float)path_objective.y - GetY();

	float modul = (sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y));

	move_vector.x = move_vector.x / modul;
	move_vector.y = move_vector.y / modul;

	LookAt(path_objective);

	return ret;
}

void Unit::PlayAttackSound() const
{
	int rand_num = rand() % 3;

	switch (rand_num)
	{
	case 0:
		App->audio->PlayFx(App->entity_manager->fx_attack01);
		break;
	case 1:
		App->audio->PlayFx(App->entity_manager->fx_attack02);
		break;
	case 2:
		App->audio->PlayFx(App->entity_manager->fx_attack03);
		break;
	}
}