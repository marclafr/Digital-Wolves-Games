#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Map.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
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
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
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
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

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

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
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
		rect.x -= (rect.w - pivot_x);
		rect.y -= pivot_y;
	}

	else if (flip == SDL_FLIP_VERTICAL)
	{
		rect.x -= pivot_x;
		rect.y -= (rect.h - pivot_y);
	}

	else if (flip == SDL_FLIP_NONE)
	{
		rect.x -= pivot_x;
		rect.y -= pivot_y;
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, (SDL_RendererFlip)flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
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
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
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
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
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

void j1Render::PushSprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle)
{
	Sprite sprite(texture,x,y,section,flip,pivot_x,pivot_y,speed,angle);
	
	std::deque<Sprite>::iterator queue_pos = sprite_queue.begin();

	for (int i = 0; i < sprite_queue.size(); i++)
	{
		if (y > sprite_queue[i].GetPosition().y)
			queue_pos++;
		else
			break;
	}

	//sprite_queue.push_back(sprite);
	if (queue_pos == sprite_queue.begin())
		sprite_queue.push_back(sprite);
	else
		sprite_queue.insert(queue_pos, sprite);	
}

void j1Render::BlitAllEntities()
{
	for(int i = 0; i < sprite_queue.size(); i++)
		Blit(sprite_queue[i].GetTexture(), sprite_queue[i].GetPosition().x, sprite_queue[i].GetPosition().y, sprite_queue[i].GetSection(), sprite_queue[i].GetFlip(), sprite_queue[i].GetPivotX(), sprite_queue[i].GetPivotY(), sprite_queue[i].GetSpeed(), sprite_queue[i].GetAngle());

	sprite_queue.clear();
}

Sprite::Sprite(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_RendererFlip flip, int pivot_x, int pivot_y, float speed, double angle) : texture(texture), position(iPoint(x, y)), flip(flip), pivot_x(pivot_x), pivot_y(pivot_y), speed(speed), angle(angle)
{
	if (section != nullptr)
	{
		this->section = new SDL_Rect;
		this->section->x = section->x;
		this->section->y = section->y;
		this->section->w = section->w;
		this->section->h = section->h;
	}
}

Sprite::Sprite() : section(nullptr), position(iPoint(0,0)), flip(SDL_FLIP_NONE), pivot_x(0), pivot_y(0), speed(1.0f), angle(0)
{}

Sprite::Sprite(const Sprite & copy) : texture(copy.texture), position(copy.position), flip(copy.flip), pivot_x(copy.pivot_x), pivot_y(copy.pivot_y), speed(copy.speed), angle(copy.angle)
{
	if (copy.section != nullptr)
	{
		this->section = new SDL_Rect;
		this->section->x = copy.section->x;
		this->section->y = copy.section->y;
		this->section->w = copy.section->w;
		this->section->h = copy.section->h;
	}
	else
	{
		this->section = nullptr;
	}
}

Sprite::~Sprite()
{
	if (section != nullptr)
	{
		delete section;
		section = nullptr;
	}
}

void Sprite::operator = (Sprite &sprite)
{
	texture = sprite.texture;
	position.x = sprite.position.x;
	position.y = sprite.position.y;

	if (sprite.section != nullptr)
	{
		if(this->section == nullptr)
			this->section = new SDL_Rect;

		this->section->x = section->x;
		this->section->y = section->y;
		this->section->w = section->w;
		this->section->h = section->h;
	}

	flip = sprite.flip;
	pivot_x = sprite.pivot_x;
	pivot_y = sprite.pivot_y;
	speed = sprite.speed;
	angle = sprite.angle;
}

iPoint Sprite::GetPosition() const
{
	return position;
}

SDL_Texture * Sprite::GetTexture() const
{
	return texture;
}

const SDL_Rect * Sprite::GetSection() const
{
	return section;
}

SDL_RendererFlip Sprite::GetFlip() const
{
	return flip;
}

int Sprite::GetPivotX() const
{
	return pivot_x;
}

int Sprite::GetPivotY() const
{
	return pivot_y;
}

float Sprite::GetSpeed() const
{
	return speed;
}

double Sprite::GetAngle() const
{
	return angle;
}