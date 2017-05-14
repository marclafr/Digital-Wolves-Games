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
#include "QuadTree.h"
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

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side);//TODO make const if we can take priority out
	Entity* CreateBuilding(BUILDING_TYPE b_type, fPoint pos, Side side) const;
	Entity* CreateTower(TOWER_TYPE t_type, fPoint pos) const;
	Entity* CreateResource(RESOURCE_TYPE r_type, fPoint pos) const;

	void SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection) const;
	void UnselectEverything() const;
	void Select(Entity* select) const;
	void CheckClick(int mouse_x, int mouse_y)  const;

	Entity* LookForEnemies(int range, fPoint pos, Side side) const;
	
	Entity* CheckForCombat(fPoint position, int range, Side side) const;
	Entity* CheckForObjective(fPoint position, int vision_range, Side side) const;
	void CheckUnitCollisions(const Unit* ptr) const;

	bool IsUnitInTile(const Unit* unit, const iPoint tile) const;
	void LoadAllFx();

	void DrawQuadTree() const;

	//bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&)const;

private:
	//Textura provisional para sprites por si no se cargan en animation
	//SDL_Texture* sprites;

	friend j1Collision;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	QuadTree* entity_quadtree;

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

