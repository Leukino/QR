#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "WindowsStuff.h"
#include <time.h>

#include "ModuleCollision.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Map.h"
#include "ModuleUI.h"
#include "MainMenu.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;
	map = 0;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	//App->map->Load("edgy map.tmx");
	App->map->Load("Map 2 retextured.tmx");

	pl = App->entities->CreateEntity(0.0f, 0.0f, player);
	MenuButton = App->ui->CreateButton({ 0,10 }, { 35,35 }, "Menu", false);
	App->ui->CreateConsole({ 100, 100 }, { 200, 20 });
	
	return true;
	srand(time(NULL));
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

void j1Scene::changeMap()
{
	
	App->map->CleanUp();
	if (map == 0)
	{
		App->map->Load("edgy map retextured.tmx");
		map++;
	}
	else if (map == 1)
	{
		App->map->Load("Map 2 retextured.tmx");
		map++;
	}
	if (map > 1)
		map = 0;
	Reset();
	
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	MenuButton->exists = true;

	//if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//	App->LoadGame();
	//
	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	//	App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;
	//if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	//	App->setFpsCap(App->getFpsCap() + 10);
	//if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	//{
	//	//if (App->getFpsCap() > 10)
	//	App->setFpsCap(App->getFpsCap() - 10);
	//}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		changeMap();
	}
	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN)
	{
		App->consoleEnabled = !App->consoleEnabled;
		LOG(" % s", App->consoleEnabled ? "true" : "false");
	}
	if (MenuButton->is_pressed)
	{
		CleanUp();
		Disable();
		App->collision->Disable();
		App->entities->Disable();
		App->mainmenu->Init();
		MenuButton->is_pressed = false;
	}

	App->render->setCamera();
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

void j1Scene::Reset()
{
	App->entities->CleanUp();
	App->collision->CleanUp();
	App->map->DrawMapEntities();
	App->map->DrawMapColliders();
	pl = App->entities->CreateEntity(200.0f, 300.0f, player);
}

bool j1Scene::CleanUp()
{
	MenuButton->exists = false;
	App->entities->CleanUp();
	App->collision->CleanUp();
	App->map->CleanUp();
	return true;
}
