#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Player.h"
#include <math.h>
#include <cstring>

Player::Player()
{
}

Player::~Player()
{}

bool Player::Start()
{
	player_sprites = App->tex->Load("textures/Knight.png");
	current_animation = &idle_right;

	return true;
}

void Player::Animate(Animation anim,const int first_coll,const int first_row, int n)
{
	int coll = first_coll;
	int row = first_row;
	for (int i = 0; i < n; i++)
	{
		anim.PushBack({ xy_increase * coll, xy_increase * row, 60,60 });
		coll++;
		if (coll == n_coll)
		{
			coll = 0;
			row++;
		}
	}
}

bool Player::Awake(pugi::xml_node& conf)
{
	position.x = 100;
	position.y = 100;

	idle_right.speed = 0.01f;
	idle_left.speed = 0.01f;
	run_right.speed = 0.01f;
	run_left.speed = 0.01f;
	return true;
}

bool Player::CleanUp()
{
	return true;
}

bool Player::Update(float dt)
{

	SDL_Rect &current_frame = current_animation->GetCurrentFrame();
	App->render->Blit(player_sprites, position.x, position.y, &current_frame);
	LOG("Reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
	return true;
}