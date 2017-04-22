#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "j1Input.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	if(map != nullptr) RELEASE_ARRAY(map);
	if (node_map != nullptr) RELEASE_ARRAY(node_map);
	if (constructible_map_ally != nullptr) RELEASE_ARRAY(constructible_map_ally);
	if (constructible_map_neutral != nullptr) RELEASE_ARRAY(constructible_map_neutral);
	return true;
}

void j1PathFinding::CleanUpJPS()
{
	destination_reached = false;

	for (std::vector<PathNode*>::iterator i = visited.begin(); i != visited.end(); ++i)
	{
		delete *i;
		*i = nullptr;
	}

	visited.clear();

	frontier.clear();
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	//TODO1
	//create a node_map
	//RELEASE_ARRAY(node_map);
	node_map = new PathNode[width*height];

	memcpy(map, data, width*height);
}
void j1PathFinding::SetConstructibleMaps(uint width, uint height, uchar* data, uchar* data2)
{
	constructible_map_ally = new uchar[width*height];
	memcpy(constructible_map_ally, data, width*height);

	constructible_map_neutral = new uchar[width*height];
	memcpy(constructible_map_neutral, data2, width*height);

}
// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}
bool j1PathFinding::IsConstructible_ally(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_ally(pos);
	return t != INVALID_WALK_CODE && t > 1;
}
bool j1PathFinding::IsConstructible_neutral(const iPoint& pos) const
{
	uchar t = GetTileAtConstructible_neutral(pos);
	return t != INVALID_WALK_CODE && t > 1;
}
void j1PathFinding::MakeNoConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeNoConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeConstruible_neutral(const iPoint& pos)
{
	constructible_map_neutral[pos.y*width + pos.x] = 10;
}
void j1PathFinding::MakeConstruible_ally(const iPoint& pos)
{
	constructible_map_ally[pos.y*width + pos.x] = 10;
}

void j1PathFinding::MakeNoWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = INVALID_WALK_CODE;
}

