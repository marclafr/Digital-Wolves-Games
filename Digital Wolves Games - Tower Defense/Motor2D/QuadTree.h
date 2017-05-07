#ifndef _QUADTREE
#define _QUADTREE

#include "SDL\include\SDL_rect.h"
#include "Entity.h"

#define END_NODE_SIZE 3

class QuadTreeNode
{
private:
	QuadTreeNode* parent = nullptr;
	SDL_Rect area;
public:
	QuadTreeNode(SDL_Rect area, QuadTreeNode* parent);

};

class QuadTreeEndNode : public QuadTreeNode
{
private:
	Entity* entities[END_NODE_SIZE];

public:
	void AddEntity(Entity* entity);
	void SubDivide();
};

class QuadTree
{

};

#endif
