#include "QuadTree.h"

QuadTreeNode::QuadTreeNode(SDL_Rect area, QuadTreeNode * parent): area(area), parent(parent)
{
	for (int i = 0; i < 4; i++)
		childs[i] = nullptr;

	for (int i = 0; i < NODE_ENTITIES; i++)
		entities[i] = nullptr;
}

QuadTreeNode::~QuadTreeNode()
{
	for (int i = 0; i < 4; i++)
		if (childs[i] != nullptr)
		{
			DELETE_PTR(childs[i]);
		}
		else
			break;
}

void QuadTreeNode::AddEntity(Entity * entity)
{
	if (childs[0] == nullptr)
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] == nullptr)
			{
				entities[i] = entity;
				break;
			}
			if (i == NODE_ENTITIES - 1)
				SubDivide(entity);
		}
	else
		PushToCorrectChild(entity);
}

bool QuadTreeNode::Inside(Entity* entity)
{
	iPoint pos(entity->GetX(), entity->GetY());
	return Inside(pos);
}

bool QuadTreeNode::Inside(iPoint pos)
{
	if (pos.x > area.x
		&& pos.x < area.x + area.w
		&& pos.y > area.y
		&& pos.y < area.y + area.h)
		return true;
	return false;
}

Entity * QuadTreeNode::Search(int pixel_range, iPoint from)
{
	if (childs[0] == nullptr)
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			int distance_x = entities[i]->GetX() - from.x;
			int distance_y = entities[i]->GetY() - from.y;
			int distance = sqrtf(distance_x * distance_x + distance_y * distance_y);

			if (distance <= pixel_range)
				return entities[i];
		}
	else
		for(int i = 0; i < 4; i++)
			if (childs[i]->Inside(iPoint(from.x, from.y + pixel_range))
				|| childs[i]->Inside(iPoint(from.x, from.y - pixel_range))
				|| childs[i]->Inside(iPoint(from.x + pixel_range, from.y))
				|| childs[i]->Inside(iPoint(from.x - pixel_range, from.y)))
				 return Search(pixel_range, from);
	return nullptr;
}

void QuadTreeNode::PushToCorrectChild(Entity * entity)
{
	for (int i = 0; i < 4; i++)
		if (childs[i]->Inside(entity))
			childs[i]->AddEntity(entity);
}

void QuadTreeNode::SubDivide(Entity * entity)
{
	childs[0] = new QuadTreeNode(SDL_Rect{ area.x, area.y, area.w / 2, area.h / 2 }, this);
	childs[1] = new QuadTreeNode(SDL_Rect{ area.x + area.w / 2, area.y, area.w / 2, area.h / 2 }, this);
	childs[2] = new QuadTreeNode(SDL_Rect{ area.x, area.y + area.h / 2, area.w / 2, area.h / 2 }, this);
	childs[3] = new QuadTreeNode(SDL_Rect{ area.x + area.w / 2, area.y + area.h / 2, area.w / 2, area.h / 2 }, this);

	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		PushToCorrectChild(entities[i]);
		entities[i] = nullptr;
	}

	PushToCorrectChild(entity);
}

QuadTree::QuadTree(SDL_Rect area)
{
	origin = new QuadTreeNode(area, nullptr);
}

QuadTree::~QuadTree()
{
	DELETE_PTR(origin);
}

bool QuadTree::PushBack(Entity * entity) const
{
	if(!origin->Inside(entity))
		return false;

	origin->AddEntity(entity);
	return true;
}

Entity * QuadTree::EntitySearch(int pixel_range, iPoint from) const
{
	return origin->Search(pixel_range, from);
}
