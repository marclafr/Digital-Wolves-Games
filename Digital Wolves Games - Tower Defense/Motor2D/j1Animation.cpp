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
				Animation* new_anim = new Animation(unit_node.name());
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
				animations.push_back(new_anim);

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
	for(int i = 0; i < animations.size(); i++)
	{
		animations[i]->CleanUp();
		delete animations[i];
	}

	animations.clear();

	return true;
}

Animation* j1Animation::GetAnimation(const UNIT_TYPE unit, const ACTION_TYPE action, const DIRECTION direction) const
{
	for (int i = 0; i < animations.size(); i++)
		if (animations[i]->unit_type == unit && animations[i]->action_type == action && animations[i]->direction_type == direction)
			return animations[i];
	return nullptr;
}

bool j1Animation::GetAnimationFrame(SDL_Rect& frame, iPoint& pivot, const Unit* unit)
{
	bool ret = false;
	//direction == NORTH_EAST || direction == EAST || direction == SOUTH_EAST
	DIRECTION direction = unit->GetDir();

	switch (direction)
	{
	case NORTH_EAST:
		direction = NORTH_WEST;
		break;

	case EAST:
		direction = WEST;
		break;

	case SOUTH_EAST:
		direction = SOUTH_WEST;
		break;

	default:
		break;
	}

	Animation* anim = App->anim->GetAnimation(unit->GetUnitType(), unit->GetActionType(), direction);


	if (anim->Finished() == false)
	{
		frame = anim->GetCurrentFrame();
		pivot = anim->GetCurrentPivotPoint();

		if (anim == NULL)
		{
			LOG("ERROR: GetAnimationFrame: animation not found");
			return NULL;
		}
	}
	else
	{
		anim->anim_timer.Start();
		anim->RestartAnim();
		return true;
	}
	return false;
}


//--------------------------------------------------------------------------------------//

Animation::Animation(std::string name): name(name)
{}

// Destructor
Animation::~Animation()
{}

void Animation::SetSpeed(float spd)
{
	speed = spd;
}

void Animation::SetLoopState(bool state)
{
	loop = state;
}

void Animation::RestartAnim()
{
	current_frame = 0.0f;
}

SDL_Rect Animation::GetCurrentFrame()
{
	if (current_frame == -1)
		return SDL_Rect{0,0,0,0};

	if (idle_wait_timer.ReadSec() <= IDLE_ANIMATION_WAIT && this->action_type == IDLE)
	{
		return frames[0];
	}	

	if (idle_wait_timer.ReadSec() >= 2.5f)
		idle_wait_timer.Start();

	current_frame = (float) floor(anim_timer.Read() / speed);
	
	if (Finished())
	{
		if (loop == true)
		{
			anim_timer.Start();
			current_frame = 0.0f;
			loops++;
		}
		else
		{
			current_frame = -1;
			loops = 0;
			return SDL_Rect{ 0,0,0,0 };
		}
	}

	return frames[(int)current_frame];
}

iPoint Animation::GetCurrentPivotPoint()
{
	return pivot_points[(int)current_frame];
}

bool Animation::Finished() const
{
	if (current_frame >= frames.size() - 1)
	{
		return true;
	}

	return false;
}

void Animation::Reset()
{
	current_frame = 0.0f;
	anim_timer.Start();
	loops = 0;
}

void Animation::SetUnit(const pugi::xml_node node)
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

void Animation::SetAction(const pugi::xml_node node)
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

void Animation::SetDirection(const pugi::xml_node node)
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

bool Animation::CleanUp()
{
	frames.clear();
	pivot_points.clear();

	return true;
}
