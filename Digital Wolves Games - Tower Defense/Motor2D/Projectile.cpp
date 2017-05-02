#include "Projectile.h"
#include <math.h>


Projectile::Projectile(fPoint initialpos, Entity * target, int damage, float TimeInSecs,int Startheight, int Curveheight, PROJECTILE_TYPE type) : StartPos(initialpos), Damage(damage),Target(target),StartHeight(Startheight),CurveHeight(Curveheight)
{
	LastPos = Target->GetPosition();
	Diferential = 1 / TimeInSecs;
	PreActualPos = initialpos;
	switch (type)
	{
	case P_BASIC_ARROW:
		SetRect({0,0,45,8});
		pivot = { 22.5, 4 };
		break;
	case P_FIRE_ARROW:
		SetRect({ 0,9,50,19 });
		pivot = { 25, 9.5 };
		break;
	case P_ICE_ARROW:
		SetRect({ 0,29,50,19 });
		pivot = { 25, 9.5 };
		break;
	case P_AIR_ARROW:
		SetRect({ 0,49,50,19 });
		pivot = { 25, 9.5 };
		break;
	case P_CANNONBALL:
		SetRect({ 0,70,23,25 });
		pivot = { 11.5, 12.5 };
		break;
	default:
		break;
	}
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	if (Target != nullptr) LastPos = Target->GetPosition();


	fPoint initial_point = { StartPos.x,StartPos.y - StartHeight };

	fPoint mid_point = { (initial_point.x + LastPos.x) / 2,((initial_point.y + LastPos.y) / 2) - CurveHeight };

	ActualPos.x = ((1 - ProjectilePos)*(1 - ProjectilePos)*initial_point.x) + ((2 * ProjectilePos)*(1 - ProjectilePos)*mid_point.x) + ((ProjectilePos*ProjectilePos)*LastPos.x);
	ActualPos.y = ((1 - ProjectilePos)*(1 - ProjectilePos)*initial_point.y) + ((2 * ProjectilePos)*(1 - ProjectilePos)*mid_point.y) + ((ProjectilePos*ProjectilePos)*LastPos.y);


	fPoint first_point = { initial_point.x * (1 - ProjectilePos) + mid_point.x * ProjectilePos, initial_point.y * (1 - ProjectilePos) + mid_point.y * ProjectilePos };
	fPoint second_point = { mid_point.x * (1 - ProjectilePos) + LastPos.x * ProjectilePos, mid_point.y * (1 - ProjectilePos) + LastPos.y * ProjectilePos };

	fPoint vector = { second_point.x - first_point.x, second_point.y - first_point.y };
	angle = atan2(vector.y, vector.x)*57.9257795;
	ProjectilePos += Diferential;
	if (ProjectilePos > 1) ProjectilePos = 1;

	Draw();
	
	if (Target != nullptr && ProjectilePos == 1) 
		Target->Damaged(Damage);


}

void Projectile::Draw()
{
	//alfa = Math.atan2(by - ay, bx - ax);

	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), ActualPos.x, ActualPos.y, &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y, 1, angle, false);
}

int Projectile::GetProjectilePos() const
{
	return ProjectilePos;
}

int Projectile::GetDamage() const
{
	return Damage;
}

void Projectile::SetRect(SDL_Rect rect)
{
	this->rect = rect;
}
