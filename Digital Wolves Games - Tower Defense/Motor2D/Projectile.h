#ifndef _PROJECTILE
#define _PROJECTILE

#define AREA_DMG_RADIUS 30
#define ELEMENT_TERRAIN_TIME 1.0f

#include "j1App.h"
#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "p2Point.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1PerfTimer.h"

class AnimationManager;

enum PROJECTILE_TYPE 
{
	P_BASIC_ARROW,
	P_FIRE_ARROW,
	P_ICE_ARROW,
	P_AIR_ARROW,
	P_CANNONBALL,
	P_FIRE_CANNONBALL,
	P_ICE_CANNONBALL,
	P_AIR_CANNONBALL
};


class Projectile
{
private:
	SDL_Rect rect;
	iPoint pivot;
	fPoint StartPos;
	fPoint ActualPos, PreActualPos;
	fPoint LastPos;
	Entity* Target;
	int Damage;
	float Diferential;
	enum PROJECTILE_TYPE projectile_type;
	float ProjectilePos = 0; //it goes from 0 to 1
	int angle = 0;
	int StartHeight;
	int CurveHeight;
	AnimationManager* projectile_anim = nullptr;

	void AreaDamage(int damage, iPoint center, int radius);

	AnimationManager* anim_ice_floor = nullptr;
	AnimationManager* anim_fire_try = nullptr;
	j1PerfTimer PrintElementTerrainTimer;
	bool dest_reached = false;
	fPoint element_terrain_pos;
	void PrintElementTerrain(PROJECTILE_TYPE element, fPoint center);

public:

	Projectile(fPoint initialpos, Entity* target, int damage, float TimeInSecs,int Startheight, int Curveheight, PROJECTILE_TYPE type);
	~Projectile();

	void Update();
	void Draw();

	bool floor_effect = false;
	int GetProjectilePos() const;
	int GetDamage() const;
	void SetRect(SDL_Rect rect);
	AnimationManager* GetProjectileAnim();
};

#endif