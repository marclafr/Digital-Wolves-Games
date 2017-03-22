#ifndef _CAMERA
#define _CAMERA


#include "SDL\include\SDL.h"
#include "p2Point.h"

class Camera
{
private:
	SDL_Rect view_port;
	bool moving;
	iPoint destination;
	int speed;
	int x_movement;
	int y_movement;

public:
	Camera(Camera& copy);
	Camera(SDL_Rect& rect);

	void SetPosition(iPoint pos);
	void SetSize(int w, int h);

	const iPoint GetPosition() const;
	const int GetWidth() const;
	const int GetHeight() const;

	void Move(iPoint destination, int speed);
	void MoveUp(float amount);
	void MoveLeft(float amount);
	void MoveDown(float amount);
	void MoveRight(float amount);


	void UpdateCamera();
};



#endif