
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
#include "ProjectileManager.h"
#include "j1Score.h"
#include "j1Scene.h"

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority) : Entity(E_UNIT, pos, side), unit_type(u_type), direction(D_EAST), action(A_IDLE), changed(false), target(nullptr), priority(priority)
{
	//Add paths
	

	if (side == S_ENEMY)
	{
		//if the bonuses start in true they are never applied
		bonus_attack = true;
		bonus_defense = true;
	}
	else
		App->pathfinding->AddPath(&path_vec);

	switch (u_type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
		//TODO: ALL UNITS VALUES MUST BE CHANGED
		//TODO: CHANGE/QUIT PRIORITY

	case U_GOD:
		SetHp(1000);
		attack = 100;
		SetArmor(50);
		speed = 15.0f;
		rate_of_fire = 15.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_HEAVYCAVALRYARCHER);
		else
			SetTextureID(T_HEAVYCAVALRYARCHER);

		priority = 1;
		break;

		//INFANTRY

	case U_MILITIA:
		SetHp(43);
		attack = 5;
		SetArmor(1);
		speed = 1.42f;
		rate_of_fire = 80.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_MILITIA);
		else
			SetTextureID(T_MILITIA);

		priority = 1;
		break;

	case U_MANATARMS:
		SetHp(58);
		attack = 7;
		SetArmor(4);
		speed = 1.28f;
		rate_of_fire = 120.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_MANATARMS);
		else
			SetTextureID(T_MANATARMS);
		priority = 1;
		break;

	case U_LONGSWORDMAN:
		SetHp(118);
		attack = 10;
		SetArmor(5);
		speed = 1.30f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_LONGSWORDMAN);
		else
			SetTextureID(T_LONGSWORDMAN);
		priority = 1;
		break;

	case U_TWOHANDEDSWORDMAN:
		SetHp(95);
		attack = 0.5;// 13;
		SetArmor(7);
		speed = 1.30f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_TWOHANDEDSWORDMAN);
		else
			SetTextureID(T_TWOHANDEDSWORDMAN);
		priority = 3;
		break;

	case U_CHAMPION:
		SetHp(152);
		attack = 14;
		SetArmor(8);
		speed = 1.30f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CHAMPION);
		else
			SetTextureID(T_CHAMPION);
		priority = 1;
		break;

	case U_SPEARMAN:
		SetHp(55);
		attack = 4;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_SPEARMAN);
		else
			SetTextureID(T_SPEARMAN);
		priority = 1;
		break;

	case U_PIKEMAN:
		SetHp(65);
		attack = 5;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_PIKEMAN);
		else
			SetTextureID(T_PIKEMAN);
		priority = 1;
		break;

		//---
		//ARCHERS

	case U_ARCHER:
		SetHp(41);
		attack = 5;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_ARCHER);
		else
			SetTextureID(T_ARCHER);
		priority = 1;
		break;


	case U_ARBALEST:
		SetHp(50);
		attack = 7;
		SetArmor(1);
		speed = 1.19f;
		rate_of_fire = 80.0f;
		range = MID_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse({ GetX(), GetY() }, 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_ARBALEST);
		else
			SetTextureID(T_ARBALEST);
		priority = 1;
		break;

	case U_CAVALRYARCHER:
		SetHp(65);
		attack = 6;
		SetArmor(2);
		speed = 1.55f;
		rate_of_fire = 85.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CAVALRYARCHER);
		else
			SetTextureID(T_CAVALRYARCHER);
		priority = 2;
		break;

	case U_HEAVYCAVALRYARCHER:
		SetHp(81);
		attack = 8;
		SetArmor(4);
		speed = 1.55f;
		rate_of_fire = 120.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_HEAVYCAVALRYARCHER);
		else
			SetTextureID(T_HEAVYCAVALRYARCHER);
		priority = 2;
		break;

		//---
		//CAVALRY

	case U_PALADIN:
		SetHp(167);
		attack = 16;
		SetArmor(6);
		speed = 1.60f;
		rate_of_fire = 110.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_PALADIN);
		else
			SetTextureID(T_PALADIN);
		priority = 2;
		break;

	case U_KNIGHT:
		SetHp(137);
		attack = 12;
		SetArmor(5);
		speed = 1.62f;
		rate_of_fire = 105.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_KNIGHT);
		else
			SetTextureID(T_KNIGHT);
		priority = 2;
		break;

	case U_CAVALIER:
		SetHp(125);
		attack = 13;
		SetArmor(3);
		speed = 1.55f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		unit_circle = Elipse({ GetX(), GetY() }, 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CAVALIER);
		else
			SetTextureID(T_CAVALIER);
		priority = 2;
		break;

		//---
		//SIEGE

	case U_SIEGERAM:
		SetHp(600);
		attack = 110;
		SetArmor(3);
		speed = 0.7f;
		rate_of_fire = 210.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_SIEGE;
		unit_circle = Elipse({ GetX(), GetY() }, 30);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_SIEGERAM);
		else
			SetTextureID(T_SIEGERAM);
		priority = 1;
		break;

	case U_MANGONEL:
		SetHp(500);
		attack = 80;
		SetArmor(-1);
		speed = 0.8f;
		rate_of_fire = 250.0f;
		range = MID_COMBAT_RANGE;
		unit_class = C_SIEGE;
		unit_circle = Elipse({ GetX(), GetY() }, 30);
		SetTextureID(T_MANGONEL);
		priority = 1;
		break;

		//TODO: ADD UNIT

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = C_NO_CLASS;
		break;
	}

	if (unit_type == U_GOD)
		animation = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction));
	else
		animation = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction));

	if (unit_class == C_SIEGE)
		idle_siege = new AnimationManager(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_IDLE, direction));

}

