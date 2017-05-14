#ifndef _QUADTREE
#define _QUADTREE

#include <vector>
#include "SDL\include\SDL_rect.h"
#include "Entity.h"
#include "Units.h"
#include "Buildings.h"
#include "Resources.h"

#define NODE_ENTITIES 3

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	QuadTreeNode* childs[4];
	IsoRect area;
	Entity* entities[NODE_ENTITIES];

public:
	QuadTreeNode(IsoRect area, QuadTreeNode* parent);
	~QuadTreeNode();
	
	bool AddEntity(Entity* entity);
	bool Inside(const Entity* entity) const;

	Entity* SearchFirst(int pixel_range, const  fPoint from) const;
	Entity* SearchFirst(IsoRect rect) const;
	Entity* SearchFirst(const SDL_Rect rect) const;
	Entity* SearchFirstEnemy(int pixel_range, const fPoint from, const Side side) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type) const;
	void Search(int pixel_range, const fPoint from, std::vector<Entity*>& vec) const;
	void Search(const IsoRect rect, std::vector<Entity*>& vec) const;

	void Selection(const SDL_Rect rect, std::vector<Entity*>& vec) const;

	void Update(float dt);
	void DeleteEntities();
	Unit* CheckCollision(Elipse Circle) const;
	void CheckUnitCollisions(const Unit* ptr) const;
	void DrawArea();
	void SaveAll(pugi::xml_node&);
	void BlitMinimap();

private:
	bool PushToCorrectChild(Entity* entity);
	bool SubDivide(Entity* entity);
	void Reorganise();
	void RedoQuads();
	bool Empty() const;
	int NumEntities() const;
	void MoveUpEntities();
};

class QuadTree
{
private:
	QuadTreeNode* origin;

public:
	QuadTree(IsoRect area);
	~QuadTree();

	bool PushBack(Entity* entity) const;
	
	Entity* SearchFirst(int pixel_range, fPoint from) const;
	Entity* SearchFirstEnemy(int pixel_range, fPoint from, Side side) const;
	Entity* SearchFirstEnemy(IsoRect rect, const Side side, ENTITY_TYPE entity_type = E_NO_ENTITY) const;
	void Search(int pixel_range, fPoint from, std::vector<Entity*>& vec) const;

	void Selection(SDL_Rect rect, std::vector<Entity*>& vec) const;

	void UpdateAll(float dt) const;

	void DeleteEntities() const;

	Unit* CheckCollisions(Elipse elipse) const;

	void DrawRects() const;
};

#endif
