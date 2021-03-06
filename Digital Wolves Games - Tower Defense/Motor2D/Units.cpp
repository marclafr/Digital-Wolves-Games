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

Unit::Unit(UNIT_TYPE u_type, fPoint pos, Side side, int priority) : Entity(E_UNIT, pos, side), unit_type(u_type), direction(D_EAST), action(A_IDLE), changed(false), target(nullptr), ai_update(0)
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
	case U_GOD:
		SetHp(1000);
		attack = 100;
		SetArmor(50);
		speed = 15.0f;
		rate_of_fire = 15.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse(GetPosition(), 18);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_HEAVYCAVALRYARCHER);
		else
			SetTextureID(T_HEAVYCAVALRYARCHER);

		priority = 1;
		break;

		//INFANTRY

	case U_MILITIA:
		SetHp(43);
		attack = 6;
		SetArmor(2);
		speed = 1.35f;
		rate_of_fire = 80.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse(GetPosition(), 10);
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
		speed = 1.27f;
		rate_of_fire = 120.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_MANATARMS);
		else
			SetTextureID(T_MANATARMS);
		priority = 1;
		break;

	case U_LONGSWORDMAN:
		SetHp(118);
		attack = 11;
		SetArmor(5);
		speed = 1.28f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_LONGSWORDMAN);
		else
			SetTextureID(T_LONGSWORDMAN);
		priority = 1;
		break;

	case U_TWOHANDEDSWORDMAN:
		SetHp(95);
		attack = 13;
		SetArmor(7);
		speed = 1.30f;
		rate_of_fire = 100.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse(GetPosition(), 10);
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
		unit_circle = Elipse(GetPosition(), 10);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_CHAMPION);
		else
			SetTextureID(T_CHAMPION);
		priority = 1;
		break;

	case U_SPEARMAN:
		SetHp(55);
		attack = 6;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_INFANTRY;
		unit_circle = Elipse(GetPosition(), 10);
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
		unit_circle = Elipse(GetPosition(), 10);
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
		attack = 6;
		SetArmor(1);
		speed = 1.29f;
		rate_of_fire = 95.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse(GetPosition(), 10);
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
		speed = 1.29f;
		rate_of_fire = 80.0f;
		range = MID_COMBAT_RANGE;
		unit_class = C_ARCHER;
		unit_circle = Elipse(GetPosition(), 10);
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
		unit_circle = Elipse(GetPosition(), 18);
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
		unit_circle = Elipse(GetPosition(), 18);
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
		speed = 1.58f;
		rate_of_fire = 110.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		unit_circle = Elipse(GetPosition(), 18);
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
		speed = 1.60f;
		rate_of_fire = 105.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_CAVALRY;
		unit_circle = Elipse(GetPosition(), 18);
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
		unit_circle = Elipse(GetPosition(), 18);
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
		SetArmor(4);
		speed = 0.7f;
		rate_of_fire = 210.0f;
		range = CLOSE_COMBAT_RANGE;
		unit_class = C_SIEGE;
		unit_circle = Elipse(GetPosition(), 30);
		if (side == S_ENEMY)
			SetTextureID(T_ENEMY_SIEGERAM);
		else
			SetTextureID(T_SIEGERAM);
		priority = 1;
		break;

	case U_MANGONEL:
		SetHp(500);
		attack = 67;
		SetArmor(-5);
		speed = 0.7f;
		rate_of_fire = 300.0f;
		range = LONG_COMBAT_RANGE;
		unit_class = C_SIEGE;
		unit_circle = Elipse(GetPosition(), 30);
		SetTextureID(T_MANGONEL);
		priority = 1;
		break;

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
		ChangeAnimation();
		changed = false;
	}

	Draw();
}

bool Unit::Walk()
{
	if (path_objective.DistanceTo(iPoint(GetX(), GetY())) < 3)
		if (!GetNextTile())
			return false;
	SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);
	unit_circle.SetPosition({ GetX(), GetY() });
	return true;
}

void Unit::AI()
{
	ai_update++;

	if (slowed == true && slow_timer.ReadSec() >= SLOW_TIME)
	{
		this->speed *= SLOW_PROPORTION;
		slowed = false;
	}

	//Investigations bonuses
	CheckUnitsBuffs();
	//----------------------

	switch (action)
	{
	case A_IDLE:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		EnemyInSight();

		if (GetSide() == S_ENEMY)
		{
			target = App->scene->townhall;
			GoToEnemy();
			break;
		}

		if (collision == nullptr)
			Collisions();			
		
		break;

	case A_WALK:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}

		if (collision != nullptr)
			if (collision->GetTile() != GetTile())
				collision = nullptr;

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
			if (target != nullptr)
				GoToEnemy();
			else
				GetNewDestination();
			break;
		}

		EnemyInSight();

		if (target != nullptr)
		{
			if (App->pathfinding->IsEmpty(target->GetTile()))
			{
				ChangeDirecctionToEnemy();
				break;
			}
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

	case A_CENTER:

		if (OutOfHP())
		{
			UnitDies();
			break;
		}
		
		if (DestinationFull())
		{
			GoIdle();
			break;
		}

		if (CenterUnit() == true)
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
			if (unit_class == C_SIEGE)
				siege_attacked = false;
			break;
		}

		if (unit_class == C_SIEGE)
		{
			if (animation->GetCurrentFrame() >= GetFrameAttack() && siege_attacked == false)
				DoDamage();
			else if (siege_attacked == true)
				if (animation->Finished())
					siege_attacked = false;
		}
		else
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

	if (unit_class == C_SIEGE && (action == A_WALK || action == A_APPROACH || action == A_CENTER))
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

