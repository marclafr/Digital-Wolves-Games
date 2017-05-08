#ifndef _QUADTREE
#define _QUADTREE

#include "SDL\include\SDL_rect.h"
#include "Entity.h"

#define NODE_ENTITIES 3

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	QuadTreeNode* childs[4];
	SDL_Rect area;
	Entity* entities[NODE_ENTITIES];

public:
	QuadTreeNode(SDL_Rect area, QuadTreeNode* parent);
	~QuadTreeNode();
	
	void AddEntity(Entity* entity);
	bool Inside(Entity* entity);
	bool Inside(iPoint pos);
	Entity* Search(int pixel_range, iPoint from);

private:
	void PushToCorrectChild(Entity* entity);
	void SubDivide(Entity* entity);
};

class QuadTree
{
private:
	QuadTreeNode* origin = nullptr;

public:
	QuadTree(SDL_Rect area);
	~QuadTree();

	bool PushBack(Entity* entity) const;
	
	Entity* EntitySearch(int pixel_range, iPoint from) const;
};

#endif
