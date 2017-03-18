#ifndef _ENTITY
#define _ENTITY

#include "SDL\include\SDL.h"
#include "p2Point.h"

enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	BUILDING,
	UNIT
};
enum ENTITY_STATUS
{
	E_SELECTED = 0,
	E_NON_SELECTED
};
class Entity
{
private:
	SDL_Texture* texture; //should later change to enum
	SDL_Rect rect;
	iPoint pivot;
	bool to_delete;
	iPoint position;
	int hp;
	int armor;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = E_SELECTED;
public:
	Entity(ENTITY_TYPE entity_type, iPoint pos);
	~Entity();

	virtual void Update() = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;


	void Die();//to_delete = true


	//Geters
	bool ToDelete() const;

	void SetEntityStatus(ENTITY_STATUS status);
	void SetPosition(float x, float y);

	ENTITY_TYPE GetEntityType() const;
	ENTITY_STATUS GetEntityStatus();
	const int GetHp() const;
	const int GetArmor() const;
	const float GetX() const;
	const float GetY() const;
	const SDL_Texture* GetTexture() const;
	const iPoint GetPivot() const;
	const SDL_Rect GetRect() const;

protected:
	//Seters
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
	void SetTexture(SDL_Texture* text);
	void SetPivot(int x, int y);
	void SetRect(SDL_Rect rect);
};
#endif
