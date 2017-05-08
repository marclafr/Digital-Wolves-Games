#ifndef _RESOURCES
#define _RESOURCES

#include "SDL/include/SDL_rect.h"
#include "Entity.h"
#include "j1Textures.h"
#include "j1Module.h"
#include "Entity.h"
#include "Units.h"
#include "j1Timer.h"
#include <vector>

enum RESOURCE_TYPE
{
	R_NO_RESOURCE,
	R_WOOD,
	R_STONE,
	R_GOLD,
	R_FOOD
};

class Resources : public Entity
{
private:
	int amount_collected;
	enum RESOURCE_TYPE resource_type;
	float collect_time;
	float build_time;
	j1Timer buildtimer;
	j1Timer CollectTimer;
	bool totallybuilded = false;
public:

	Resources(RESOURCE_TYPE b_type, fPoint pos);
	~Resources();

	void Update(float dt); // defines order

	virtual void AI();
	virtual void Draw();

	const RESOURCE_TYPE GetResourceType() const;
	const double GetBuildTime() const;
	void AddResource(int add);
	bool ReduceCollectTime(float reduction);
	void IncreaseResourceAmount(int amount);
};

#endif

