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
		break;
	case P_FIRE_ARROW:
		SetRect({ 0,9,50,19 });
		break;
	case P_ICE_ARROW:
		SetRect({ 0,29,50,19 });
		break;
	case P_AIR_ARROW:
		SetRect({ 0,49,50,19 });
		break;
	case P_CANNONBALL:
		SetRect({ 0,70,23,25 });
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

	if(LastPos.y > StartPos.y)
		angle = atan2(ActualPos.y - StartPos.y, ActualPos.x- StartPos.x)*360/PI;
	else 
		angle = -atan2(ActualPos.y - StartPos.y, ActualPos.x - StartPos.x) * 360 / PI;
	ProjectilePos += Diferential;
	if (ProjectilePos > 1) ProjectilePos = 1;

	Draw();
	
	if (Target != nullptr && ProjectilePos == 1) 
		Target->Damaged(Damage);


}

void Projectile::Draw()
{
	//alfa = Math.atan2(by - ay, bx - ax);

	App->render->Blit(App->tex->GetTexture(T_ARROW_BOMB), ActualPos.x, ActualPos.y, &rect, SDL_FLIP_NONE, 0, 0, 1, angle, false);
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
