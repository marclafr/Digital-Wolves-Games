
#include "j1App.h"

#include "j1Render.h"
#include "j1Map.h"
#include "Camera.h"


Camera::Camera(Camera & copy) : moving(false), destination(iPoint(0,0)), speed(0), x_movement(0), y_movement(0), zoom(0.0f), opacity(255), frames_to_black(0), frames_in_black(0), frames_to_light(0), opacity_delta(0)
{
	view_port.x = copy.view_port.x;
	view_port.y = copy.view_port.y;
	view_port.w = copy.view_port.w;
	view_port.h = copy.view_port.h;
}

Camera::Camera(SDL_Rect & rect) : moving (false), destination(iPoint(0, 0)), speed(0), x_movement(0), y_movement(0), zoom(0.0f), opacity(255), frames_to_black(0), frames_in_black(0), frames_to_light(0), opacity_delta(0)
{
	view_port.x = rect.x;
	view_port.y = rect.y;
	view_port.w = rect.w;
	view_port.h = rect.h;
}

void Camera::SetPosition(iPoint pos)
{
	view_port.x = pos.x;
	view_port.y = pos.y;
}

void Camera::SetSize(int w, int h)
{
	view_port.w = w;
	view_port.h = h;
}

const iPoint Camera::GetPosition() const
{
	return iPoint(view_port.x, view_port.y);
}

const int Camera::GetWidth() const
{
	return view_port.w;
}

const int Camera::GetHeight() const
{
	return view_port.h;
}

const SDL_Rect Camera::GetVP() const
{
	return view_port;
}

const iPoint Camera::GetCenter() const
{
	return iPoint((view_port.x + view_port.w / 2), (view_port.y + view_port.h / 2));
}

const float Camera::GetOpacity() const
{
	return opacity;
}

bool Camera::InsideRenderTarget(int x, int y)
{
	x = (x - view_port.w / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.w / 2.0f;

	y = (y - view_port.h / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.h / 2.0f;

	int zoomed_tile_width = App->map->data.tile_width *(1.0f + zoom / 100.0f);
	int zoomed_tile_height = App->map->data.tile_height *(1.0f + zoom / 100.0f);
	int zoomed_tower_height = TOWER_HEIGHT * (1.0f + zoom / 100.0f);

	if (x < -zoomed_tile_width)
		return false;
	if (x > view_port.w)
		return false; 
	if (y < -zoomed_tile_height)
		return false;
	if (y > (view_port.h + zoomed_tower_height)) // IF NOT FOR TOWER HEIGHT TOWERS WOULD NOT PRINT UNTIL THEIR LOWEST PART IS INSIDE THE CAMERA
		return false;
	return true;
}

void Camera::Move(iPoint destination, int speed)
{
	moving = true;

	this->destination = destination;
	this->speed = speed;

	y_movement = abs(destination.y - view_port.y);
	x_movement = abs(destination.x - view_port.x);
}

void Camera::MoveUp(float amount)
{
	view_port.y += floor(amount);
}

void Camera::MoveLeft(float amount)
{
	view_port.x += floor(amount);
}

void Camera::MoveDown(float amount)
{
	view_port.y -= floor(amount);
}

void Camera::MoveRight(float amount)
{
	view_port.x -= floor(amount);
}

void Camera::SetZoom(float zoom_percentage)
{
	zoom = zoom_percentage;
}

void Camera::ZoomIn(float percentage)
{
	zoom += percentage;
}

void Camera::ZoomOut(float percentage)
{
	zoom -= percentage;
}

SDL_Rect Camera::GetZoomedRect(const SDL_Rect &starting_rect) const
{
	SDL_Rect rect = starting_rect;

	rect.w = rect.w * (1.0f + zoom / 100.0f);

	rect.h = rect.h * (1.0f + zoom / 100.0f);
	
	rect.x = (rect.x - view_port.w / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.w / 2.0f;

	rect.y = (rect.y - view_port.h / 2.0f) * (1.0f + (zoom / 100.0f)) + view_port.h / 2.0f;

	return rect;
}

void Camera::FadeToBlack(float secs_to_black, int wait, int secs_to_light)
{
	frames_to_black = secs_to_black * FPS;
	frames_in_black = wait * FPS;
	frames_to_light = secs_to_light * FPS;
	opacity_delta = 255 / frames_to_black;
}

void Camera::UpdateCamera()
{
	if (moving)
	{
		int total_movement = y_movement + x_movement;

		if (destination.x > view_port.x)
		{
			if ((speed * (float(x_movement) / float(total_movement))) > (destination.x - view_port.x))
			{
				view_port.x = destination.x;
				moving = false;
			}
			else
			view_port.x += speed * (float (x_movement) / float (total_movement));
		}
		else
		{
			if ((speed * (float(x_movement) / float(total_movement))) > abs(destination.x - view_port.x))
			{
				view_port.x = destination.x;
				moving = false;
			}
			else
				view_port.x -= speed * (float(x_movement) / float(total_movement));
		}

		if (destination.y > view_port.y)
		{
			if ((speed * (float(y_movement) / float (total_movement))) > (destination.y - view_port.y))
			{
				view_port.y = destination.y;
				moving = false;
			}
			else
				view_port.y += speed * (float(y_movement) / float(total_movement));
		}
		else
		{
			if ((speed * (float(y_movement) / float(total_movement))) > abs(destination.y - view_port.y))
			{
				view_port.y = destination.y;
				moving = false;
			}
			else
				view_port.y -= speed * (float(x_movement) / float(total_movement));
		}
	}


	if (frames_to_black > 0)
	{
		frames_to_black--;
		opacity -= opacity_delta;
		if (frames_to_black == 0)
			opacity_delta = 255 / frames_to_light;
	}
	else if (frames_in_black > 0)
		frames_in_black--;
	else if (frames_to_light > 0)
	{
		opacity += opacity_delta;
		frames_to_light--;
	}

}
