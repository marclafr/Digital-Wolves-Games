#ifndef __UIHUDRESORCES_H__
#define __UIHUDRESORCES_H__

#include "UIComponents.h"

#include "UILabel.h"

class Resources;

enum RESOURCE_TYPE;

class UIHUDResources : public UIComponents
{
private:
	UILabel* stone;
	uint num_stone;

	UILabel* gold;
	uint num_gold;

	UILabel* food;
	uint num_food;

	UILabel* wood;
	uint num_wood;

	std::list<Resources*>  entities_resources;

public:
	UIHUDResources(UICOMPONENT_TYPE type);
	~UIHUDResources();

	void AddResource(Resources* res);

	bool Update();

	void UpdateResourcesNum();
	void UpdateLabels();
};

#endif // __UIHUDRESORCES_H__