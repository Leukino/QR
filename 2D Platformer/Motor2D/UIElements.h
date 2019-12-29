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
	console,
	none
};

class UIElement
{
public:

	UIElement(){}

	virtual void Update(float dt) = 0;

	iPoint position;
	iPoint initialpos;
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

	void Update(float dt);

	bool lock_press;
	bool is_pressed;
	bool is_hovering;

private:

	Animation idle;
	Animation hover;
	Animation pressed;
};

class Console : public UIElement
{
public:

	Console();

	void Update(float dt);
	void ExecuteCommand(const char* text);
	p2SString GetOutputText();
	bool isOpen;

private:
	p2SString inputText;
	p2SString outputText;

};
#endif //__UI_H__