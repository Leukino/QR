#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Enemy.h"
#include <math.h>
#include <cstring>

#include "j1Input.h"
#include "j1Render.h"

#define THIS App->entities->Entities[index]

Enemy::Enemy() 
{
	type = enemy;
	enemy_sprites = App->tex->Load("textures/Knight.png");

	idle.PushBack({ 0, 0, 200, 200 });
}

bool Enemy::Update(float dt)
{ 
	current_animation = &idle;
	
	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	
	App->render->Blit(enemy_sprites, THIS->position.x, THIS->position.y, &current_frame);

	return true; 
}

bool Enemy::CleanUp()
{ 
	return true;
}