#include "j1App.h"
#include "j1Animation.h"
#include "Units.h"

void Unit::Update()
{
	AI();
	Move();
	Draw();
}

void Unit::Draw()
{
	SDL_Texture* text = nullptr;
	SDL_Rect* rect;
	iPoint pivot;

	//App->anim->GetAnimationFrame(text, rect, pivot, this);

	if(direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST)
	App->render
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