Unit::~Unit()
{
	if (animation != nullptr)
		delete animation;
}

void Unit::Update(float dt)
{	
	AI();

	if (changed == true)
	{
		if (unit_type == U_GOD)
		{
			if (action == A_ATTACK)
				animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction), this->rate_of_fire);
			else
				animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction));
		}

		else
		{
			if (action == A_ATTACK)
				animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction), this->rate_of_fire);
			else
				animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction));

			if (unit_class == C_SIEGE)
				idle_siege->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_IDLE, direction));;
		}

		changed = false;
	}

	Draw();

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && GetEntityStatus() == ST_SELECTED && GetSide() == S_ALLY)
	{
		SetHp(0);
	}
}

bool Unit::Walk()
{
	if (target != nullptr)
		GoToEnemy();

	if (path_objective.DistanceTo(iPoint(GetX(), GetY())) < 3)
		if (!GetNextTile())
			return false;
	SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);
	unit_circle.SetPosition({ GetX(), GetY() });
	return true;
}

void Unit::AI()
{
	if (slowed == true && slow_timer.ReadSec() >= SLOW_TIME)
	{
		this->speed *= SLOW_PROPORTION;
		slowed = false;
	}

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
	Unit* collision = nullptr;

	switch (action)
	{
	case A_IDLE:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		target = EnemyInSight();
		if (target != nullptr)
		{
			GoToEnemy();
			break;
		}
		
		/*if (GetSide() == S_ENEMY)
		{
			target = App->scene->townhall;
			GoToEnemy();
			break;
		}*/

		/*collision = App->entity_manager->CheckUnitCollisions(this);
		if (collision != nullptr && collided == false)
		{
			new_pos = App->pathfinding->FindNearestEmpty(App->map->WorldToMap(GetX(), GetY()));
			if (new_pos.y != -1)
				GoTo(App->map->MapToWorld(new_pos.x, new_pos.y));	
			collided = true;
			collision->collided = true;
			collision = nullptr;
		}

		if (collision == nullptr)
			collided = false;*/
		
		break;

	case A_WALK:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (target != nullptr && EnemyDead())
		{
			GoIdle();
			break;
		}
		
		if (Walk() == false)
		{
			if (target == nullptr)
			{
				GoIdle();
				break;
			}
			else
			{
				SetAttackPosition();
				break;
			}
		}
		
		if (DestinationFull())
		{
			GoIdle();
			break;
		}

		target = EnemyInSight();
		if (target != nullptr)
		{
			ChangeDirecctionToEnemy();
			break;
		}

		break;

	case A_APPROACH:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (EnemyDead())
		{
			GoIdle();
			break;
		}

		if (AproachEnemy() == true)
			StartAttack();

		break;

	case A_ATTACK:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (EnemyDead())
		{
			GoIdle();
			break;
		}

		if (animation->Finished())
			DoDamage();

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
			Die();
		break;
	}
}

