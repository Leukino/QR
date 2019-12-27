#ifndef __UI_H__
#define __UI_H__

#include "p2Defs.h"
#include "p2Point.h"
#include "Animation.h"

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

	UIElement();

	virtual void Start() = 0;
	virtual void Update() = 0;

	fPoint position;
	fPoint size;

private:

	Element element;
};

class Button : public UIElement
{
public:

	Button();

	void Start();
	void Update();

private:

	Animation idle;
	Animation hover;
	Animation pressed;
};

#endif //__UI_H__
