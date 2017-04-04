#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Map.h"
#include "Entity.h"
#include "j1Animation.h"
#include "Camera.h"

#define VSYNC true

j1Render::j1Render() : j1Module(), camera(nullptr)
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera = new Camera(SDL_Rect{0, 0, App->win->screen_surface->w, App->win->screen_surface->h});
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	camera->UpdateCamera();
	return true;
}

bool j1Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	delete camera; 

	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera->SetPosition(iPoint(data.child("camera").attribute("x").as_int(), data.child("camera").attribute("y").as_int()));
	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera->GetPosition().x;
	cam.append_attribute("y") = camera->GetPosition().y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::GetDimensionsFromTexture(SDL_Texture* tex, int& w, int& h)
{
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
}


void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera->GetPosition().x / scale);
	ret.y = (y - camera->GetPosition().y / scale);

	return ret;
}

fPoint j1Render::ScreenToWorldf(int x, int y) const
{
	fPoint ret;
	int scale = App->win->GetScale();

	ret.x = float(x - camera->GetPosition().x / scale);
	ret.y = float(y - camera->GetPosition().y / scale);

	return ret;
}
iPoint j1Render::WorldToScreen(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x + camera->GetPosition().x / scale);
	ret.y = (y + camera->GetPosition().y / scale);

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle, bool not_in_world) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera->GetPosition().x * speed) + x * scale;
	rect.y = (int)(camera->GetPosition().y * speed) + y * scale;

	iPoint screen_position = App->render->WorldToScreen(x,y);

	if (App->render->camera->InsideRenderTarget(screen_position.x, screen_position.y) || not_in_world)
	{
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}

		if (flip == SDL_FLIP_HORIZONTAL)
		{
			screen_position.x -= (rect.w - pivot_x);
			screen_position.y -= pivot_y;
		}

		else if (flip == SDL_FLIP_VERTICAL)
		{
			screen_position.x -= pivot_x;
			screen_position.y -= (rect.h - pivot_y);
		}

		else if (flip == SDL_FLIP_NONE)
		{
			screen_position.x -= pivot_x;
			screen_position.y -= pivot_y;
		}

		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivot_x != INT_MAX && pivot_y != INT_MAX)
		{
			pivot.x = pivot_x;
			pivot.y = pivot_y;
			p = &pivot;
		}
	
		if (not_in_world == false)
		{
			rect = App->render->camera->GetZoomedRect(SDL_Rect{ screen_position.x,screen_position.y,rect.w,rect.h });
			SDL_SetTextureAlphaMod(texture, App->render->camera->GetOpacity());
		}

		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, (SDL_RendererFlip)flip) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera->GetPosition().x + rect.x * scale);
		rec.y = (int)(camera->GetPosition().y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera->GetPosition().x + x1 * scale, camera->GetPosition().y + y1 * scale, camera->GetPosition().x + x2 * scale, camera->GetPosition().y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void j1Render::PushEntity(Entity* entity)
{
	std::deque<Entity*>::iterator queue_pos = sprite_queue.begin();
	int i = 0;

	while (i < sprite_queue.size())
	{		
		if (entity->GetY() <= sprite_queue[i]->GetY())
			break;
		i++;
	}

	sprite_queue.insert(queue_pos + i, entity);
}

void j1Render::BlitAllEntities()
{
	Entity* sp = nullptr;
	Unit* u_sp = nullptr;

	for (int i = 0; i < sprite_queue.size(); i++)
	{
		sp = sprite_queue[i];

		if (sp->GetEntityType() == E_UNIT)
		{
			u_sp = (Unit*)sp;

			if (u_sp->GetDir() == D_NORTH_EAST || u_sp->GetDir() == D_EAST || u_sp->GetDir() == D_SOUTH_EAST)
				Blit(App->tex->GetTexture(sp->GetTextureID()), sp->GetX(), sp->GetY(), &sp->GetRect(), SDL_FLIP_HORIZONTAL, sp->GetPivot().x, sp->GetPivot().y);
			else
				Blit(App->tex->GetTexture(sp->GetTextureID()), sp->GetX() - sp->GetPivot().x, sp->GetY() - sp->GetPivot().y, &sp->GetRect());

			//selected should change cus after sprite order implementation it gets printed before the acual unit sprite 
			
			if (u_sp->GetEntityStatus() == ST_SELECTED)
				App->render->DrawCircle(u_sp->GetX() + camera->GetPosition().x, u_sp->GetY() + camera->GetPosition().y, u_sp->GetUnitRadius(), 255, 255, 255);

		}
		else
		{
			Blit(App->tex->GetTexture(sp->GetTextureID()), sp->GetX(), sp->GetY(), &sp->GetRect(), SDL_FLIP_NONE, sp->GetPivot().x, sp->GetPivot().y);
		}		
	}

	sprite_queue.clear();
}