void j1PathFinding::MakeWalkable(const iPoint& pos)
{
	map[pos.y*width + pos.x] = 34;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

uchar j1PathFinding::GetTileAtConstructible_ally(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_ally[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}
uchar j1PathFinding::GetTileAtConstructible_neutral(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return constructible_map_neutral[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>& j1PathFinding::GetLastPath() const
{
	return last_path;
}

PathNode* j1PathFinding::GetPathNode(int x, int y)
{
	return &node_map[(y*width) + x];
}

iPoint j1PathFinding::FindNearestWalkable(const iPoint & origin)
{
	iPoint ret(origin);

	// dx -> direction x  | dy -> direction y  
	// search_in_radius -> finds the nearest walkable tile in a radius (max radius in FIND_RADIUS) 

	int search_in_radius = 1;
	while (search_in_radius != FIND_RADIUS)
	{
		for (int dx = -search_in_radius; dx < search_in_radius; dx++)
		{
			for (int dy = -search_in_radius; dy < search_in_radius; dy++)
			{
				ret.x = origin.x + dx;
				ret.y = origin.y + dy;
				if (IsWalkable(ret))
					return ret; // Found the nearest walkable tile
			}
		}

		++search_in_radius;
	}
	return ret.create(-1, -1);
}

	 //----------------||----------------||----------------\\
	//----------------||A* + JPS algorithm||----------------\\
   //----------------||____________________||----------------\\

bool j1PathFinding::CalculatePath(iPoint start, const iPoint & end, std::vector<iPoint>& vec_to_fill)
{
	CleanUpJPS();

	origin = new PathNode(0.0f, 0.0f, start, nullptr);
	destination = new PathNode(0.0f, 0.0f, end, nullptr);
	PathNode* check = new PathNode(*origin);

	if (IsWalkable(start))
	{
		visited.push_back(check);
		visited.push_back(destination);
	}
	else
	{
		LOG("Non-Walkable origin");
		return false;
	}

	if (!IsWalkable(end))
	{
		LOG("Non-Walkable Destination");
		return false;
	}

	int dx;
	int dy;

	OpenOrigin();

	//open all forced neighbours
	ForcedNeighbour forced_neighbour;

	while (GetLowestFN(forced_neighbour))
	{
		if (forced_neighbour.after->cost_so_far > destination->cost_so_far && destination_reached == true)
			break;

		check = forced_neighbour.after;
		dx = forced_neighbour.after->pos.x - forced_neighbour.before->pos.x;
		dy = forced_neighbour.after->pos.y - forced_neighbour.before->pos.y;

		CheckForTiles(check, dx, dy, end);
	}

	if (destination_reached == true)
	{
		FillPathVec(vec_to_fill);

		CleanUpJPS();

		DeleteIfNotPushed(origin);

		return true;
	}

	return false;
}

bool j1PathFinding::CheckForTiles(const PathNode* start, int dx, int dy, const iPoint& destination) //true when destination reached
{
	if (IsWalkable(start->pos))
	{
		bool diagonal_end = false;
		bool diagonal_pushed = false;

		PathNode* diagonal = new PathNode(start->pos);
		PathNode* line = new PathNode(start->pos);

		while (diagonal_end == false)
		{
			//x > 0 == right
			line->pos = diagonal->pos;

			//Right
			if (dx > 0)
				while (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)))
				{
					line->pos.x++;

					//check destination

					if (line->pos == destination)
					{
						DestinationReached(line, start, diagonal_pushed, diagonal);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//UP
					if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
						if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start, destination, diagonal_pushed, diagonal);

					//DOWN
					if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
						if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start, destination, diagonal_pushed, diagonal);
				}

			//Left
			if (dx < 0)
				while (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)))
				{
					line->pos.x--;

					if (line->pos == destination)
					{
						DestinationReached(line, start, diagonal_pushed, diagonal);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//UP
					if (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)) == false)
						if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start, destination, diagonal_pushed, diagonal);

					//DOWN
					if (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)) == false)
						if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start, destination, diagonal_pushed, diagonal);
				}

			//y > 0 down
			line->pos = diagonal->pos;

			//Down
			if (dy > 0)
				while (IsWalkable(iPoint(line->pos.x, line->pos.y + 1)))
				{
					line->pos.y++;

					if (line->pos == destination)
					{
						DestinationReached(line, start, diagonal_pushed, diagonal);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//Left
					if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
						if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y + 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y + 1), start, destination, diagonal_pushed, diagonal);

					//Right
					if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
						if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y + 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y + 1), start, destination, diagonal_pushed, diagonal);
				}

			//Up
			if (dy < 0)
				while (IsWalkable(iPoint(line->pos.x, line->pos.y - 1)))
				{
					line->pos.y--;

					if (line->pos == destination)
					{
						DestinationReached(line, start, diagonal_pushed, diagonal);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//Left
					if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y)) == false)
						if (IsWalkable(iPoint(line->pos.x - 1, line->pos.y - 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x - 1, line->pos.y - 1), start, destination, diagonal_pushed, diagonal);

					//Right
					if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y)) == false)
						if (IsWalkable(iPoint(line->pos.x + 1, line->pos.y - 1)) == true)
							FoundForcedNeighbour(line, iPoint(line->pos.x + 1, line->pos.y - 1), start, destination, diagonal_pushed, diagonal);
				}

			//diagonal dir

			if (diagonal_pushed)
			{
				diagonal = new PathNode(diagonal->pos);
				diagonal_pushed = false;
			}

			if (dx > 0 && dy > 0)
				if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
				{
					diagonal->pos.x++;
					diagonal->pos.y++;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//x
					if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
					//y
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);

				}
				else
					diagonal_end = true;

			else if (dx < 0 && dy < 0)
				if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
				{
					diagonal->pos.x--;
					diagonal->pos.y--;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//x
					if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
					//y
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dx > 0 && dy < 0)
				if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
				{
					diagonal->pos.x++;
					diagonal->pos.y--;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//x
					if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
					//y
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dx < 0 && dy > 0)
				if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
				{
					diagonal->pos.x--;
					diagonal->pos.y++;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//x
					if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
					//y
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dx < 0)
				if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)))
				{
					diagonal->pos.x--;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//up
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
					//down
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dx > 0)
				if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)))
				{
					diagonal->pos.x++;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//up
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
					//down
					if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dy < 0)
				if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y - 1)))
				{
					diagonal->pos.y--;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//right
					if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
					//left
					if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y - 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;

			else if (dy > 0)
				if (IsWalkable(iPoint(diagonal->pos.x, diagonal->pos.y + 1)))
				{
					diagonal->pos.y++;

					if (diagonal->pos == destination)
					{
						DestinationReached(diagonal, start, diagonal_pushed);

						DeleteIfNotPushed(diagonal);
						DeleteIfNotPushed(line);

						return true;
					}

					//right
					if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x + 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
					//left
					if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y)) == false)
						if (IsWalkable(iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1)))
							FoundForcedNeighbour(diagonal, iPoint(diagonal->pos.x - 1, diagonal->pos.y + 1), start, destination, diagonal_pushed);
				}
				else
					diagonal_end = true;
		}

		DeleteIfNotPushed(diagonal);
		DeleteIfNotPushed(line);
	}
	else
		LOG("ERROR: Non Walkable Start Search Position");

	return false;
}

