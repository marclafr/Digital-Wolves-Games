#pragma once

#ifndef __UIHUDMINIMAP_H__
#define __UIHUDMINIMAP_H__

#include "UIComponents.h"
#include "p2Point.h"

class IsoRect;

class UIHUDMinimap : public UIComponents
{
private:
	UIComponents* minimap_image = nullptr;

	IsoRect* minimap;

	iPoint quad_minimap_position;

	SDL_Rect rect_map;
	SDL_Rect quad_atlas;

public:
	UIHUDMinimap(UICOMPONENT_TYPE type = UIT_UIHUDMINIMAP);
	~UIHUDMinimap();

	bool Update();

	void Draw();

	iPoint WorldToMinimap(fPoint world_point);
	iPoint MinimapToWorld(fPoint minimap_point);
};

#endif // __UIHUDMINIMAP_H__