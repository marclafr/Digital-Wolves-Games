#include "Camera.h"

Camera::Camera(Camera & copy) : moving(false)
{
	view_port.x = copy.view_port.x;
	view_port.y = copy.view_port.y;
	view_port.w = copy.view_port.w;
	view_port.h = copy.view_port.h;
}

Camera::Camera(SDL_Rect & rect) : moving (false)
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

void Camera::Move(iPoint destination, int speed)
{
	destination = this->destination;
	speed = this->speed;

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

void Camera::UpdateCamera()
{
	if (moving)
	{
		int total_movement = y_movement + x_movement;

		if (destination.x > view_port.x)
		{
			if ((speed * (x_movement / total_movement)) > (destination.x - view_port.x))
				view_port.x = destination.x;
			else
			view_port.x += speed * (x_movement / total_movement);
		}
		else
		{
			if ((speed * (x_movement / total_movement)) > abs(destination.x - view_port.x))
				view_port.x = destination.x;
			else
				view_port.x -= speed * (x_movement / total_movement);
		}

		if (destination.y > view_port.y)
		{
			if ((speed * (y_movement / total_movement)) > (destination.y - view_port.y))
				view_port.y = destination.y;
			else
				view_port.y += speed * (y_movement / total_movement);
		}
		else
		{
			if ((speed * (y_movement / total_movement)) > abs(destination.y - view_port.y))
				view_port.y = destination.y;
			else
				view_port.y -= speed * (x_movement / total_movement);
		}
	}
}