void Unit::Draw()
{	
	SDL_Rect rect;
	iPoint pivot;

	if (unit_class == C_SIEGE && action == A_WALK)
	{	
		idle_siege->Update(rect, pivot);

		SetPivot(pivot.x, pivot.y);
		SetRect(rect);
		if (direction == D_NORTH_EAST || direction == D_EAST || direction == D_SOUTH_EAST)
			App->render->PushInGameSprite(App->tex->GetTexture(GetTextureID()), GetX(), GetY(), &GetRect(), SDL_FLIP_HORIZONTAL, GetPivot().x, GetPivot().y);
		else
			App->render->PushInGameSprite(App->tex->GetTexture(GetTextureID()), GetX(), GetY(), &GetRect(), SDL_FLIP_NONE, GetPivot().x, GetPivot().y);
	}

	animation->Update(rect, pivot);
	
	SetPivot(pivot.x, pivot.y);
	SetRect(rect);

	App->render->PushInGameSprite(this);

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
	return unit_circle.GetRad();
}

bool Unit::GetPath(iPoint dest)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	iPoint destinat = App->map->WorldToMap(dest.x, dest.y);
	if(App->pathfinding->CalculatePath(ori, destinat, path_vec) == false)
		return false;
	return true;
}

bool Unit::GetTilePath(iPoint & tile)
{
	iPoint ori = App->map->WorldToMap(GetX(), GetY());
	if (App->pathfinding->CalculatePath(ori, tile, path_vec) == false)
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

const Elipse Unit::GetUnitCircle() const
{
	return unit_circle;
}

const bool Unit::IsMoving() const
{
	if (action == A_WALK)
		return true;
	return false;
}

void Unit::CheckCollisions() const
{
	App->entity_manager->CheckUnitCollisions(this);
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

bool Unit::GoTo( iPoint dest)
{
	if (GetPath(dest) != false)
	{
		GetNextTile();
		action = A_WALK;
		changed = true;
		destination = App->map->WorldToMap(dest.x, dest.y);
		return true;
	}
	return false;
}

bool Unit::GoToTile(iPoint & tile)
{
	if (GetTilePath(tile) != false)
	{
		GetNextTile();
		action = A_WALK;
		changed = true;
		destination = tile;
		return true;
	}
	return false;
}

bool Unit::ChangeDirection(iPoint dest)
{
	if (GetPath(dest) != true)
	{
		GetNextTile();
		destination = App->map->WorldToMap(dest.x, dest.y);
		return true;
	}
	return false;
}

bool Unit::ChangeDirectionTile(iPoint & tile)
{
	if (GetTilePath(tile) != true)
	{
		GetNextTile();
		destination = tile;
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
		App->audio->PlayFx(App->audio->fx_twohanded_die01);
		break;
	case 1:
		App->audio->PlayFx(App->audio->fx_twohanded_die02);
		break;
	case 2:
		App->audio->PlayFx(App->audio->fx_twohanded_die03);
		break;
	case 3:
		App->audio->PlayFx(App->audio->fx_twohanded_die04);
		break;
	case 4:
		App->audio->PlayFx(App->audio->fx_twohanded_die05);
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
	if(unit_class == C_ARCHER)
		App->audio->PlayFx(App->audio->fx_arrow);

	else
	{
		int rand_num = rand() % 3;

		switch (rand_num)
		{
		case 0:
			App->audio->PlayFx(App->audio->fx_attack01);
			break;
		case 1:
			App->audio->PlayFx(App->audio->fx_attack02);
			break;
		case 2:
			App->audio->PlayFx(App->audio->fx_attack03);
			break;
		}
	}
}

void Unit::UnitDies()
{
	action = A_DIE;

	for (std::vector<Entity*>::iterator it = App->scene->selection.begin(); it != App->scene->selection.end(); ++it)
		if ((*it) == ((Entity*)this))
		{
			SetEntityStatus(ST_NON_SELECTED);
			App->scene->selection.erase(it);
			break;
		}
	
	if (GetSide() == S_ENEMY)
		App->score->EnemyKilled();
	changed = true;
	PlayDeathSound();
}

bool Unit::OutOfHP() const
{
	if (GetHp() <= 0)
		return true;
	return false;
}

Entity * Unit::EnemyInSight()
{
	Entity* ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPosition(), GetSide(), this, E_UNIT);
	if (ret == nullptr)
		ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPosition(), GetSide(), this);
	return ret;
}

void Unit::GoToEnemy()
{
	destination = App->pathfinding->FindClosestEmptyAttackTile(target->GetIPos(), range, this);
	if (destination.y == -1)
		target = nullptr;
	else
		GoToTile(destination);
}

void Unit::ChangeDirecctionToEnemy()
{
	iPoint destination_tile;
	destination_tile = App->pathfinding->FindClosestEmptyAttackTile(target->GetIPos(), range, this);
	if (destination_tile.y == -1)
		target = nullptr;
	else
		ChangeDirectionTile(destination_tile);
}

void Unit::GoIdle()
{
	action = A_IDLE;
	changed = true;
}

bool Unit::DestinationFull() const
{
	if (App->pathfinding->IsEmpty(destination, (Entity*) this))
		return false;
	return true;
}

bool Unit::EnemyDead()
{
	if (target->GetHp() <= 0.0f)
	{
		target = nullptr;
		return true;
	}
	return false;
}

void Unit::DoDamage()
{
	LookAt(target->GetIPos());

	if (unit_class == C_ARCHER)
	{
		if (unit_type == U_GOD)
			App->projectile_manager->CreateProjectile(GetPosition(), target, attack, 5, 30, 0, P_ICE_ARROW);
		else
			App->projectile_manager->CreateProjectile(GetPosition(), target, attack, 15, 20, 0, P_BASIC_ARROW);
	}
	else
		target->Damaged(attack);

	PlayAttackSound();

	if (destination != App->map->WorldToMap(GetX(), GetY()))
		LOG("NOT EQUAL!!!!!!!");
}

bool Unit::AproachEnemy()
{
	SetAttackPosition();

	SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);
	unit_circle.SetPosition({ GetX(), GetY() });

	if (GetPosition().DistanceTo(target->GetPosition()) <= APPROACH)
		return true;
	return false;
}

