#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include <vector>
#include "SDL/include/SDL_rect.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side);
	Entity* CreatBuilding(BUILDING_TYPE b_type, fPoint pos, Side side);

	void SelectInQuad(const SDL_Rect& select_rect);
	void UnselectEverything();

	void DeleteEntity(Entity* ptr); // will call other delete methods
	void DeleteUnit(Unit* ptr);
	void DeleteBuilding(Entity* ptr);
	void GetUnitsPath(iPoint destintation);

	Entity* CheckForEnemies(iPoint position, int range, Side side);

	std::vector<Entity*> GetEntityVector();
private:

	//Textura provisional para sprites por si no se cargan en animation
	//SDL_Texture* sprites;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	std::vector<Entity*> entity_array;
};
#endif //_j1EntityManager_

