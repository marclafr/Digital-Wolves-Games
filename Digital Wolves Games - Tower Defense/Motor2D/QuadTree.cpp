#include "j1App.h"
#include "IsoPrimitives.h"
#include "j1Pathfinding.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(IsoRect area, QuadTreeNode * parent): area(area), parent(parent)
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

	for (int i = 0; i < NODE_ENTITIES; i++)
		if(entities[i] != nullptr)
		{
			//DELETE_PTR(entities[i]); TODO
		}
		else
			break;
}

void QuadTreeNode::AddEntity(Entity * entity)
{
	if (childs[0] == nullptr)
	{
		if (entities[NODE_ENTITIES - 1] != nullptr)
			SubDivide(entity);
		else
			for (int i = 0; i < NODE_ENTITIES; i++)
				if (entities[i] == nullptr)
				{
					entities[i] = entity;
					break;
				}
	}
	else
		PushToCorrectChild(entity);
}

bool QuadTreeNode::Inside(const Entity* entity)
{
	iPoint pos(entity->GetX(), entity->GetY());
	return area.Inside(pos);
}

Entity * QuadTreeNode::SearchFirst(int pixel_range, const iPoint from) const
{
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					return entities[i];
	}
	else
		for(int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				 return childs[i]->SearchFirst(pixel_range, from);
	return nullptr;
}

Entity * QuadTreeNode::SearchFirst(const SDL_Rect rect) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			iPoint entity_pos(entities[i]->GetX(), entities[i]->GetY());
			if (entities[i] != nullptr)
				if(rect.x < entity_pos.x && rect.x + rect.w > entity_pos.x
					&& rect.y < entity_pos.y && rect.y + rect.h > entity_pos.y)
					return entities[i];
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area.Overlaps(rect))
				return childs[i]->SearchFirst(rect);
	return nullptr;
}

Entity * QuadTreeNode::SearchFirstEnemy(int pixel_range, const iPoint from, const Side side) const
{
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					if(entities[i]->GetSide() != side && entities[i]->GetHp() > 0)
						return entities[i];
	}
	else
		for (int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				return childs[i]->SearchFirstEnemy(pixel_range, from, side);
	return nullptr;
}

void QuadTreeNode::Search(int pixel_range, const iPoint from, std::vector<Entity*>& vec) const
{
	vec.clear();
	Circle circle(from, pixel_range);

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (circle.Intersects(&iPoint(entities[i]->GetX(), entities[i]->GetY())))
					vec.push_back(entities[i]);
	}
	else
		for (int i = 0; i < 4; i++)
			if (circle.Intersects(&childs[i]->area))
				childs[i]->Search(pixel_range, from, vec);
}

void QuadTreeNode::Search(const SDL_Rect rect, std::vector<Entity*>& vec) const
{
	vec.clear();

	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if(entities[i] != nullptr)
				if (entities[i]->Inside(rect))
					vec.push_back(entities[i]);
	}
	else
		for (int i = 0; i < 4; i++)
			if (childs[i]->area.Overlaps(rect))
				childs[i]->Search(rect, vec);
}

void QuadTreeNode::PushToCorrectChild(Entity * entity)
{
	for (int i = 0; i < 4; i++)
		if (childs[i]->Inside(entity))
			childs[i]->AddEntity(entity);
}

void QuadTreeNode::SubDivide(Entity * entity)
{
	float half_w = area.GetWidth() / 2.0f;
	float half_h = area.GetHeight() / 2.0f;

	IsoRect first(iPoint(area.GetPosition().x, area.GetPosition().y - half_h / 2.0f), half_w, half_h);
	IsoRect second(iPoint(area.GetPosition().x - half_w / 2.0f, area.GetPosition().y), half_w, half_h);
	IsoRect third(iPoint(area.GetPosition().x, area.GetPosition().y + half_h / 2.0f), half_w, half_h);
	IsoRect forth(iPoint(area.GetPosition().x + half_w / 2.0f, area.GetPosition().y), half_w, half_h);

	childs[0] = new QuadTreeNode(first, this);
	childs[1] = new QuadTreeNode(second, this);
	childs[2] = new QuadTreeNode(third, this);
	childs[3] = new QuadTreeNode(forth, this);

	for (int i = 0; i < NODE_ENTITIES; i++)
	{
		PushToCorrectChild(entities[i]);
		entities[i] = nullptr;
	}

	PushToCorrectChild(entity);
}

void QuadTreeNode::Reorganise()
{
	for(int i = 0; i < NODE_ENTITIES; i++)
		for(int j = i+1; NODE_ENTITIES; j++)
			if (entities[i] == nullptr && entities[j] != nullptr)
			{
				entities[i] = entities[j];
				entities[j] = nullptr;
			}
}

