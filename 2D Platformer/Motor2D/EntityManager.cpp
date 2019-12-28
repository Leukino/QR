#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "EntityManager.h"
#include <math.h>
#include <cstring>
#include "j1Input.h"
#include "j1Window.h"
#include "j1Render.h"
#include "ModuleCollision.h"

EntityManager::EntityManager()
{
	LOG("Entity Manager Loaded");
}

EntityManager::~EntityManager() 
{
	CleanUp();
}

bool EntityManager::Awake(pugi::xml_node& entities_data)
{
	entity_id = 0;
	return true;
}

bool EntityManager::Start()
{
	return true;
}

Entity* EntityManager::CreateEntity(float x, float y, entity_type type, bool exists)
{
	switch (type)
	{
	case player: this->entity = new Player(); break;
	case walker: this->entity = new Walker(); break;
	case bat: this->entity = new Bat(); break;
	case none: this->entity = new Entity(); break;
	}
	entity->position.x = x;
	entity->position.y = y;
	entity->index = entity_id;
	entity->type = type;
	entity->exists = exists;

	Entities.add(entity);

	entity_id++;

	return entity;
}

void EntityManager::DeleteEntity(int index)
{
	Entities.del(Entities.At(index));
	for (int i = index; i < Entities.count(); i++)
		Entities[i]->index--;
}

bool EntityManager::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		CreateEntity(0.0f, 0.0f, player);
		CreateEntity(0.0f, 0.0f, walker);
		CreateEntity(0.0f, 0.0f, bat);
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		DeleteEntity(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		for (int i = 0; i < Entities.count(); i++)
			LOG("Entity %d - x: %f y: %f Size: %d, type: %d", Entities[i]->index, Entities[i]->position.x, Entities[i]->position.y, Entities.count(), Entities[i]->type);
	for (int i = 0; i < Entities.count(); i++)
		if (Entities[i]->exists)
		{
			//if (Entities[i]->position.x > App->player->position.x - 1000.0f && Entities[i]->position.x < App->player->position.x + 1000.0f && Entities[i]->position.y > App->player->position.y - 1000.0f && Entities[i]->position.y < App->player->position.y + 1000.0f)
			Entities[i]->Update(dt);
		}

	//
	//	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//		CreateEntity(App->player->position.x, App->player->position.y - 20, enemy);
	//
	//	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	//		for (int i = 0; i < Entities.count(); i++)
	//			LOG("Entity %d - x: %f y: %f Size: %d", Entities[i]->index, Entities[i]->position.x, Entities[i]->position.y, Entities.count());
	//		
	return true;
}

bool EntityManager::CleanUp()
{
	//for (int i = 0; i < Entities.count(); i++)
	//	Entities[i]->col->to_delete= true;
	Entities.clear();
	entity_id = 0;

	return true;
}
