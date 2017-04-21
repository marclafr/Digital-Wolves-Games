#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include <vector>
#include <list>

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1PerfTimer.h"


#define DEFAULT_PATH_LENGTH 0
#define INVALID_WALK_CODE 255

//JPS
#define DIAGONAL_COST sqrt(2)
#define STRAIGHT_COST 1.0f
#define FIND_RADIUS 10

// --------------------------------------------------
// Recommended reading:
// Intro: http://www.raywenderlich.com/4946/introduction-to-a-pathfinding
// Details: http://theory.stanford.edu/~amitp/GameProgramming/
// --------------------------------------------------
struct PathNode;

//class Corner;
struct CornerCompare;

struct PathNode;
struct Path;
struct PathList;

struct ForcedNeighbour;

class j1PathFinding : public j1Module
{
public:

	j1PathFinding();

	// Destructor
	~j1PathFinding();

	// Called before quitting
	bool CleanUp();
	void CleanUpJPS();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);
	void SetConstructibleMaps(uint width, uint height, uchar* data, uchar* data2);

	//float CreatePath(const iPoint & origin, const iPoint & destination);
	// To request all tiles involved in the last generated path
	const std::vector<iPoint>* GetLastPath() const;

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;
	bool IsConstructible_ally(const iPoint& pos) const;
	bool IsConstructible_neutral(const iPoint& pos) const;
	void MakeNoConstruible_ally(const iPoint& pos);
	void MakeNoConstruible_neutral(const iPoint& pos);
	void MakeConstruible_ally(const iPoint& pos);
	void MakeConstruible_neutral(const iPoint& pos);
	void MakeNoWalkable(const iPoint& pos);
	void MakeWalkable(const iPoint& pos);

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;
	uchar GetTileAtConstructible_ally(const iPoint& pos) const;
	uchar GetTileAtConstructible_neutral(const iPoint& pos) const;

	PathNode* GetPathNode(int x, int y);

	//JPS
	bool CalculatePath(iPoint origin,const iPoint& destination, std::vector<iPoint>& vec_to_fill);

private:

	iPoint FindNearestWalkable(const iPoint& origin);
	bool GetLowestFN(ForcedNeighbour& fn);
	bool FrontierFinished();
	void FillPathVec(std::vector<iPoint>& vec);
	void ChangeCosts(PathNode* from, float new_cost);
	void OpenOrigin();
	bool CheckIfInFrontier(ForcedNeighbour& FN, std::list<ForcedNeighbour>::iterator& pos);
	void FoundForcedNeighbour(PathNode*& before, iPoint after_pos,const PathNode* start, iPoint destination, bool& pushed_path, PathNode* path_to = nullptr);
	void DestinationReached(PathNode* destination,const PathNode* start, bool& diagonal_pushed, PathNode* path_to = nullptr);
	void DeleteIfNotPushed(PathNode*& ptr);
	bool CheckForTiles(const PathNode* start, int dx, int dy,const iPoint& destination);	
	//JPS end

private:

	// size of the map
	uint width;
	uint height;

	// all map walkability values [0..255]
	uchar* map = nullptr;
	PathNode* node_map = nullptr;
	uchar* constructible_map_ally = nullptr;
	uchar* constructible_map_neutral = nullptr;

	// we store the created path here
	std::vector<iPoint> last_path;

	//JPS
	std::list<ForcedNeighbour> frontier;
	std::vector<PathNode*> visited;
	bool destination_reached = false;
	PathNode* origin = nullptr;
	PathNode* destination = nullptr;
};

// ---------------------------------------------------------------------
// Pathnode: Helper struct to represent a node in the path creation
// ---------------------------------------------------------------------

//JPS

struct ForcedNeighbour
{
	PathNode* before;
	PathNode* after;
	bool opened;

	ForcedNeighbour(PathNode* before, PathNode* after);
	ForcedNeighbour();

	bool operator > (const ForcedNeighbour& rhs) const;
	bool operator < (const ForcedNeighbour& rhs) const;
	const ForcedNeighbour& operator = (const ForcedNeighbour& rhs);
};

struct PathNode
{
	// Convenient constructors
	PathNode();
	PathNode(iPoint pos);
	PathNode(float cost, float dist, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	~PathNode();
	
	float CalculateDistance(iPoint destination);
	float CalculatePriority(const iPoint& destination);

	const PathNode& operator = (const PathNode& node);

	inline bool operator > (const PathNode & rhs) const;
	inline bool operator < (const PathNode & rhs) const;
	inline bool operator == (const PathNode & rhs) const;

	// -----------
	float cost_so_far;
	float distance;
	float priority;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end
};

#endif // __j1PATHFINDING_H__