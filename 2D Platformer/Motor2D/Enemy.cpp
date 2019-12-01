#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Enemy.h"
#include <math.h>
#include <cstring>

#include "j1Input.h"
#include "j1Render.h"
#include "ModuleCollision.h"

#define THIS App->entities->Entities[index]

Enemy::Enemy() 
{
	type = enemy;
	enemy_sprites = App->tex->Load("textures/Enemy.png");
	Animate(run, 0, 0, 8, 0.1f);
	initialposx = -69420.0f;
	velocity = 0.5f;
	fliped = true;
	col = App->collision->AddCollider({ 0, 0, 20, 27}, COLLIDER_ENEMY_SHOT, App->entities);
}

void Enemy::Animate(Animation& anim, const int first_coll, const int first_row, const int n, float speed, bool loop)
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

bool Enemy::Update(float dt)
{ 
	current_animation = &run;
	if (initialposx == -69420.0f)
		initialposx = THIS->position.x;
	
	if (THIS->position.x < initialposx - 50.0f && velocity < 0.0f)
	{
		velocity *= -1;
		fliped = true;
	}
	if (THIS->position.x > initialposx + 50.0f && velocity > 0.0f)
	{
		velocity *= -1;
		fliped = false;
	}

	THIS->position.x += App->SyncVelocity(velocity);


	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	col->SetPos(position.x + 21, position.y + 17);

	if (fliped)
		App->render->Blit(enemy_sprites, THIS->position.x, THIS->position.y, &current_frame, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(enemy_sprites, THIS->position.x, THIS->position.y, &current_frame);


	return true; 
}

bool Enemy::CleanUp()
{ 
	return true;
}