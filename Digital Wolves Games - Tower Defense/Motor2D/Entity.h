#ifndef _ENTITY
#define _ENTITY

#include "p2Point.h"

enum ENTITY_TYPE
{
	NO_ENTITY = 0,
	BUILDING,
	UNIT
};

class Entity
{
private:
	bool to_delete;
	fPoint position;
	int h;
	int w;
	int damage;
	enum ENTITY_TYPE entity_type;

public:
	Entity();
	~Entity();

	virtual void Update() = 0;
	virtual void AI() = 0;
	virtual void Draw() = 0;

	void Die();//to_delete = true

			   //Geters
	bool ToDelete() const;
	ENTITY_TYPE GetEntityType() const;
};
#endif
