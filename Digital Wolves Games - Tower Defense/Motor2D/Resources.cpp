#include "Resources.h"

Resources::Resources() {
	wood = 2000;
	food = 2000;
	gold = 2000;
	stone = 2000;
}

Resources::~Resources()
{}

int Resources::GetWood() 
{
	return wood;
}
int Resources::GetFood()
{
	return food;
}
int Resources::GetGold()
{
	return gold;
}
int Resources::GetStone()
{
	return stone;
}

void Resources::AddWood(int adding) 
{
	wood += adding;
}
void Resources::AddFood(int adding)
{
	food += adding;
}
void Resources::AddGold(int adding)
{
	gold += adding;
}
void Resources::AddStone(int adding)
{
	stone += adding;
}

void Resources::UseWood(int used)
{
	wood -= used;
}
void Resources::UseFood(int used)
{
	food -= used;
}
void Resources::UseGold(int used)
{
	gold -= used;

}
void Resources::UseStone(int used)
{
	stone -= used;
}