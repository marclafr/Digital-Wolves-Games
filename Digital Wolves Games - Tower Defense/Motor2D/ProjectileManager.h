#ifndef _PROJECTILEMANAGER
#define _PROJECTILEMANAGER


#include "j1Module.h"
#include "Projectile.h"
#include "j1Textures.h"
#include "p2Point.h"
#include <vector>
#include "SDL/include/SDL_rect.h"


class ProjectileManager : public j1Module
{
private:
	std::vector<Projectile*> projectile_vector;
public:

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	Projectile* CreateProjectile(fPoint initialpos, Entity * target, int damage, float TimeInSecs, int Startheight, int Curveheight, PROJECTILE_TYPE type);
	void DeleteProjectile(Projectile* projectile);
};

#endif
