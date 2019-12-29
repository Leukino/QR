#include "p2Defs.h"
#include "p2Log.h"
#include <math.h>
#include <cstring>

#include "ModuleUI.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Scene.h"


ModuleUI::ModuleUI()
{
	LOG("UI Module Loaded");
}

ModuleUI::~ModuleUI()
{
	CleanUp();
}

bool ModuleUI::Awake(pugi::xml_node& entities_data)
{
	element_id = 0;
	return true;
}

bool ModuleUI::Start()
{
	return true;
}

Button* ModuleUI::CreateButton(iPoint pos, iPoint size, char* text, bool exists, bool lock)
{
	Button* but = new Button();
	
	but->text = App->fonts->Print(text);
	but->initialpos.x = pos.x;
	but->initialpos.y = pos.y;
	but->size.x = size.x;
	but->size.y = size.y;
	but->index = element_id;
	but->type = Element::button;
	but->exists = exists;
	but->lock_press = lock;

	UIElements.add(but);

	element_id++;

	return but;
}

Console* ModuleUI::CreateConsole(iPoint pos, iPoint size, bool exists)
{
	Console* cons = new Console();

	cons->text = App->fonts->Print(cons->GetOutputText().GetString());
	cons->position.x = pos.x;
	cons->position.y = pos.y;
	cons->size.x = size.x;
	cons->size.y = size.y;
	cons->index = element_id;
	cons->type = Element::console;
	cons->exists = exists;

	UIElements.add(cons);

	element_id++;
	return cons;
}

void ModuleUI::DeleteUIElement(int index)
{
	UIElements.del(UIElements.At(index));
	for (int i = index; i < UIElements.count(); i++)
		UIElements[i]->index--;
}

bool ModuleUI::Update(float dt)
{
	for (int i = 0; i < UIElements.count(); i++)
		if (UIElements[i]->exists)
		{
			UIElements[i]->Update(dt);
		}
	return true;
}

bool ModuleUI::CleanUp()
{
	UIElements.clear();
	element_id = 0;

	return true;
}