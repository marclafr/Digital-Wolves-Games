#include "Projectile.h"
#include "j1Animation.h"
#include <math.h>


Projectile::Projectile(fPoint initialpos, Entity * target, int damage, float TimeInSecs,int Startheight, int Curveheight, PROJECTILE_TYPE type) : StartPos(initialpos), Damage(damage),Target(target),StartHeight(Startheight),CurveHeight(Curveheight)
{
	//XMLPROJECTILE TODO PIVOTS??? (XML has no pivots on arrows/bombs)
	LastPos = Target->GetPosition();
	Diferential = 1 / TimeInSecs;
	PreActualPos = initialpos;
	switch (type)
	{
	case P_BASIC_ARROW:
		//arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_ARROW));
		SetRect({ 0,0,36,5 });
		pivot = { 18, 2 };
		break;
	case P_FIRE_ARROW:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_ARROW));
		break;
	case P_ICE_ARROW:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_ARROW));
		//pivot = { 18, 7 };
		break;
	case P_AIR_ARROW:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_ARROW));
		//pivot = { 18, 7 };
		break;
	case P_CANNONBALL:
		//arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_BOMB));
		SetRect({ 0,54,10,10 }); 
		pivot = { 5, 5 };
		break;
	case P_FIRE_CANNONBALL:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_BOMB));
		break; 
	case P_ICE_CANNONBALL:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_BOMB));
		break;
	case P_AIR_CANNONBALL:
		arrow_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_BOMB));
		break;
	default:
		arrow_anim = nullptr;
		break;
	}
}

Projectile::~Projectile()
{
	if (arrow_anim != nullptr)
		delete arrow_anim;
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
	{
		switch (projectile_type)
		{
		case P_BASIC_ARROW:
		case P_FIRE_ARROW:
		case P_ICE_ARROW:
		case P_AIR_ARROW:
			Target->Damaged(Damage);
			break;
		case P_CANNONBALL:
		case P_FIRE_CANNONBALL:
		case P_ICE_CANNONBALL:
		case P_AIR_CANNONBALL:
			Target->Damaged(Damage);
			AreaDamage(Damage, Target->GetPosition(), AREA_DMG_RADIUS);
			break;
		default:
			break;
		}
	}

}

void Projectile::Draw()
{
	//alfa = Math.atan2(by - ay, bx - ax);
	if (arrow_anim != nullptr)
		arrow_anim->Update(rect, pivot);
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

void Projectile::AreaDamage(int damage, fPoint center, int radius)
{
	//TODO: Create elipse and check if any unit is inside, if so, damage them.

}