void j1PathFinding::PushToVisited(PathNode* node)
{	
	bool found = false;

	for (std::vector<PathNode*>::iterator it = visited.begin(); it != visited.end(); ++it)
	{
		if ((*it)->pos == node->pos)
		{
			if (node->cost_so_far <= (*it)->cost_so_far || (*it)->cost_so_far == 0)
			{
				*it = node;
			}
			found = true;
		}
	}

	if (found == false)
		visited.push_back(node);
}

void j1PathFinding::FoundForcedNeighbour(PathNode*& before, iPoint after_pos, const PathNode* start, iPoint destination, bool& pushed_path, PathNode * path_to)
{
	if (path_to != nullptr)
	{
		if (path_to->pos != start->pos)
		{
			if (path_to->pos != before->pos)
			{
				if (pushed_path == false)
				{
					path_to->parent = start;
					path_to->cost_so_far = start->cost_so_far + abs(start->pos.x - path_to->pos.x) * DIAGONAL_COST;
					path_to->CalculatePriority(destination);
				}
				before->parent = path_to;
				before->cost_so_far = abs(path_to->pos.x - before->pos.x) * STRAIGHT_COST + path_to->cost_so_far;
			}
			else
			{
				before->parent = start;
				before->cost_so_far = start->cost_so_far + abs(start->pos.x - before->pos.x) * DIAGONAL_COST;
			}
		}
		else
		{
			before->parent = start;
			before->cost_so_far = start->cost_so_far + abs(start->pos.x - before->pos.x) * STRAIGHT_COST;
		}
	}
	else
	{
		before->parent = start;
		before->cost_so_far = start->cost_so_far + abs(start->pos.x - before->pos.x) * STRAIGHT_COST;
	}

	before->CalculatePriority(destination);

	PathNode* after = new PathNode();
	after->pos = after_pos;
	after->parent = before;
	after->cost_so_far = before->cost_so_far + DIAGONAL_COST;
	after->CalculatePriority(destination);

	ForcedNeighbour corner(before, after);

	std::list<ForcedNeighbour>::iterator pos;

	if (CheckIfInFrontier(corner, pos) == false)
	{
		if (pushed_path == false && path_to != nullptr && path_to->pos != start->pos && path_to->pos != before->pos)
		{
			PushToVisited(path_to);
			pushed_path = true;
		}

		PushToVisited(before);
		PushToVisited(after);
		frontier.push_back(corner);

		before = new PathNode(before->pos);
	}
	else if (after->cost_so_far < pos->after->cost_so_far)
	{
		if (pushed_path == false && path_to != nullptr && path_to->pos != start->pos && path_to->pos != before->pos)
		{
			PushToVisited(path_to);
			pushed_path = true;
		}

		PushToVisited(before);
		PushToVisited(after);

		before = new PathNode(before->pos);

		ChangeCosts(after, after->cost_so_far);
	}
}

bool j1PathFinding::GetLowestFN(ForcedNeighbour& fn)
{
	bool ret = false;
	bool all_opened = true;
	fn = frontier.front();

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if ((it->opened == false))
		{
			fn = *it;
			all_opened = false;
		}

	if (all_opened)
		return false;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if ((it->opened == false) && (it->after->priority <= fn.after->priority))
			fn = *it;

	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if (it->after == fn.after && it->before == fn.before && it->opened == false)
		{
			it->opened = true;
			return true;
		}
	return false;
}

bool j1PathFinding::FrontierFinished()
{
	if (frontier.empty())
		return true;

	for (std::list<ForcedNeighbour>::iterator i = frontier.begin(); i != frontier.end(); ++i)
		if (i->opened != true)
			return false;

	return true;
}

void j1PathFinding::FillPathVec(std::vector<iPoint>& vec)
{
	vec.clear();

	const PathNode* it = destination;

	while (it->parent != nullptr)
	{
		vec.push_back(it->pos);
		it = it->parent;
	}

	last_path = vec;
}

void j1PathFinding::ChangeCosts(PathNode * from, float new_cost)
{
	for (int i = 0; i > visited.size(); i++)
	{
		if (visited[i]->parent->pos == from->pos)
		{
			int difference = visited[i]->cost_so_far - from->cost_so_far;
			ChangeCosts(visited[i], new_cost + difference);
		}
	}

	from->cost_so_far = new_cost;
	from->CalculatePriority(destination->pos);
}

void j1PathFinding::OpenOrigin()
{
	//open origin in 4 diagonals
	CheckForTiles(origin, 1, 1, destination->pos);
	CheckForTiles(origin, -1, 1, destination->pos);
	CheckForTiles(origin, -1, -1, destination->pos);
	CheckForTiles(origin, 1, -1, destination->pos);
}