const iPoint & Unit::GetDestination() const
{
	return destination;
}

const Unit * Unit::GetCollision() const
{
	return collision;
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

	else if(unit_type == U_SIEGERAM)
		App->audio->PlayFx(App->audio->fx_siegeram_hit);

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
		if ((*it) == this)
		{
			SetEntityStatus(ST_NON_SELECTED);
			App->scene->selection.erase(it);
			break;
		}
	
	if (GetSide() == S_ENEMY)
		App->score->EnemyKilled();

	if (unit_type == U_SIEGERAM)
		App->entity_manager->DropUnits(GetX(), GetY());

	changed = true;
	PlayDeathSound();
}

bool Unit::OutOfHP() const
{
	if (GetHp() <= 0)
		return true;
	return false;
}

void Unit::EnemyInSight()
{
	Entity* ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPosition(), GetSide(), this, E_UNIT);
	if (ret == nullptr)
		ret = App->entity_manager->LookForEnemies(VISION_RANGE, GetPosition(), GetSide(), this);

	if (ret != nullptr && ret != target)
	{
		target = ret;
		GoToEnemy();
	}
		
}

void Unit::GoToEnemy()
{
	destination = App->pathfinding->FindClosestEmptyAttackTile(target->GetIPos(), range, this);
	if (destination.y == -1)
	{
		target = nullptr;
		GoIdle();
	}
	else
	{
		if (destination == GetTile())
			GoToTileCenter();
		else
			GoToTile(destination);
	}
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

	else if(unit_type == U_MANGONEL)
		App->projectile_manager->CreateProjectile(GetPosition(), target, attack, 30, 40, 50, P_CANNONBALL);

	else
		target->Damaged(attack);

	PlayAttackSound();

	if (unit_class == C_SIEGE)
		siege_attacked = true;

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
	if (range != CLOSE_COMBAT_RANGE || GetUnitType() == U_SIEGERAM)
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

void Unit::MoveAway()
{
	iPoint new_pos = App->pathfinding->FindNearestEmpty(this);
	if (new_pos.y == -1)
		LOG("CAN NOT FIND EMPTY POS");
	else
		GoToTile(new_pos);
}

void Unit::GetNewDestination()
{
	iPoint new_pos = App->pathfinding->FindNearestWalkableToDestination(this);

	if (new_pos.y == -1)
		LOG("Can't Find new destination");
	else
		GoToTile(destination);
}

void Unit::CheckUnitsBuffs()
{
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
	//-------------
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
	//-------------
}

void Unit::GoToTileCenter()
{
	iPoint tile = GetTile();
	iPoint tile_center = App->map->MapToWorld(tile.x, tile.y);

	action = A_CENTER;
	float delta_x = tile_center.x - GetX();
	float delta_y = tile_center.y - GetY();
	float distance = sqrtf(delta_x * delta_x + delta_y * delta_y);

	float modul = sqrt(move_vector.x*move_vector.x + move_vector.y * move_vector.y);

	move_vector.x = move_vector.x / modul;
	move_vector.y = move_vector.y / modul;
}

bool Unit::CenterUnit()
{
	SetPosition(GetX() + move_vector.x*speed, GetY() + move_vector.y*speed);
	unit_circle.SetPosition(GetPosition());

	iPoint tile = GetTile();
	iPoint center = App->map->MapToWorld(tile.x, tile.y);

	if (GetIPos().DistanceTo(center) <= 3)
		return true;
	return false;
}

void Unit::ChangeAnimation()
{
	if (unit_type == U_GOD)
	{
		if (action == A_CENTER || action == A_APPROACH)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, A_WALK, direction));
		if (action == A_ATTACK)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction), this->rate_of_fire);
		else
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, U_HEAVYCAVALRYARCHER, action, direction));
	}
	else
	{
		if (action == A_CENTER || action == A_APPROACH)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_WALK, direction));
		if (action == A_ATTACK)
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction), this->rate_of_fire);
		else
			animation->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, action, direction));

		if (unit_class == C_SIEGE)
			idle_siege->ChangeAnimation(App->anim->GetAnimationType(ANIM_UNIT, unit_type, A_IDLE, direction));;
	}
}

int Unit::GetFrameAttack()
{
	switch (unit_type)
	{
	case U_MANGONEL:
		return 2;
		break;
	case U_SIEGERAM:
		return 9;
		break;
	default:
		return 0;
		break;
	}
}

void Unit::Collisions()
{
	std::vector<Unit *> collisions;
	App->entity_manager->TileCollisions(this, collisions);

	if (collisions.empty() == false)
	{
		for (std::vector<Unit*>::iterator it = collisions.begin(); it != collisions.end(); ++it)
		{
			if ((*it)->GetAction() == A_IDLE)
			{
				(*it)->collision = this;
				(*it)->MoveAway();
			}
			else
			{
				collision = (*it);
				MoveAway();
			}
		}
	}
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