void Unit::SetAttackPosition()
{
	if (range != CLOSE_COMBAT_RANGE)
		StartAttack();
	else
	{
		action = A_APPROACH;
		float delta_x = target->GetX() - GetX();
		float delta_y = target->GetY() - GetY();
		float distance = sqrtf(delta_x * delta_x + delta_y * delta_y);

		move_vector.x = delta_x * (APPROACH / distance);
		move_vector.y = delta_y * (APPROACH / distance);

		float modul = sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y);

		move_vector.x = move_vector.x / modul;
		move_vector.y = move_vector.y / modul;
	}
}

void Unit::StartAttack()
{
	action = A_ATTACK;
	LookAt(target->GetIPos());
	changed = true;
}

void Unit::SlowUnit()
{
	if (slowed == false)
	{
		this->speed /= SLOW_PROPORTION;
		slowed = true;
		slow_timer.Start();
	}
}

void Unit::Save(pugi::xml_node &data)
{
	pugi::xml_node build = data.child("units");
	pugi::xml_node ActualUnit = build.append_child("unit");
	ActualUnit.append_attribute("unit_type") = GetUnitType();
	ActualUnit.append_attribute("posx") = GetX();
	ActualUnit.append_attribute("posy") = GetY();
	ActualUnit.append_attribute("hp") = GetHp();
	ActualUnit.append_attribute("side") = GetSide();
}
