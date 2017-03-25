#ifndef _RESOURCES
#define _RESOURCES

#include "SDL\include\SDL.h"

class Resources 
{
private:
	int wood = 0;
	int food = 0;
	int gold = 0;
	int stone = 0;
public:
	Resources();
	~Resources();

	int GetWood();
	int GetFood();
	int GetGold();
	int GetStone();

	void AddWood(int adding);
	void AddFood(int adding);
	void AddGold(int adding);
	void AddStone(int adding);

	void UseWood(int used);
	void UseFood(int used);
	void UseGold(int used);
	void UseStone(int used);
};



#endif //_RESOURCES_

