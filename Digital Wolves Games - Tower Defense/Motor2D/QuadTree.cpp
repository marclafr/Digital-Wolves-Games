#include "QuadTree.h"

void QuadTreeEndNode::AddEntity(Entity * entity)
{
	for (int i = 0; i < END_NODE_SIZE; i++)
	{
		if (entities[i] != nullptr)
		{
			entities[i] = entity;
			break;
		}

		if (i == (END_NODE_SIZE - 1))
		{
			//SubDivide();
		}
	}
}

void QuadTreeEndNode::SubDivide()
{
}

QuadTreeNode::QuadTreeNode(SDL_Rect area, QuadTreeNode* parent): area(area), parent()
{}
