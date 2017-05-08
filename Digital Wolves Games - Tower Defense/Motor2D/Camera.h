#ifndef _CAMERA
#define _CAMERA

#include "SDL\include\SDL.h"
#include "p2Point.h"

#define H_MARGIN 60 //biggest height for culling without popping
#define W_MARGIN 60 //biggest width for culling without popping
#define FPS 60

//Camera limits
#define CAMERA_LIMIT_UP 58
#define CAMERA_LIMIT_DOWN - 2900
#define CAMERA_LIMIT_LEFT 3602
#define CAMERA_LIMIT_RIGHT -2142

#define MARGIN 25


class Camera
{
private:
	SDL_Rect view_port;
	bool moving;
	iPoint destination;
	int speed;
	int x_movement;
	int y_movement;
	float zoom;

	//fade to black
	bool transitioning;
	float alpha;
	float wait;
	float alpha_delta;

public:
	Camera(Camera& copy);
	Camera(SDL_Rect& rect);

	void SetPosition(iPoint pos);
	void SetSize(int w, int h);

	const iPoint GetPosition() const;
	const int GetWidth() const;
	const int GetHeight() const;
	const SDL_Rect GetVP() const;
	const iPoint GetCenter() const;
	const float GetAlpha() const;

	void KeyboardMove(float dt);
	void MouseMove(int x, int y, float dt);
	
	bool InsideRenderTarget(int x, int y);

	void Move(iPoint destination, int speed);
	void MoveUp(float amount);
	void MoveLeft(float amount);
	void MoveDown(float amount);
	void MoveRight(float amount);

	void SetZoom(float percentage); //%

	void ZoomIn(float percentage = 1); //percentage per frame
	void ZoomOut(float percentage = 1);

	SDL_Rect GetZoomedRect(const SDL_Rect &rect) const;

	void FadeToBlack(float secs_to_black, int wait_secs = 0);// secs at 60fps

	void FadeToLight(float secs_to_light);

	void UpdateCamera();

	void UpdateTransitions();
};



#endif