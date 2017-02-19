#ifndef _J1UNITS_
#define _J1UNITS_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
enum UNITS_TYPE{
	VILLAGER,
	TWOHANDED
};

class j1Units : public j1Module
{
public:

	j1Units();
	~j1Units();

	bool Awake();
	bool Update();
	bool CleanUp();

	bool AddEnemy(UNITS_TYPE type, int x, int y);
private:
	iPoint Position;
	int hp, attack;
	float speed;

	//Textura provisional para sprites por si no se cargan en animation
	SDL_Texture* sprites;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	std::list<j1Units*> unit_list;
};
#endif //_J1UNITS_
