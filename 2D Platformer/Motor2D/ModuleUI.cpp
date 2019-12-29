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

Button* ModuleUI::CreateButton(iPoint pos, iPoint size, char* text, bool exists)
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

	UIElements.add(but);

	element_id++;

	return but;
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
		CreateButton({ (int)App->scene->pl->position.x , (int)App->scene->pl->position.y }, { 100,40 }, "aaaaaaa");
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