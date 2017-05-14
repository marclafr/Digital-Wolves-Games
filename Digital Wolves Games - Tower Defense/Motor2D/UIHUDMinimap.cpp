#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1UIManager.h"
#include "j1EntityManager.h"
#include "IsoPrimitives.h"
#include "j1Scene.h"
#include "UIHUDMinimap.h"

#define MINIMAP_WIDTH 236
#define MINIMAP_HEIGHT 119
#define ISOMINIMAP_POSITIOIN {1243,706}
#define MINIMAP_POSITION {1126, 647, MINIMAP_WIDTH, MINIMAP_HEIGHT}
#define MINIMAP_ATLAS {926,2572,MINIMAP_WIDTH,MINIMAP_HEIGHT}
#define RED_POINT_ATLAS {1164, 2572,2,2}

UIHUDMinimap::UIHUDMinimap(UICOMPONENT_TYPE type) : UIComponents(type)
{
	this->Set(MINIMAP_POSITION, MINIMAP_ATLAS);

	minimap = new IsoRect(fPoint(ISOMINIMAP_POSITIOIN), MINIMAP_WIDTH, MINIMAP_HEIGHT);

	int map_x = -(App->map->data.width * App->map->data.tile_width / 2.0f);
	int map_y = 0;
	int map_w = App->map->data.width * App->map->data.tile_width;
	int map_h = App->map->data.height * App->map->data.tile_height + 75;
	rect_map = { map_x, map_y , map_w , map_h };
	
	quad_atlas = { 0, 0, 50, 28 };
}

UIHUDMinimap::~UIHUDMinimap()
{
	DELETE_PTR(minimap);
}

bool UIHUDMinimap::Update()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	fPoint mouse_pos(x,y);

	if (minimap->Inside(fPoint(x,y)))
	{
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN)
		{
			mouse_pos.x -= GetPosRect().x;
			mouse_pos.y -= GetPosRect().y;
			iPoint new_camera_pos(MinimapToWorld(mouse_pos));
			new_camera_pos.x += App->render->camera->GetWidth()/2;
			new_camera_pos.y += App->render->camera->GetHeight()/2;
			App->scene->move_camera = true;
			App->scene->camera_new_position = new_camera_pos;
		}
	}
	else
	{
		mouse_pos.x -= App->render->camera->GetPosition().x;
		mouse_pos.y -= App->render->camera->GetPosition().y;
		quad_minimap_position = WorldToMinimap(mouse_pos);
		quad_atlas = { quad_minimap_position.x, quad_minimap_position.y, 100, 56 };
	}

	Draw();
	return true;
}

void UIHUDMinimap::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), GetPosRect().x - App->render->camera->GetPosition().x, GetPosRect().y - App->render->camera->GetPosition().y, &GetAtlasRect());

	//Test unit print pos
	fPoint unit_pos(0,1000);
	unit_pos.x += 3600; // Iso To Rect
	//unit_pos.x -= App->render->camera->GetPosition().x;
	//unit_pos.y -= App->render->camera->GetPosition().y;
	iPoint unit_minimap_pos = WorldToMinimap(unit_pos);
	SDL_Rect red_point = RED_POINT_ATLAS;
	//Draw enemy units points
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), unit_minimap_pos.x - App->render->camera->GetPosition().x, unit_minimap_pos.y - App->render->camera->GetPosition().y, &red_point);
}

iPoint UIHUDMinimap::WorldToMinimap(fPoint world_point)
{
	fPoint minimap_point;
	minimap_point.x = world_point.x / rect_map.w * GetPosRect().w;
	minimap_point.y = world_point.y / rect_map.h * GetPosRect().h;
	minimap_point.x += GetPosRect().x;
	minimap_point.y += GetPosRect().y;
	return iPoint(minimap_point.x, minimap_point.y);
}

iPoint UIHUDMinimap::MinimapToWorld(fPoint minimap_point)
{
	fPoint world_point;
	minimap_point.x -= GetPosRect().w / 2.0f;
	world_point.x = minimap_point.x / GetPosRect().w * rect_map.w * -1;
	world_point.y = minimap_point.y / GetPosRect().h * rect_map.h * -1;
	return iPoint(world_point.x, world_point.y);
}