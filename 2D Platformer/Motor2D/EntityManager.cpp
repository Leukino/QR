#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "EntityManager.h"
#include "Enemy.h"
#include <math.h>
#include <cstring>

#include "j1Input.h"
#include "Player.h"
#include "j1Window.h"
#include "j1Render.h"
#include "ModuleCollision.h"

Entity::Entity() {}

entity_type Entity::GetType()
{
	return this->type;
}

EntityManager::EntityManager() 
{
	LOG("Entity Manager Loaded");
}

EntityManager::~EntityManager() {}

bool EntityManager::Awake(pugi::xml_node& entities_data)
{
	entity_id = 0;
	return true;
}

bool EntityManager::Start()
{
	return true;
}

void EntityManager::CreateEntity(float x, float y, entity_type type, bool exists)
{
	switch (type)
	{
	case player: this->entity = new Entity(); break;
	case enemy: this->entity = new Enemy(); break;
	}
	this->entity->position.x = x;
	this->entity->position.y = y;
	this->entity->index = entity_id;
	this->entity->exists = exists;

	Entities.add(entity);

	entity_id++;
}

bool EntityManager::Update(float dt)
{
	for (int i = 0; i < Entities.count(); i++)
		if (Entities[i]->exists)
		{
			if (Entities[i]->position.x > App->player->position.x - 1000.0f && Entities[i]->position.x < App->player->position.x + 1000.0f && Entities[i]->position.y > App->player->position.y - 1000.0f && Entities[i]->position.y < App->player->position.y + 1000.0f)
				Entities[i]->Update(dt);
		}
		else
		{
			Entities[i]->col->to_delete = true;
		}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		CreateEntity(App->player->position.x, App->player->position.y - 20, enemy);

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		for (int i = 0; i < Entities.count(); i++)
			LOG("Entity %d - x: %f y: %f Size: %d", Entities[i]->index, Entities[i]->position.x, Entities[i]->position.y, Entities.count());
		
	return true;
}

bool EntityManager::CleanUp()
{
	Entities.clear();
	entity_id = 0;

	return true;
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	//if ((c2->type == COLLIDER_ENEMY_SHOT && c1->type == COLLIDER_PLAYER_ATK))
	//{
	//	for (int i = 0; i < Entities.count(); i++)
	//		if (Entities[i]->col == c2)
	//			Entities[i]->exists = false;
	//}
}
