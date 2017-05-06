#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Resources.h"
#include "Buildings.h"
#include "Towers.h"
#include <vector>
#include "SDL/include/SDL_rect.h"

class UILabel;
class UIComponents;

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side);
	Entity* CreateBuilding(BUILDING_TYPE b_type, fPoint pos, Side side);
	Entity* CreateTower(TOWER_TYPE t_type, fPoint pos);
	Entity* CreateResource(RESOURCE_TYPE r_type, fPoint pos);
	void SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection);
	void UnselectEverything();
	void Select(Entity* select);
	Entity* LookForEnemies(int range, iPoint pos);

	void CheckClick(int mouse_x, int mouse_y);

	void DeleteEntity(Entity* ptr); // will call other delete methods
	void DeleteUnit(Unit* ptr);
	void DeleteBuilding(Building* ptr);
	void DeleteResource(Resources* ptr);

	Entity* CheckForCombat(iPoint position, int range, Side side);
	Entity* CheckForObjective(iPoint position, int vision_range, Side side);

	bool IsUnitInTile(const Unit* unit, const iPoint tile)const;
	void LoadAllFx();

private:
	//Textura provisional para sprites por si no se cargan en animation
	//SDL_Texture* sprites;

	friend j1Collision;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	std::vector<Entity*> entity_array;

	//TODO:Must change this
	int priority = 0;

public: //TODO: should be protected
	//FX UNITS
	unsigned int fx_twohanded_die01;
	unsigned int fx_twohanded_die02;
	unsigned int fx_twohanded_die03;
	unsigned int fx_twohanded_die04;
	unsigned int fx_twohanded_die05;
	unsigned int fx_attack01;
	unsigned int fx_attack02;
	unsigned int fx_attack03;

	//FX BUILDING
	unsigned int fx_building_destroyed;
	unsigned int fx_arrow;
	unsigned int fx_construction;

};
#endif //_j1EntityManager_

