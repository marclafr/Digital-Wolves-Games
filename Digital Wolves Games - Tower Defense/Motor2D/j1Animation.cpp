#include "j1Animation.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "p2Log.h"
#include "j1Render.h"

j1Animation::j1Animation()
{
	name.assign("animation");
}

// Destructor
j1Animation::~j1Animation()
{}

bool j1Animation::Start()
{
	return true;
}
bool j1Animation::Awake(pugi::xml_node& config)
{
	bool ret = true;

	std::string anim_folder = "animations/Units_data.xml";	//TODO: change name

	//Load animations data from animations folder
	char* buff = nullptr;
	int size = App->fs->Load(anim_folder.c_str(), &buff);
	pugi::xml_document anim_data;
	pugi::xml_parse_result result = anim_data.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Error loading animations data: %s", result.description());
		return false;
	}

	//Loading units
	pugi::xml_node unit_node = anim_data.child("Sprites").first_child();

	while (unit_node != NULL)
	{
		pugi::xml_node action_node = unit_node.first_child();
		while (action_node != NULL)
		{
			pugi::xml_node direction_node = action_node.first_child();
			while (direction_node != NULL)
			{
				AnimationType* new_anim = new AnimationType(unit_node.name());
				pugi::xml_node sprite_node = direction_node.first_child();
				while (sprite_node != NULL)
				{
					new_anim->frames.push_back({ sprite_node.attribute("x").as_int(),sprite_node.attribute("y").as_int(), sprite_node.attribute("w").as_int(),sprite_node.attribute("h").as_int() });
					
					float pX = sprite_node.attribute("w").as_int() * sprite_node.attribute("pX").as_float();
					float pY = sprite_node.attribute("h").as_int() * sprite_node.attribute("pY").as_float();
					pX = (pX > (floor(pX) + 0.5f)) ? ceil(pX) : floor(pX);
					pY = (pY > (floor(pY) + 0.5f)) ? ceil(pY) : floor(pY);
					new_anim->pivot_points.push_back({ (int)pX, (int)pY });
					
					sprite_node = sprite_node.next_sibling();
				}
				std::string anim_name = unit_node.name();
				new_anim->name = anim_name + "_" + action_node.name() + "_" + direction_node.name();
				new_anim->SetUnit(unit_node);
				new_anim->SetAction(action_node);
				new_anim->SetDirection(direction_node);

				std::string action = action_node.name();
				if (!action.compare("disappear"))
				{
					new_anim->speed = 1000.0f;
					new_anim->loop = false;
				}
				if (!action.compare("die"))
				{
					new_anim->loop = false;
				}
				animation_types.push_back(new_anim);

				direction_node = direction_node.next_sibling();
			}
			action_node = action_node.next_sibling();
		}
		unit_node = unit_node.next_sibling();
	}

	return ret;
}


bool j1Animation::CleanUp()
{
	for(int i = 0; i < animation_types.size(); i++)
	{
		animation_types[i]->CleanUp();
		delete animation_types[i];
	}

	animation_types.clear();

	return true;
}

AnimationType* j1Animation::GetAnimationType(const UNIT_TYPE unit, const ACTION_TYPE action, const DIRECTION direction) const
{
	DIRECTION dir = direction;

	switch (dir)
	{
		case NORTH_EAST:
			dir = NORTH_WEST;
			break;

		case EAST:
			dir = WEST;
			break;

		case SOUTH_EAST:
			dir = SOUTH_WEST;
			break;

		default:
			break;
	}

	for (int i = 0; i < animation_types.size(); i++)
		if (animation_types[i]->unit_type == unit && animation_types[i]->action_type == action && animation_types[i]->direction_type == dir)
			return animation_types[i];
	return nullptr;
}

//--------------------------------------------------------------------------------------//

AnimationType::AnimationType(std::string name): name(name)
{}

// Destructor
AnimationType::~AnimationType()
{}

void AnimationType::SetSpeed(float spd)
{
	speed = spd;
}

void AnimationType::SetLoopState(bool state)
{
	loop = state;
}

const int AnimationType::GetNumFrames() const
{	
	return (frames.size() - 1);
}

const float AnimationType::GetSpeed() const
{
	return speed;
}

const bool AnimationType::GetLoopState() const
{
	return loop;
}

const SDL_Rect AnimationType::GetFrame(int frame_num) const
{
	return frames[frame_num];
}

const iPoint AnimationType::GetPivot(int frame_num) const
{
	return pivot_points[frame_num];
}

const ACTION_TYPE AnimationType::GetActionType() const
{
	return action_type;
}

const UNIT_TYPE AnimationType::GetUnitType() const
{
	return unit_type;
}

const DIRECTION AnimationType::GetDirection() const
{
	return direction_type;
}

