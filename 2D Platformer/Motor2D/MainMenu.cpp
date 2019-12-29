#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "MainMenu.h"
#include <time.h>

#include "j1Input.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "ModuleCollision.h"
#include "EntityManager.h"
#include "j1Map.h"



MainMenu::MainMenu()
{

}

// Destructor
MainMenu::~MainMenu()
{}

// Called before render is available
bool MainMenu::Awake()
{
	LOG("Loading Scene");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool MainMenu::Start()
{
	PlayButton = App->ui->CreateButton({ 0, 0 }, { 100,30 }, "Play");
	ExitButton = App->ui->CreateButton({ 0, 30 }, { 100,30 }, "Exit");
	return true;
}

// Called each loop iteration
bool MainMenu::Update(float dt)
{
	PlayButton->exists = true;
	ExitButton->exists = true;
	

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
		App->setFpsCap(App->getFpsCap() + 10);
	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		App->setFpsCap(App->getFpsCap() - 10);
	}

	if(PlayButton->is_pressed)
	{ 
		CleanUp();
		App->scene->Init();
		App->collision->Init();
		App->entities->Init();
		PlayButton->is_pressed = false;
		App->map->Load("Map 2 retextured.tmx");
		App->scene->Reset();
		Disable();
	}

	if (ExitButton->is_pressed)
		return false;
	

	return true;
}

// Called each loop iteration
bool MainMenu::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	PlayButton->exists = false;
	ExitButton->exists = false;
	return true;
}
