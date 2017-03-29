#ifndef __GETENTITYINFO__
#define __GETENTITYINFO__

#define TURRET_BUILD 9000

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"

SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type);
const char* GetUnitName(const UNIT_TYPE type);
SDL_Rect GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type);
const char* GetBuildingName(const BUILDING_TYPE type);

const uint GetBuildingPercentage(const Entity* build);
const double GetBuildTotalTime(const BUILDING_TYPE type);
const bool isBuilded(const Entity* build);
SDL_Rect GetAtlasBarBuilding(const uint percentage);

#endif