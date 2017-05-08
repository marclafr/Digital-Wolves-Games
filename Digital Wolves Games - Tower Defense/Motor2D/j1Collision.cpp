#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include "j1Collision.h"

j1Collision::j1Collision()
{
	name.assign("collision");
}

j1Collision::~j1Collision() {}

bool j1Collision::Update(float dt)
{	
	for (int i = 0; i < App->entity_manager->entity_array.size(); i++)
		if (App->entity_manager->entity_array[i]->GetEntityType() == E_UNIT)
		{
			iPoint pos = App->map->WorldToMap(App->entity_manager->entity_array[i]->GetX(), App->entity_manager->entity_array[i]->GetY());
			Unit* unit_1 = (Unit*)App->entity_manager->entity_array[i];
			
			//Check colisions between units
			for (int j = 0; j < App->entity_manager->entity_array.size(); j++)
				if (App->entity_manager->entity_array[i] != App->entity_manager->entity_array[j] && App->entity_manager->entity_array[j]->GetEntityType() == E_UNIT)
					if (DoUnitsIntersect((Unit*)App->entity_manager->entity_array[i], (Unit*)App->entity_manager->entity_array[j]) == true)
					{
						//Collision detected
						Unit* unit_2 = (Unit*)App->entity_manager->entity_array[j];
						if (unit_1->GetPriority() > unit_2->GetPriority() && unit_1->GetAction() == A_IDLE && unit_2->GetAction() == A_IDLE)
							SplitUnits((Unit*)App->entity_manager->entity_array[i]);
					}
		}
	return true;
}

bool j1Collision::AbleToBuild(iPoint pos)
{
	for (int i = 0; i < App->entity_manager->entity_array.size(); i++)
	{
		iPoint tile = App->map->WorldToMap(App->entity_manager->entity_array[i]->GetX(), App->entity_manager->entity_array[i]->GetY());
		if (tile == App->map->WorldToMap(pos.x, pos.y))
			return false;
	}
	return true;
}

bool j1Collision::DoUnitsIntersect(Unit* unit1, Unit* unit2)
{
	//float distance_x = unit1->GetX() - unit2->GetX();
	//float distance_y = unit1->GetY() - unit2->GetY();
	//return (sqrt(distance_x * distance_x + distance_y * distance_y) < unit1->GetUnitRadius() + unit2->GetUnitRadius());
	return unit1->GetUnitCircle().Overlap(&unit2->GetUnitCircle());
}

iPoint j1Collision::FindClosestWalkable(Unit* unit)
{
	bool found = false;

	iPoint tile = App->map->WorldToMap(unit->GetX(), unit->GetY());
	iPoint origin = tile;

	int dist = 1;

	while (!found)
	{
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x -= dist;
		tile.x -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//diagonals
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y += dist;
		tile.y += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.x += dist;
		tile.x += dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		tile.y -= dist;
		tile.y -= dist;
		if (App->pathfinding->IsWalkable({ tile.x, tile.y }) == true && App->entity_manager->IsUnitInTile(unit, tile) == false)
		{
			found = true;
			break;
		}

		//back to origin and increment the tile distance
		tile.x--;
		tile.y++;
		dist++;
	}

	return App->map->MapToWorld(tile.x, tile.y);
}

void j1Collision::SplitUnits(Unit * unit1)
{
	unit1->GoTo(FindClosestWalkable(unit1));
}
