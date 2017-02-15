#include <iostream> 
#include <sstream> 

#include "c2Defs.h"
#include "c2Log.h"

#include "d1Window.h"
#include "d1Input.h"
#include "d1Render.h"
#include "d1Textures.h"
#include "d1Audio.h"
#include "d1Scene.h"
#include "d1FileSystem.h"
#include "d1Map.h"
#include "d1Pathfinding.h"
#include "d1Fonts.h"
#include "d1App.h"

// Constructor
d1App::d1App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new d1Input();
	win = new d1Window();
	render = new d1Render();
	tex = new d1Textures();
	audio = new d1Audio();
	scene = new d1Scene();
	fs = new d1FileSystem();
	map = new d1Map();
	pathfinding = new d1PathFinding();
	font = new d1Fonts();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(pathfinding);
	AddModule(font);

	// scene last
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
}

// Destructor
d1App::~d1App()
{
	// release modules
	std::list<d1Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend())
	{
		RELEASE(*item);
		item++;
	}

	modules.clear();
}

void d1App::AddModule(d1Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool d1App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = true;

	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	if(ret == true)
	{
		std::list<d1Module*>::iterator item = modules.begin();

		while(item != modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->name.GetString()));
			item++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool d1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<d1Module*>::iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool d1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node d1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void d1App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void d1App::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		d1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool d1App::PreUpdate()
{
	bool ret = true;
	std::list<d1Module*>::iterator item;
	d1Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool d1App::DoUpdate()
{
	bool ret = true;
	std::list<d1Module*>::iterator item;

	d1Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool d1App::PostUpdate()
{
	bool ret = true;
	std::list<d1Module*>::iterator item;
	d1Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = (*item);

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool d1App::CleanUp()
{
	PERF_START(ptimer);
	bool ret = true;
	std::list<d1Module*>::reverse_iterator item = modules.rbegin();

	while(item != modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int d1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* d1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* d1App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float d1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* d1App::GetOrganization() const
{
	return organization.c_str();
}

// Load / Save
void d1App::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
	char dir[512];
	sprintf_s(dir, "%s%s", fs->GetSaveDirectory(), file);
	load_game.assign(dir);
}

// ---------------------------------------
void d1App::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
	save_game.assign(file);
}

// ---------------------------------------
void d1App::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool d1App::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->Load(load_game.c_str(), &buffer);

	if(size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if(result != NULL)
		{
			LOG("Loading new Game State from %s...", load_game.c_str());

			root = data.child("game_state");

			std::list<d1Module*>::iterator item = modules.begin();
			ret = true;

			while(item != modules.end() && ret == true)
			{
				ret = (*item)->Load(root.child((*item)->name.GetString()));
				item++;
			}

			data.reset();
			if(ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (*item != NULL) ? (*item)->name.GetString() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", load_game.c_str());

	want_to_load = false;
	return ret;
}

bool d1App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.c_str());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	std::list<d1Module*>::const_iterator item = modules.begin();

	while(item != modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->name.GetString()));
		item++;
	}

	if(ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		fs->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", save_game.c_str());
	}
	else
		LOG("Save process halted from an error in module %s", (*item != NULL) ? (*item)->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}