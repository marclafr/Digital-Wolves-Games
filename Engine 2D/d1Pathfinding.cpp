#include "c2Defs.h"
#include "c2Log.h"
#include "d1App.h"
#include "d1PathFinding.h"

d1PathFinding::d1PathFinding() : d1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
d1PathFinding::~d1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool d1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void d1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool d1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool d1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar d1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const c2DynArray<iPoint>* d1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
c2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	c2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
c2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	c2List_item<PathNode>* ret = NULL;
	int min = 65535;

	c2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int d1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = 1;
	// TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination))
	{
		// TODO 2: Create three lists: open, close
		// Add the origin tile to open
		// Iterate while we have tile in the open list
		PathList open, close;
		open.list.add(PathNode(0, origin.DistanceManhattan(destination), origin, nullptr));

		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.add(open.GetNodeLowestScore()->data);
		open.list.del(open.GetNodeLowestScore());

		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		while(close.list.end->data.pos != destination)
		{
			// TODO 5: Fill a list of all adjancent nodes
			uint count = open.list.count();
			for(uint i = close.list.end->data.FindWalkableAdjacents(open); i > count; i--)
			{
				c2List_item<PathNode>* modify = open.list.At(i - 1);
				if (modify != nullptr)
				{
					c2List_item<PathNode>* close_exist = close.Find(modify->data.pos);
					if (close_exist == nullptr)
					{
						modify->data.g = modify->data.parent->g + 1;
						modify->data.h = modify->data.pos.DistanceManhattan(destination);
					}
					else
						open.list.del(open.Find(close_exist->data.pos));
				}
			}

			close.list.add(open.GetNodeLowestScore()->data);
			open.list.del(open.GetNodeLowestScore());
		}

		last_path.PushBack(close.list.end->data.pos);

		const PathNode* actual_parent = close.list.end->data.parent;
		
		while (actual_parent != nullptr)
		{
			last_path.PushBack(actual_parent->pos);
			actual_parent = actual_parent->parent;
		}

		last_path.Flip();

		// TODO 6: Iterate adjancent nodes:
		// ignore nodes in the closed list
		// If it is NOT found, calculate its F and add it to the open list
		// If it is already in the open list, check if it is a better path (compare G)
		// If it is a better path, Update the parent
	}
	else
		ret = -1;

	return ret;
}

