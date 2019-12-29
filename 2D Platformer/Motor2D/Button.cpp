#include "p2Defs.h"
#include "p2Log.h"
#include <math.h>
#include <cstring>

#include "UIElements.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"

Button::Button()
{
	lock_press = true;
}

void Button::Update(float dt)
{
	iPoint mpos = App->input->GetMousePosition();
	int scale = App->win->GetScale();
	mpos.x -= App->render->camera.x / scale;
	mpos.y -= App->render->camera.y / scale;
	if (mpos.x > position.x && mpos.x < (position.x + size.x) && mpos.y > position.y && mpos.y < (position.y + size.y))
	{
		is_hovering = true;
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			if (!lock_press)
				is_pressed = true;
			else
				is_pressed = !is_pressed;
		else
			if (!lock_press)
				is_pressed = false;
	}
	else
	{
		is_hovering = false;
		if (!lock_press)
			is_pressed = false;
	}
	
	//LOG("mouseX: %d | mouseY: %d", mpos.x, mpos.y);
	//LOG("cameraX: %d | cameraY: %d", App->render->camera.x, App->render->camera.y);
	LOG("mouseworldX: %d | mouseworldY: %d", mpos.x - App->render->camera.x/scale, mpos.y - App->render->camera.y/scale);

	if(!is_hovering && !is_pressed)
		App->render->DrawQuad({ position.x, position.y, size.x, size.y}, 0, 0, 255);
	else if(!is_pressed)
		App->render->DrawQuad({ position.x, position.y, size.x, size.y }, 0, 255, 0);
	else
		App->render->DrawQuad({ position.x, position.y, size.x, size.y }, 255, 0, 0);
	App->render->Blit(text, position.x, position.y);
}