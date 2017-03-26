#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "UIComponents.h"

struct _TTF_Font;

class UILabel : public UIComponents
{
public:
	std::string text;
	SDL_Texture* text_img = nullptr;
	_TTF_Font*  font = nullptr;

public:
	UILabel(UICOMPONENT_TYPE type);

	~UILabel();

	void Set(int pos_x, int pos_y, const char* text, _TTF_Font*  font = nullptr);

	void Draw();

	bool Update();

	void ChangeText(const char* text);
};

#endif // __UILABEL_H__