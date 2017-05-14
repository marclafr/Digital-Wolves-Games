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
	Entity* CreateBuilding(BUILDING_TYPE b_type, fPoint pos, bool builded) const;
	Entity* CreateTower(TOWER_TYPE t_type, fPoint pos, iPoint posintiles) const;
	Entity* CreateResource(RESOURCE_TYPE r_type, fPoint pos, int amount_collect, int time) const;

	void SelectInQuad(const SDL_Rect& select_rect, std::vector<Entity*>& selection) const;
	void UnselectEverything() const;
	void Select(Entity* select) const;
	void CheckClick(int mouse_x, int mouse_y)  const;

	Entity* LookForEnemies(int range, fPoint pos, Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	
	Entity* CheckForCombat(fPoint position, int range, Side side) const;
	Entity* CheckForObjective(fPoint position, int vision_range, Side side) const;
	Unit* CheckUnitCollisions(const Unit* ptr) const;

	bool IsUnitInTile(const Unit* unit, const iPoint tile) const;

	void DrawQuadTree() const;

	bool Load(pugi::xml_node&);
	void LoadBuilding(pugi::xml_node&);
	void LoadUnit(pugi::xml_node&);
	void LoadTurret(pugi::xml_node&);
	void LoadResource(pugi::xml_node&);

	bool Save(pugi::xml_node&)const;



	void GetEntitiesInIsoRect(const IsoRect rect, std::vector<Entity*>& vec) const;

private:
	//Textura provisional para sprites por si no se cargan en animation
	//SDL_Texture* sprites;

	friend j1Collision;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	QuadTree* entity_quadtree;

	//TODO:Must change this
	int priority = 0;

public: //TODO: should be protected

};
#endif //_j1EntityManager_

