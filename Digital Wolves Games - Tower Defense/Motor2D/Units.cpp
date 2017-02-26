
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Animation.h"
#include "Units.h"
#include "p2Log.h"
#include "j1Pathfinding.h"

Unit::Unit(UNIT_TYPE u_type, iPoint pos): Entity(UNIT, pos), unit_type(u_type), direction(EAST), action_type(IDLE)
{
	switch (u_type)
	{
		//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	case TWOHANDEDSWORDMAN:
		SetHp(60);
		attack = 12;
		SetArmor(1);
		speed = 0.9;
		rate_of_fire = 2;
		range = 1;
		unit_class = INFANTRY;
		break;

	case CAVALRYARCHER:
		SetHp(50);
		attack = 6;
		SetArmor(1);
		speed = 1.4;
		rate_of_fire = 2;
		range = 4;
		unit_class = ARCHER;
		break;

	case SIEGERAM:
		SetHp(270);
		attack = 4;
		SetArmor(-5);
		speed = 0.6;
		rate_of_fire = 5;
		range = 1;
		unit_class = SIEGE;
		break;

	default:
		LOG("Error UNIT TYPE STATS NULL");
		unit_class = NO_CLASS;
		break;
	}
}

void Unit::Update()
{
	AI();
	Move();
	Draw();
}

void Unit::Move()
{
}

void Unit::AI()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		action_type = ATTACK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		action_type = IDLE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		action_type = WALK;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		action_type = DISAPPEAR;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);
		
		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		action_type = DIE;
		Animation* anim = nullptr;
		if (direction == EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, WEST);

		else if (direction == NORTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, NORTH_WEST);

		else if (direction == SOUTH_EAST)
			anim = App->anim->GetAnimation(unit_type, action_type, SOUTH_WEST);

		else
			anim = App->anim->GetAnimation(unit_type, action_type, direction);

		anim->Reset();
	}

}

void Unit::Draw()
{
	SDL_Texture* tex = App->anim->GetTexture(unit_type);
	SDL_Rect rect;
	iPoint pivot;

	App->anim->GetAnimationFrame(rect, pivot, this);

	if (direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
		App->render->Blit(tex, GetX(), GetY(), &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y);
	else
		App->render->Blit(tex, GetX() - pivot.x, GetY() - pivot.y, &rect);
}

const DIRECTION Unit::GetDir() const
{
	return direction;
}

const UNIT_TYPE Unit::GetUnitType() const
{
	return unit_type;
}

const ACTION_TYPE Unit::GetActionType() const
{
	return action_type;
}

void Unit::GetPath(iPoint destination) {
	iPoint p = App->render->ScreenToWorld(GetX(), GetY());
	App->pathfinding->CreatePath(p, destination);

}