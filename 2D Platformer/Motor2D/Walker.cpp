#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

#include "ModuleCollision.h"
#include "j1Render.h"
#include "EntityManager.h"

Walker::Walker()
{
	LOG("it's a Walker");
	type = walker;
	sprites = App->tex->Load("textures/Enemy.png");
	Animate(walk, 0, 0, 8, 0.1f);
	initialpos.x = -69420.0f;
	
	velocity = 0.5f;
	fliped = true;
	col = App->collision->AddCollider({ 0, 0, 20, 27 }, COLLIDER_ENEMY_SHOT, this);
}

bool Walker::Update(float dt)
{
	current_animation = &walk;
	if (initialpos.x == -69420.0f)
	{
		initialpos.x = position.x;
		position.y -= 20.0f;
	}

	if (this->position.x < initialpos.x - 50.0f && velocity < 0.0f)
	{
		velocity *= -1;
		fliped = true;
	}
	if (this->position.x > initialpos.x + 50.0f && velocity > 0.0f)
	{
		velocity *= -1;
		fliped = false;
	}

	this->position.x += App->SyncVelocity(velocity);


	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	col->SetPos(position.x + 21, position.y + 17);

	if (fliped)
		App->render->Blit(sprites, this->position.x, this->position.y, &current_frame, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(sprites, this->position.x, this->position.y, &current_frame);


	return true;
}

bool Walker::CleanUp()
{
	return true;
}

void Walker::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_ENEMY_SHOT && c2->type == COLLIDER_PLAYER_ATK)
	{
		col->to_delete = true;
		App->entities->DeleteEntity(index);
	}
}