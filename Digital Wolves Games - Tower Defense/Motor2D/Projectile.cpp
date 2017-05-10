#include <math.h>
#include "Projectile.h"
#include "j1Animation.h"
#include "IsoPrimitives.h"
#include "Camera.h"
#include "j1Render.h"

Projectile::Projectile(fPoint initialpos, Entity * target, int damage, float TimeInSecs, int Startheight, int Curveheight, PROJECTILE_TYPE type) : StartPos(initialpos), Damage(damage), Target(target), StartHeight(Startheight), CurveHeight(Curveheight), projectile_type(type)
{
	//XMLPROJECTILE TODO PIVOTS??? (XML has no pivots on arrows/bombs)
	LastPos = Target->GetPosition();
	Diferential = 1 / TimeInSecs;
	PreActualPos = initialpos;
	switch (type)
	{
	case P_BASIC_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_ARROW));
		break;
	case P_FIRE_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_ARROW));
		break;
	case P_ICE_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_ARROW));
		break;
	case P_AIR_ARROW:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_ARROW));
		break;
	case P_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_SIMPLE_BOMB));
		break;
	case P_FIRE_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_BOMB));
		floor_effect = true;
		break; 
	case P_ICE_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_BOMB));
		floor_effect = true;
		break;
	case P_AIR_CANNONBALL:
		projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_AIR_BOMB));
		break;
	default:
		projectile_anim = nullptr;
		break;
	}
}

Projectile::~Projectile()
{
	if (projectile_anim != nullptr)
		delete projectile_anim;
}

void Projectile::Update()
{
	if (Target != nullptr)
		LastPos = Target->GetPosition();

	fPoint initial_point = { StartPos.x,StartPos.y - StartHeight };

	fPoint mid_point = { (initial_point.x + LastPos.x) / 2,((initial_point.y + LastPos.y) / 2) - CurveHeight };

	ActualPos.x = ((1 - ProjectilePos)*(1 - ProjectilePos)*initial_point.x) + ((2 * ProjectilePos)*(1 - ProjectilePos)*mid_point.x) + ((ProjectilePos*ProjectilePos)*LastPos.x);
	ActualPos.y = ((1 - ProjectilePos)*(1 - ProjectilePos)*initial_point.y) + ((2 * ProjectilePos)*(1 - ProjectilePos)*mid_point.y) + ((ProjectilePos*ProjectilePos)*LastPos.y);

	fPoint first_point = { initial_point.x * (1 - ProjectilePos) + mid_point.x * ProjectilePos, initial_point.y * (1 - ProjectilePos) + mid_point.y * ProjectilePos };
	fPoint second_point = { mid_point.x * (1 - ProjectilePos) + LastPos.x * ProjectilePos, mid_point.y * (1 - ProjectilePos) + LastPos.y * ProjectilePos };

	fPoint vector = { second_point.x - first_point.x, second_point.y - first_point.y };
	angle = atan2(vector.y, vector.x) * 57.9257795;
	ProjectilePos += Diferential;

	if (ProjectilePos > 1)
		ProjectilePos = 1;

	if (Target != nullptr && ProjectilePos == 1 && dest_reached == false)
	{
		switch (projectile_type)
		{
		case P_BASIC_ARROW:
		case P_FIRE_ARROW:
		case P_ICE_ARROW:
		case P_AIR_ARROW:
			Target->Damaged(Damage);
			dest_reached = true;
			break;
		case P_CANNONBALL:
		case P_FIRE_CANNONBALL:
		case P_AIR_CANNONBALL:
			Target->Damaged(Damage);
			AreaDamage(Damage, { (int)Target->GetX(), (int)Target->GetY() }, AREA_DMG_RADIUS);
			element_terrain_pos = Target->GetPosition();
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_EXPLOSION));
			dest_reached = true;
			PrintElementTerrainTimer.Start();
			break;
		case P_ICE_CANNONBALL:
			Target->Damaged(Damage);
			AreaDamage(Damage, { (int)Target->GetX(), (int)Target->GetY() }, AREA_DMG_RADIUS);
			element_terrain_pos = Target->GetPosition();
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_EXPLOSION));
			dest_reached = true;
			PrintElementTerrainTimer.Start();
			break;
		default:
			break;
		}
		if (projectile_type == P_ICE_ARROW)
		{
			Unit* unit_target = (Unit*)Target; 
			unit_target->SlowUnit();
		}
	}

	Draw();
}

void Projectile::Draw()
{
	float pett = PrintElementTerrainTimer.ReadMs() / 1000.0f;
	if (pett < ELEMENT_TERRAIN_TIME && dest_reached == true)
		PrintElementTerrain(projectile_type, element_terrain_pos);
	

	//alfa = Math.atan2(by - ay, bx - ax);
	if (projectile_anim != nullptr)
		projectile_anim->Update(rect, pivot);
	if (dest_reached == false)
		if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + ActualPos.x, App->render->camera->GetPosition().y + ActualPos.y))
			App->render->PushInGameSprite(App->tex->GetTexture(T_ARROW_BOMB), ActualPos.x, ActualPos.y, &rect, SDL_FLIP_HORIZONTAL, pivot.x, pivot.y, 1, angle, false);
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

AnimationManager * Projectile::GetProjectileAnim()
{
	return projectile_anim;
}

void Projectile::AreaDamage(int damage, iPoint center, int radius)
{
	Circle circle(center, radius);
	//TODO: when quadtree is finished
	/*
	if (projectile_type == P_ICE_CANNONBALL)
	{
		Unit* target_unit = (Unit*)Target;
		target_unit->SlowUnit();
	}
	*/
}

void Projectile::PrintElementTerrain(PROJECTILE_TYPE element, fPoint center)
{
	if (floor_effect == true && projectile_anim->Finished() == true)
	{
		switch (projectile_type)
		{
		case P_FIRE_CANNONBALL:
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_FIRE_FLOOR));
			break;
		case P_ICE_CANNONBALL:
			delete projectile_anim;
			projectile_anim = new AnimationManager(App->anim->GetAnimationType(ANIM_ICE_FLOOR));
			break;
		default:
			break;
		}
		floor_effect = false;
	}
	SDL_Rect rect;
	iPoint pivot;
	projectile_anim->Update(rect, pivot);
	if (App->render->camera->InsideRenderTarget(App->render->camera->GetPosition().x + ActualPos.x, App->render->camera->GetPosition().y + ActualPos.y))
	{
		switch (element)
		{
		case P_CANNONBALL:
		case P_FIRE_CANNONBALL:
		case P_ICE_CANNONBALL:
		case P_AIR_CANNONBALL:
			App->render->PushInGameSprite(App->tex->GetTexture(T_EXPLOSIONS_AND_FLOOR), center.x, center.y, &rect, SDL_FLIP_NONE, pivot.x, pivot.y);
			break;
		default:
			break;
		}
	}
}