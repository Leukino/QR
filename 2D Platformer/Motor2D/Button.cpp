#include "p2Defs.h"
#include "p2Log.h"
#include <math.h>
#include <cstring>

#include "UIElements.h"
#include "j1Input.h"
#include "j1Render.h"

Button::Button()
{

}

void Button::Start()
{

}

void Button::Update(float dt)
{
	App->render->DrawQuad({ (int)position.x, (int)position.y, (int)size.x, (int)size.y }, 0, 0, 255);
	App->render->Blit(text, position.x, position.y);
}