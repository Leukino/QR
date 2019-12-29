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

Console::Console()
{
	inputText = "";
	outputText = "Type a command...";
	LOG("Created Console Pog");
}

void Console::Update(float dt)
{
	if (App->consoleEnabled)
		SDL_StartTextInput();

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_TEXTINPUT)
			inputText += event.text.text;
	}

	SDL_StopTextInput();
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
	{
		inputText.Clear();
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		inputText.Clear();
		LOG("ENTERED COMMAND %s", inputText.GetString());
		ExecuteCommand(inputText.GetString());
	}
	if (strcmp(inputText.GetString(), "") == 0)
		outputText = "Type a command...";
	else
		outputText = inputText;

	text = App->fonts->Print(outputText.GetString());
}

void Console::ExecuteCommand(const char* text)
{
	//if (strcmp(text, "quit") == 0)
		
}

p2SString Console::GetOutputText()
{
	return outputText;
}