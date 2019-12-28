#ifndef __UI_H__
#define __UI_H__

#include "p2Defs.h"
#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;

enum class Element
{
	button,
	slider,
	textblock,
	none
};

class UIElement
{
public:

	UIElement(){}

	virtual void Start() = 0;
	virtual void Update(float dt) = 0;

	iPoint position;
	iPoint size;
	bool exists;
	int index;
	Element type;
	SDL_Texture* text;
};

class Button : public UIElement
{
public:

	Button();

	void Start();
	void Update(float dt);

private:

	Animation idle;
	Animation hover;
	Animation pressed;
};

#endif //__UI_H__
