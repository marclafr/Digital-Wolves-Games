#include "Entity.h"
#include "j1Textures.h"

Entity::Entity(ENTITY_TYPE entity_type, fPoint pos, Side side): to_delete (false), entity_type(entity_type), position(pos), side(side)
{}

Entity::~Entity()
{}

void Entity::Die()
{
	to_delete = true;
}

bool Entity::ToDelete() const
{
	return to_delete;
}

ENTITY_TYPE Entity::GetEntityType() const
{
	return entity_type;
}

ENTITY_STATUS Entity::GetEntityStatus() {

	return entity_status;
}

void Entity::SetEntityStatus(ENTITY_STATUS status)
{
	entity_status = status;
}

void Entity::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

const float Entity::GetX() const
{
	return position.x;
}

const float Entity::GetY() const
{
	return position.y;
}

const fPoint Entity::GetPosition() const
{
	return position;
}

const int Entity::GetHp() const
{
	return hp;
}

const int Entity::GetArmor() const
{
	return armor;
}

void Entity::IncreaseArmor(int extra_defense)
{
	armor += extra_defense;
}

const iPoint Entity::GetPivot() const
{
	return pivot;
}

const SDL_Rect Entity::GetRect() const
{
	return rect;
}

const TextureID Entity::GetTextureID() const
{
	return texture_id;
}

const Side Entity::GetSide() const
{
	return side;
}

void Entity::SetArmor(int new_armor)
{
	armor = new_armor;
}

void Entity::SetHp(int new_hp)
{
	hp = new_hp;
}
const int Entity::GetAttack() const {

	return attack;

}
void Entity::SetPivot(int x, int y)
{
	pivot.x = x;
	pivot.y = y;
}

void Entity::SetRect(SDL_Rect rect)
{
	this->rect = rect;
}

void Entity::SetTextureID(TextureID id)
{
	texture_id = id;
}

void Entity::SetSide(Side side)
{
	this->side = side;
}

void Entity::SetToDelete()
{
	to_delete = true;
}

void Entity::SetAttack(int new_attack)
{
	attack = new_attack;
}
void Entity::Attack(Entity* entity)
{
	entity->SetHp(entity->hp - this->attack);
}

void Entity::Damaged(int dmg)
{
	hp -= (dmg - armor);
}

void Entity::UpgradeUnit(int plushealth) {

	this->SetHp(GetHp() + plushealth);

}
