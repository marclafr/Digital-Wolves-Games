#ifndef _QUADTREE
#define _QUADTREE

#include "SDL\include\SDL_rect.h"
#include "Entity.h"

#define END_NODE_SIZE 3

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	QuadTreeNode* childs[4];
	SDL_Rect area;
	Entity* entities[END_NODE_SIZE];

public:
	QuadTreeNode(SDL_Rect area, QuadTreeNode* parent);
	
	void AddEntity(Entity* entity);
};

class QuadTree
{

};

#endif
