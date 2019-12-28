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

UIElement* ModuleUI::CreateUIElement(iPoint pos, iPoint size, Element type, char* text, bool exists)
{
	switch (type)
	{
	case Element::button: elem = new Button(); break;
	}
	elem->text = App->fonts->Print(text);
	elem->position.x = pos.x;
	elem->position.y = pos.y;
	elem->size.x = size.x;
	elem->size.y = size.y;
	elem->index = element_id;
	elem->type = type;
	elem->exists = exists;

	UIElements.add(elem);

	element_id++;

	return elem;
}

void ModuleUI::DeleteUIElement(int index)
{
	UIElements.del(UIElements.At(index));
	for (int i = index; i < UIElements.count(); i++)
		UIElements[i]->index--;
}

bool ModuleUI::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		for (int i = 0; i < UIElements.count(); i++)
			LOG("Entity %d - x: %f y: %f Size: %d, type: %d", UIElements[i]->index, UIElements[i]->position.x, UIElements[i]->position.y, UIElements.count(), UIElements[i]->type);

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		CreateUIElement({ (int)App->scene->pl->position.x , (int)App->scene->pl->position.y }, { 100,40 }, Element::button, "aaaaaaa");
	}

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