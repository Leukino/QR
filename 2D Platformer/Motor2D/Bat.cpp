#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Bat.h"
#include <math.h>
#include <cstring>

#include "j1Input.h"
#include "j1Render.h"
#include "ModuleCollision.h"

#define THIS App->entities->Entities[index]

Bat::Bat()
{
	type = enemy;
	enemy_sprites = App->tex->Load("textures/Bat.png");
	Animate(idle, 0, 0, 1);
	Animate(run, 1, 0, 2, 0.2f);
	initialpos = { -69420.0f, 0.0f };
	velocity = 1.0f;
	fliped = false;
	col = App->collision->AddCollider({ 0, 0, 20, 27 }, COLLIDER_ENEMY_SHOT, App->entities);
}

void Bat::Animate(Animation& anim, const int first_coll, const int first_row, const int n, float speed, bool loop)
{
	int coll = first_coll;
	int row = first_row;
	for (int i = 0; i < n; i++)
	{
		anim.PushBack({ 61 * coll, 61 * row, 60, 60 });
		coll++;
		if (coll == 17)
		{
			coll = 0;
			row++;
		}
	}
	anim.speed = speed;
	anim.loop = loop;
}

bool Bat::Update(float dt)
{
	
	if (initialpos.x == -69420.0f)
		initialpos = { THIS->position.x, THIS->position.y };
	if (THIS->position.x < App->player->position.x - 300.f || THIS->position.x > App->player->position.x + 300.f)
	{
		current_animation = &idle;
		THIS->position = initialpos;
	}
	else
	{
		current_animation = &run;
		if (THIS->position.x < App->player->position.x)
		{
			THIS->position.x += App->SyncVelocity(velocity);
			fliped = true;
		}
		else
		{
			THIS->position.x -= App->SyncVelocity(velocity);
			fliped = false;
		}

		if (THIS->position.y < App->player->position.y)
			THIS->position.y += App->SyncVelocity(velocity);
		else
			THIS->position.y -= App->SyncVelocity(velocity);
	}

	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	col->SetPos(position.x + 21, position.y + 17);

	if (fliped)
		App->render->Blit(enemy_sprites, THIS->position.x, THIS->position.y, &current_frame, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(enemy_sprites, THIS->position.x, THIS->position.y, &current_frame);

	return true;
}

bool Bat::CleanUp()
{
	return true;
}