bool QuadTreeNode::Empty() const
{
	bool ret = true;

	if(entities[0] != nullptr)
		ret = false;
	else
	{
		if (childs[0] != nullptr)
			for (int i = 0; i < 4; i++)
			{
				ret = childs[i]->Empty();
				if (ret == false)
					break;
			}
		else
			ret = true;
	}
	return ret;
}

void QuadTreeNode::Update(float dt)
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				{
					entities[i]->Update(dt);

					if (!area.Inside(entities[i]->GetIPos()))
					{
						AddEntity(entities[i]);
						entities[i] = nullptr;
						Reorganise();
					}
						

				}
				
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->Update(dt);
}

void QuadTreeNode::DeleteEntities()
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr)
				if (entities[i]->ToDelete())
				{
					DELETE_PTR(entities[i]);
					Reorganise();
				}
			else
				break;
	}
	else
	{	
		bool has_entities = false;

		for (int i = 0; i < 4; i++)
		{
			childs[i]->DeleteEntities();

			if (childs[i]->Empty() == false)
				has_entities = true;
		}

		if (has_entities == false)
			for (int i = 0; i < 4; i++)
			{
				DELETE_PTR(childs[i]);
			}
	}
}

void QuadTreeNode::CheckCollision() const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
			if (entities[i] != nullptr && entities[i]->GetEntityType() == E_UNIT)
			{
				Unit* unit = (Unit*)entities[i];
				unit->CheckCollisions();
			}
				
			else
				break;
	}
	else
		for (int i = 0; i < 4; i++)
			childs[i]->CheckCollision();
}

void QuadTreeNode::CheckUnitCollisions(const Unit * ptr) const
{
	if (childs[0] == nullptr)
	{
		for (int i = 0; i < NODE_ENTITIES; i++)
		{
			if (entities[i] != nullptr)
			{
				if (entities[i]->GetEntityType() == E_UNIT)
				{
					Unit* unit = (Unit*)entities[i];
					if (ptr->GetUnitCircle().Overlap(&unit->GetUnitCircle()))
						if (unit->GetAction() == A_IDLE)
						{
							iPoint destination;
							if (App->pathfinding->FindNearestUnocupied(destination))
								unit->GoTo(destination);
						}
						else if (unit->GetAction() == A_ATTACK)
						{
							iPoint destination;
							if (unit->FindEmptyAttackPos(destination))
								unit->GoTo(destination);
						}
				}
			}
		}
	}
	else
		for (int i = 0; i < 4; i++)
			if (ptr->GetUnitCircle().Intersects(&childs[i]->area))
				childs[i]->CheckUnitCollisions(ptr);
}

void QuadTreeNode::DrawArea()
{
	if (childs[0] != nullptr)
		for (int i = 0; i < 4; i++)
			childs[i]->DrawArea();

	area.SetColor(SDL_Color{ 255,0,0,255 });
	area.Draw();
}

void QuadTreeNode::SaveAll(pugi::xml_node &data)
{
	pugi::xml_node Buildings = data.append_child("buildings");
	pugi::xml_node Units = data.append_child("units");
	pugi::xml_node Turrets = data.append_child("turrets");
	//simular for

	//if( entity_type == E_BUILDING)
	//	if(building_type == B_TURRET)
	//		SaveTurret(Turrets)
	//	else 
	//		SaveBuilding(Buildings);

	//else if (entity_type == E_UNIT)

	//SaveUnit(Units)
}

QuadTree::QuadTree(IsoRect area)
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

Entity * QuadTree::SearchFirst(int pixel_range, iPoint from) const
{
	return origin->SearchFirst(pixel_range, from);
}

Entity * QuadTree::SearchFirstEnemy(int pixel_range, iPoint from, Side side) const
{
	return origin->SearchFirstEnemy(pixel_range, from, side);
}

void QuadTree::Search(int pixel_range, iPoint from, std::vector<Entity*>& vec) const
{
	origin->Search(pixel_range, from, vec);
}

void QuadTree::Search(SDL_Rect rect, std::vector<Entity*>& vec) const
{
	origin->Search(rect, vec);
}

void QuadTree::UpdateAll(float dt) const
{	
	origin->Update(dt);
}

void QuadTree::DeleteEntities() const
{
	origin->DeleteEntities();
}

void QuadTree::CheckCollisions() const
{
	origin->CheckCollision();
}

void QuadTree::CheckUnitCollisions(const Unit * ptr) const
{
	origin->CheckUnitCollisions(ptr);
}

void QuadTree::DrawRects() const
{
	origin->DrawArea();
}
