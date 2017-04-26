#define BAR_SCORE_POSITION {360, 310, 703, 19}
#define BAR_SCORE_ATLAS {0, 2585, 703, 19}

#define YELLOWBAR_SCORE_POSITION {365, 313, 698, 11}
#define YELLOWBAR_SCORE_ATLAS {0, 2572, 698, 11}

#define BAR_PIXELS 698

#include "UIHUDScoreBar.h"

#include "j1App.h"
#include "j1Input.h"
#include "j1UIManager.h"
#include "j1Render.h"
#include "Camera.h"
#include "j1EntityManager.h"

#include "UIGetEntitiesInfo.h"

UIHUDScoreBar::UIHUDScoreBar(UICOMPONENT_TYPE type) : UIComponents(type)
{
	Set(BAR_SCORE_POSITION, BAR_SCORE_ATLAS);
	SetInteractive(false);

	score_bar = new UIComponents(UICOMPONENT_TYPE::UIIMAGE);
	score_bar->Set(YELLOWBAR_SCORE_POSITION, YELLOWBAR_SCORE_ATLAS);

	rest_score_bar = YELLOWBAR_SCORE_ATLAS;
}

bool UIHUDScoreBar::Update()
{
	//rest_score_bar.w = ReturnValueBarHPBuilding(BUILDING_TYPE::B_TOWNHALL, townhall->GetHp(), BAR_PIXELS);
	rest_score_bar.w = App->entity_manager->GetScore()/10;

	return true;
}

void UIHUDScoreBar::Draw()
{
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), rect_position.x - App->render->camera->GetPosition().x, rect_position.y - App->render->camera->GetPosition().y, &rect_atlas, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
	App->render->Blit((SDL_Texture*)App->uimanager->GetAtlas(), score_bar->GetPosRect().x - App->render->camera->GetPosition().x, score_bar->GetPosRect().y - App->render->camera->GetPosition().y, &rest_score_bar, SDL_FLIP_NONE, 0, 0, 1.0f, 0.0, true);
}