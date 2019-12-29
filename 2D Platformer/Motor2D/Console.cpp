#include "p2Defs.h"
#include "p2Log.h"
#include <math.h>
#include <cstring>

#include "UIElements.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "Entities.h"
#include "ModuleCollision.h"
Console::Console()
{
	inputText = "";
	outputText = "Type a command...";
	LOG("Created Console Pog");
}

void Console::Update(float dt)
{
	if (App->consoleEnabled == false)
	{
		inputText.Clear();
		return;
	}
	//if (App->consoleEnabled)
	//	SDL_StartTextInput();
	//
	//SDL_Event event;
	//while (SDL_PollEvent(&event) != 0)
	//{
	//	if (event.type == SDL_TEXTINPUT)
	//	{
	//		inputText += event.text.text;
	//		LOG("Typed %s", inputText.GetString());
	//	}
	//
	//}
	//
	//SDL_StopTextInput();
	
	inputText = App->input->GetText();
	
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		inputText.Clear();
		App->input->ResetText();
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		LOG("ENTERED COMMAND %s", inputText.GetString());
		ExecuteCommand(inputText.GetString());
		inputText.Clear();
		App->input->ResetText();
		App->consoleEnabled = false;
		
	}
	if (strcmp(inputText.GetString(), "") == 0)
		outputText = "Type a command...";
	else
		outputText = inputText;

	text = App->fonts->Print(outputText.GetString());
	App->render->Blit(text, App->scene->pl->position.x - position.x, App->scene->pl->position.y - position.y);
}

void Console::ExecuteCommand(const char* text)
{
	//if (strcmp(text, "fpscap") == 0)
	//	if (App->input->GetKey(SDL_SCANCODE_F11) == j1KeyState::KEY_DOWN) {
	//		App->fpscap = !App->fpscap;
	//		App->frame_count = 0;
	//		if (App->fpscap)
	//			App->setFpsCap(30);
	//		else
	//			App->setFpsCap(120);
	//	}
	if (strcmp(text, "set30fps") == 0)
		App->setFpsCap(30);
	if (strcmp(text, "set60fps") == 0)
		App->setFpsCap(60);
	if (strcmp(text, "set120fps") == 0)
		App->setFpsCap(120);
	if (strcmp(text, "quit") == 0)
		App->wannaquit = true;
	if (strcmp(text, "godmode") == 0)
		App->changeGodMode = true;
	if (strcmp(text, "showcolliders") == 0)
		App->collision->changeDebug();
	if (strcmp(text, "changemap") == 0)
		App->scene->changeMap();
}

p2SString Console::GetOutputText()
{
	return outputText;
}