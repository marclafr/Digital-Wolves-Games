#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1UIManager.h"
#include "UIHUDMinimap.h"

#define MINIMAP_WIDTH 237
#define MINIMAP_HEIGHT 164
#define MINIMAP_POSITIOIN {1584,911,MINIMAP_WIDTH,MINIMAP_HEIGHT}
#define LT {1584,992}
#define RT {1909,992}
#define TT {1746,911}
#define LD {1909,993}
#define RD {1584,993}
#define TD {1747,1074}

UIHUDMinimap::UIHUDMinimap(UICOMPONENT_TYPE type) : UIComponents(type)
{
	this->Set(MINIMAP_POSITIOIN, {0,0,0,0});
	rect_map = { (App->map->data.width * App->map->data.tile_width) / 2, (App->map->data.height * App->map->data.tile_height) /2 , App->map->data.width * App->map->data.tile_width,  App->map->data.height * App->map->data.tile_height };
	quad_minimap_position = TransformPointFromMap(App->render->camera->GetCenter());
	t_top.SetPoints(LT, RT, TT);
	t_down.SetPoints(LD, RD, TD);
	quad_atlas = { 200, 300, 10, 10 };
}

UIHUDMinimap::~UIHUDMinimap()
{
	DELETE_PTR(minimap_image);
}

bool UIHUDMinimap::Update()
{
	iPoint mouse_pos;
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	mouse_pos = App->render->ScreenToWorld(mouse_pos.x, mouse_pos.y);

	if (t_top.PointInTriangle(mouse_pos) || t_down.PointInTriangle(mouse_pos))
	{
		if (App->input->GetMouseButtonDown(MK_LEFT) == KEY_DOWN || App->input->GetMouseButtonDown(MK_LEFT) == KEY_REPEAT)
			App->render->camera->SetPosition(TransformPointFromMinimap(mouse_pos));
	}
	else
	{
		iPoint minimap_point = TransformPointFromMap(App->render->camera->GetCenter());
		if (minimap_point != quad_minimap_position)
			quad_minimap_position = minimap_point;
	}
	Draw();
	return true;
}

void UIHUDMinimap::Draw()
{
	App->render->PushUISprite((SDL_Texture*)App->uimanager->GetAtlas(), quad_minimap_position.x, quad_minimap_position.y, &quad_atlas);
}

iPoint UIHUDMinimap::TransformPointFromMap(iPoint map_point)
{
	iPoint minimap_point;
	float xi = GetPosRect().w*((float)(map_point.x  - rect_map.x) / (float)rect_map.w);
	minimap_point.x = GetPosRect().x + xi;
	float yi = GetPosRect().h*((((float)rect_map.y - map_point.y) / (float)rect_map.w));
	minimap_point.y = GetPosRect().y - yi;
	return minimap_point;
}
iPoint UIHUDMinimap::TransformPointFromMinimap(iPoint minimap_point)
{
	iPoint map_point;
	float xi = rect_map.w*((float)(minimap_point.x - GetPosRect().x) / (float)GetPosRect().w);
	map_point.x = rect_map.x + xi;
	float yi = rect_map.h*((float)(GetPosRect().y - minimap_point.y) / (float)GetPosRect().h);
	map_point.y = rect_map.y - yi;
	return map_point;
}
void MinimapTriangle::SetPoints(iPoint left, iPoint right, iPoint top)
{
	this->left = left;
	this->right = right;
	this->top = top;
}

float MinimapTriangle::sign(iPoint p1, iPoint p2, iPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool MinimapTriangle::PointInTriangle(iPoint pt)
{
	bool b1, b2, b3;

	b1 = sign(pt, left, right) < 0.0f;
	b2 = sign(pt, right, top) < 0.0f;
	b3 = sign(pt, top, left) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}