bool j1PathFinding::CheckIfInFrontier(ForcedNeighbour & FN, std::list<ForcedNeighbour>::iterator& pos)
{
	for (std::list<ForcedNeighbour>::iterator it = frontier.begin(); it != frontier.end(); ++it)
		if (it->after->pos == FN.after->pos && it->before->pos == FN.before->pos)
		{
			pos = it;
			return true;
		}

	return false;
}

void j1PathFinding::DestinationReached(PathNode* destination, const PathNode* start,bool& diagonal_pushed, PathNode* path_to)
{
	if (destination->cost_so_far < this->destination->cost_so_far || destination_reached == false)
	{		
		if (path_to != nullptr && path_to->pos != start->pos)		
		{
			path_to->cost_so_far = start->cost_so_far + abs(destination->pos.x - start->pos.x) * DIAGONAL_COST;
			path_to->parent = start;

			if (diagonal_pushed == false)
			{
				path_to->parent = start;
				PushToVisited(path_to);
				diagonal_pushed = true;
			}

			destination->parent = path_to;
			destination->cost_so_far = path_to->cost_so_far + abs(destination->pos.x - start->pos.x) * STRAIGHT_COST + abs(destination->pos.y - start->pos.y) * STRAIGHT_COST;
		}
		else
		{
			destination->parent = start;
			destination->cost_so_far = start->cost_so_far + abs(destination->pos.x - start->pos.x) * DIAGONAL_COST;
		}

		if (destination_reached == false)
		{
			PushToVisited(destination);
			destination_reached = true;
		}
		else
		{
			for (int it = 0; it < visited.size(); it++)
			{
				if (visited[it]->pos == this->destination->pos)
				{
					visited[it] = destination;
				}
			}
		}

		this->destination = destination;
	}
}

void j1PathFinding::DeleteIfNotPushed(PathNode *& ptr)
{
	bool in_vec = false;

	for (int i = 0; i < visited.size(); i++)
		if (ptr == visited[i])
		{
			in_vec = true;
			break;
		}

	if (!in_vec)
	{
		delete ptr;
		ptr = nullptr;
	}
}


// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------

PathNode::PathNode() : cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(-1, -1), parent(nullptr)
{}

PathNode::PathNode(iPoint pos): cost_so_far(0.0f), distance(0.0f), priority(0.0f), pos(pos), parent(nullptr)
{}

PathNode::PathNode(float cost, float dist, const iPoint& pos, const PathNode* parent) : cost_so_far(cost), distance(dist), priority(cost + dist), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : cost_so_far(node.cost_so_far), distance(node.distance), priority(node.priority), pos(node.pos), parent(node.parent)
{}

PathNode::~PathNode()
{}

float PathNode::CalculateDistance(iPoint destination)
{
	int sr = (destination.x - pos.x) * (destination.x - pos.x) + (destination.y - pos.y) * (destination.y - pos.y);
	distance = (float) sqrt(sr);
	return distance;
}

float PathNode::CalculatePriority(const iPoint& destination)
{	
	priority = CalculateDistance(destination) + cost_so_far;
	return priority;
}

const PathNode & PathNode::operator=(const PathNode & node)
{
	this->cost_so_far = node.cost_so_far;
	this->distance = node.distance;
	this->parent = node.parent;
	this->pos = node.pos;

	return *this;
}

inline bool PathNode::operator>(const PathNode & rhs) const
{
	if (this->distance + this->cost_so_far > rhs.cost_so_far + rhs.distance)
		return true;
	return false;
}

inline bool PathNode::operator<(const PathNode & rhs) const
{
	if (this->distance + this->cost_so_far < rhs.cost_so_far + rhs.distance)
		return true;
	return false;
}

inline bool PathNode::operator==(const PathNode & rhs) const
{
	if (this->distance == rhs.distance && this->cost_so_far == rhs.cost_so_far && this->parent == rhs.parent && this->pos == rhs.pos)
		return true;
	return false;
}

//ForcedNeighbour
ForcedNeighbour::ForcedNeighbour(PathNode* before, PathNode* after): before(before), after(after), opened(false)
{}

ForcedNeighbour::ForcedNeighbour(): before(nullptr), after(nullptr)
{}

bool ForcedNeighbour::operator>(const ForcedNeighbour & rhs) const
{
	if (this->after->priority > rhs.after->priority)
		return true;
	return false;
}

bool ForcedNeighbour::operator<(const ForcedNeighbour & rhs) const
{
	if (this->after->priority < rhs.after->priority)
		return true;
	return false;
}

const ForcedNeighbour & ForcedNeighbour::operator=(const ForcedNeighbour & rhs)
{
	before = rhs.before;
	after = rhs.after;

	return *this;
}
