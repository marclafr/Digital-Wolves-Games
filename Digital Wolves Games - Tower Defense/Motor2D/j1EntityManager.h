#ifndef _j1EntityManager_
#define _j1EntityManager_

#include "j1Module.h"
#include "j1Textures.h"
#include "p2Point.h"
#include "Entity.h"
#include "Units.h"
#include "Resources.h"
#include "Buildings.h"
#include "Wall.h"
#include <vector>
#include "SDL/include/SDL_rect.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* CreateUnit(UNIT_TYPE u_type, fPoint pos, Side side);
	Entity* CreateBuilding(BUILDING_TYPE b_type, fPoint pos, Side side);
	Entity* CreateResource(RESOURCE_TYPE r_type, fPoint pos);
	Entity* CreateWall(WALL_TYPE r_type, fPoint pos);
	void SelectInQuad(const SDL_Rect& select_rect);
	void UnselectEverything();

	void DeleteEntity(Entity* ptr); // will call other delete methods
	void DeleteUnit(Unit* ptr);
	void DeleteBuilding(Entity* ptr);
	void DeleteResource(Entity* ptr);
	void DeleteWall(Entity* ptr);

	void BlitEnemyDeathCount();
	void EnemyDead();

	Entity* CheckForCombat(iPoint position, int range, Side side);
	Entity* CheckForObjective(iPoint position, int vision_range, Side side);
	std::vector<Entity*> GetEntityVector();

	bool IsUnitInTile(const Unit* unit, const iPoint tile)const;
	int GetScore();
	void AddScore(int points);
	void ResetScores();

private:

	int enemies_killed = 0;
	int score = 0;
	bool enemy_killed = true;
	char text_num_kills[256];
	char text_score[256];
	char time_left[256];
	SDL_Texture* num_kills_texture = nullptr;
	SDL_Texture* score_texture = nullptr;
	SDL_Texture* time_texture = nullptr;


	//Textura provisional para sprites por si no se cargan en animation
	//SDL_Texture* sprites;

	//lista para "almacenar" unidades, puede ser cambiada a array etc.
	std::vector<Entity*> entity_array;

	//TODO:Must change this
	int priority = 0;

};
#endif //_j1EntityManager_

