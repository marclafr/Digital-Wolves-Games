#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Console.h"
#include "j1Scene.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name.assign("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}
	

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:

				mouse_buttons[event.button.button - 1] = KEY_DOWN;
				if(App->scene->active)
					App->scene->HandleInput(event);
			break;

			case SDL_MOUSEBUTTONUP:

				mouse_buttons[event.button.button - 1] = KEY_UP;
				if (App->scene->active)
					App->scene->HandleInput(event);
			break;

			case SDL_KEYDOWN:

				if (App->scene->active)
					App->scene->HandleInput(event);

				break;

			case SDL_KEYUP:
				if (App->scene->active)
					App->scene->HandleInput(event);
				break;

			case SDL_MOUSEMOTION:
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;
			break;
		}
	}
	return true;
}

bool j1Input::Update(float dt)
{
	//TODO: DEBUG FEATURES
	if (GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->debug_features.debug_mode = !App->debug_features.debug_mode;

	if (GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->console->TurnOnOff();

	if (GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		ShellExecute(NULL, "open", "https://twitter.com/DigitalWolvesG", NULL, NULL, SW_SHOWMAXIMIZED);

	if (GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		ShellExecute(NULL, "open", "https://www.facebook.com/Digital-Wolves-Games-233798633695568/?ref=bookmarks", NULL, NULL, SW_SHOWMAXIMIZED);

	if (GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
		ShellExecute(NULL, "open", "https://github.com/marclafr/Digital-Wolves-Games/wiki", NULL, NULL, SW_SHOWMAXIMIZED);

	if (App->debug_features.debug_mode == true)
	{
		if (GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			App->debug_features.add_food = true;

		if (GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			App->debug_features.add_wood = true;

		if (GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
			App->debug_features.add_gold = true;

		if (GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
			App->debug_features.add_stone = true;

		if (GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			App->debug_features.print_walkability_map = !App->debug_features.print_walkability_map;

		if (GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
			if(App->debug_features.pf == true)
				App->debug_features.pf = false;
			else
				App->debug_features.pf = true;
	}
	//
	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}