Animation::Animation(): anim_type(nullptr), current_frame(0.0f), anim_timer(j1Timer()), idle_wait_timer(j1Timer()), speed(START_SPEED), loop(true), wait_started(false), finished(false)
{}

Animation::Animation(AnimationType * type): anim_type(type), wait_started(false)
{
	loop = type->GetLoopState();
	speed = type->GetSpeed();
	anim_timer.Start();
}

Animation::Animation(const Animation & copy) : anim_type(copy.anim_type), current_frame(copy.current_frame), speed(copy.speed), loop(copy.loop), wait_started(copy.wait_started), finished(copy.finished)
{
	anim_timer.SetTicks(copy.anim_timer.Read());
	idle_wait_timer.SetTicks(copy.idle_wait_timer.Read());
}

Animation::~Animation()
{}

void Animation::ChangeAnimation(AnimationType * type)
{
	anim_type = type;
	loop = type->GetLoopState();
	speed = type->GetSpeed();
	Reset();
}

const Animation Animation::operator = (const Animation & anim)
{
	anim_type = anim.anim_type;
	current_frame = anim.current_frame;
	anim_timer.SetTicks(anim.anim_timer.Read());
	idle_wait_timer.SetTicks(anim.idle_wait_timer.Read());
	wait_started = anim.wait_started;
	speed = anim.speed;
	loop = anim.loop;
	finished = anim.finished;

	return *this;
}

bool Animation::Update(SDL_Rect & rect, iPoint & pivot_point)
{
	rect = anim_type->GetFrame(current_frame);
	pivot_point = anim_type->GetPivot(current_frame);

	if (wait_started == false)
		current_frame = (float)floor(anim_timer.Read() / speed);

	if (finished == true && loop == true)
	{
		Reset();
	}

	return Finished();
}

bool Animation::Finished()
{
	if (current_frame >= anim_type->GetNumFrames())
	{
		if (anim_type->GetActionType() == IDLE)
		{
			if (wait_started == false)
			{
				wait_started = true;
				idle_wait_timer.Start();
			}

			if (idle_wait_timer.ReadSec() >= IDLE_ANIMATION_WAIT)
			{
				finished = true;
				wait_started = false;
				return true;
			}
		}
		else if (anim_type->GetActionType() == DIE)
		{
			ChangeAnimation(App->anim->GetAnimationType(anim_type->GetUnitType(), DISAPPEAR, anim_type->GetDirection()));
		}
		else
		{
			finished = true;
			return true;
		}
	}
	return false;
}

void Animation::Reset()
{
	current_frame = 0.0f;
	anim_timer.Start();
	finished = false;
	wait_started = false;
}

void AnimationType::SetUnit(const pugi::xml_node node)
{
	//ADD UNIT: IF ANY UNIT IS ADDED ADD CODE HERE:
	if (strcmp(node.name(), "twohandedswordman") == 0)
		unit_type = TWOHANDEDSWORDMAN;

	else if (strcmp(node.name(), "cavalryarcher") == 0)
		unit_type = CAVALRYARCHER;

	else if (strcmp(node.name(), "siegeram") == 0)
		unit_type = SIEGERAM;

	else
	{
		unit_type = NO_UNIT;
		LOG("ERROR: XML Node UNIT TYPE does not match");
	}
}

void AnimationType::SetAction(const pugi::xml_node node)
{
	if (strcmp(node.name(), "attack") == 0)
		action_type = ATTACK;

	else if (strcmp(node.name(), "die") == 0)
		action_type = DIE;

	else if (strcmp(node.name(), "disappear") == 0)	//IF PROBLEM CHECK DIsAppEAR SPELLING!!!!!
		action_type = DISAPPEAR;

	else if (strcmp(node.name(), "idle") == 0)
		action_type = IDLE;

	else if (strcmp(node.name(), "walk") == 0)
		action_type = WALK;

	else
	{
		action_type = NO_ACTION;
		LOG("ERROR: XML Node ACTION TYPE does not match");
	}
}

void AnimationType::SetDirection(const pugi::xml_node node)
{
	if (strcmp(node.name(), "north") == 0)
		direction_type = NORTH;

	else if (strcmp(node.name(), "south") == 0)
		direction_type = SOUTH;

	else if (strcmp(node.name(), "south_west") == 0)
		direction_type = SOUTH_WEST;

	else if (strcmp(node.name(), "west") == 0)
		direction_type = WEST;

	else if (strcmp(node.name(), "north_west") == 0)
		direction_type = NORTH_WEST;

	else
	{
		direction_type = NO_DIRECTION;
		LOG("ERROR: XML Node DIRECTION TYPE does not match");
	}

}

bool AnimationType::CleanUp()
{
	frames.clear();
	pivot_points.clear();

	return true;
}
