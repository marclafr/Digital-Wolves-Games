#ifndef _PROJECTILE
#define _PROJECTILE

#include "j1App.h"
#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Render.h"


enum PROJECTILE_TYPE {
	P_BASIC_ARROW,
	P_FIRE_ARROW,
	P_ICE_ARROW,
	P_AIR_ARROW,
	P_CANNONBALL
};


class Projectile
{
private:
	SDL_Rect rect;
	fPoint StartPos;
	fPoint ActualPos;
	fPoint LastPos;
	Entity* Target;
	int Damage;
	float Diferential;
	enum PROJECTILE_TYPE projectile_type;
	float ProjectilePos = 0; //it goes from 0 to 1
	int StartHeight;
	int CurveHeight;
public:

	Projectile(fPoint initialpos, Entity* target, int damage, float TimeInSecs,int Startheight, int Curveheight, PROJECTILE_TYPE type);
	~Projectile();

	void Update();
	void Draw();

	int GetProjectilePos() const;
	int GetDamage() const;
	void SetRect(SDL_Rect rect);
};

#endif