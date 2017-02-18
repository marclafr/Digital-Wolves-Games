#include "j1Animation.h"
#include "j1Textures.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "p2Log.h"

j1Animation::j1Animation()
{
	name.create("animation");
}

// Destructor
j1Animation::~j1Animation()
{}

bool j1Animation::Start()
{
	twohandedswordman_texture = App->tex->Load("animations/Twohandedswordman_data.png");

	return true;
}
bool j1Animation::Awake(pugi::xml_node& config)
{
	bool ret = true;

	std::string anim_folder = "animations/Twohandedswordman_data.xml";

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
	pugi::xml_node unit_node = anim_data.child("TextureAtlas").child("sprite");

	std::string unit_enum;

	while (unit_node != NULL)
	{
		unit_enum = unit_node.attribute("n").as_string();

		unit_node = unit_node.next_sibling();
	}

	return ret;
}

void j1Animation::SetSpeed(float spd)
{
	speed = spd;
}

void j1Animation::SetLoopState(bool state)
{
	loop = state;
}


void j1Animation::PushBack(const SDL_Rect& rect)
{
	frames.push_back(rect);
}

SDL_Rect& j1Animation::GetCurrentFrame()
{
	current_frame += speed;
	if (current_frame >= last_frame)
	{
		current_frame = (loop) ? 0.0f : last_frame - 1;
		loops++;
	}
	else if (current_frame < 0)
	{
		current_frame = last_frame - 1;
	}
	return frames[(int)current_frame];
}

bool j1Animation::Finished() const
{
	return loops > 0;
}

void j1Animation::Reset()
{
	current_frame = 0.0f;
	loops = 0;
}

bool j1Animation::CleanUp()
{
	frames.clear();
	return true;
}