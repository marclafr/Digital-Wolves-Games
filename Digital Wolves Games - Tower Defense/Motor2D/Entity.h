#ifndef _ENTITY
#define _ENTITY

#include "SDL\include\SDL.h"
#include "p2Point.h"

enum TextureID;

enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	BUILDING,
	UNIT
};

enum Side
{
	ALLY,
	ENEMY
};

enum ENTITY_STATUS
{
	E_SELECTED = 0,
	E_NON_SELECTED
};

class Entity
{
private:
	SDL_Rect rect;
	iPoint pivot;
	bool to_delete;
	fPoint position;
	int hp;
	int armor;
	int attack;
	enum ENTITY_TYPE entity_type;
	enum ENTITY_STATUS entity_status = E_NON_SELECTED;
	enum TextureID texture_id;
	enum Side side;

public:
	Entity(ENTITY_TYPE entity_type, fPoint pos, Side side);
	~Entity();

	virtual void Update() = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;


	virtual void Die();//to_delete = true


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
	const iPoint GetPivot() const;
	const SDL_Rect GetRect() const;
	const TextureID GetTextureID() const;
	const Side GetSide() const;
	void Attack(Entity*);
	void Damaged(int dmg);

protected:
	//Seters
	void SetArmor(int new_armor);
	void SetHp(int new_hp);
	void SetAttack(int new_attack);
	void SetPivot(int x, int y);
	void SetRect(SDL_Rect rect);
	void SetTextureID(TextureID id);
	void SetSide(Side side);
};
#endif
