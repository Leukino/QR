#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

#include "ModuleCollision.h"
#include "j1Render.h"
#include "j1Scene.h"

Bat::Bat()
{
	LOG("it's a Bat");
	type = enemy;
	sprites = App->tex->Load("textures/Bat.png");
	Animate(rest, 0, 0, 1);
	Animate(fly, 1, 0, 2, 0.2f);
	initialpos = { -69420.0f, 0.0f };
	velocity = 1.0f;
	fliped = false;
	col = App->collision->AddCollider({ 0, 0, 20, 27 }, COLLIDER_ENEMY_SHOT, this);
}

bool Bat::Update(float dt)
{
	if (initialpos.x == -69420.0f)
		initialpos = { this->position.x - 16, this->position.y - 16};
	if (this->position.x < App->scene->pl->position.x - 300.f || this->position.x > App->scene->pl->position.x + 300.f)
	{
		current_animation = &rest;
		this->position = initialpos;
	}
	else
	{
		current_animation = &fly;
		if (this->position.x < App->scene->pl->position.x)
		{
			this->position.x += App->SyncVelocity(velocity);
			fliped = true;
		}
		else
		{
			this->position.x -= App->SyncVelocity(velocity);
			fliped = false;
		}
	
		if (this->position.y < App->scene->pl->position.y)
			this->position.y += App->SyncVelocity(velocity);
		else
			this->position.y -= App->SyncVelocity(velocity);
	}

	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	col->SetPos(position.x + 21, position.y + 17);

	if (fliped)
		App->render->Blit(sprites, this->position.x, this->position.y, &current_frame, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(sprites, this->position.x, this->position.y, &current_frame);

	return true;
}

bool Bat::CleanUp()
{
	return true;
}

void Bat::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_ENEMY_SHOT && c2->type == COLLIDER_PLAYER_ATK)
	{
		col->to_delete = true;
		App->entities->DeleteEntity(index);
	}
}