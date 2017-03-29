#ifndef __GETENTITYINFO__
#define __GETENTITYINFO__

#include "Entity.h"
#include "Units.h"
#include "Buildings.h"

SDL_Rect GetUnitIconPositionFromAtlas(const UNIT_TYPE type);
const char* GetUnitName(const UNIT_TYPE type);
SDL_Rect GetBuildingIconPositionFromAtlas(const BUILDING_TYPE type);
const char* GetBuildingName(const BUILDING_TYPE type);

#endif