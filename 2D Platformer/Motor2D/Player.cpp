#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "ModuleCollision.h"
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

void Player::Animate(Animation& anim,const int first_coll,const int first_row,const int n)
{
	int coll = first_coll;
	int row = first_row;
	for (int i = 0; i < n; i++)
	{
		anim.PushBack({ xy_increase * coll, xy_increase * row, sprite_wh, sprite_wh});
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
	//player height=34 widht=15

	right_col = App->collision->AddCollider({ 0, 0, 5, 34 }, COLLIDER_PLAYER_RIGHT, this);
	left_col = App->collision->AddCollider({ 0, 0, 5, 34 }, COLLIDER_PLAYER_LEFT, this);
	feet_col = App->collision->AddCollider({ 0, 0, 5, 10 }, COLLIDER_PLAYER_FOOT, this);

	Animate(idle_right, 0, 0, 2);
	Animate(idle_left, 2, 0, 2);
	Animate(run_right, 4, 0, 6);
	Animate(run_left, 10, 0, 6);

	idle_right.speed = 0.01f;
	idle_left.speed = 0.01f;
	run_right.speed = 0.03f;
	run_left.speed = 0.03f;
	return true;
}

bool Player::CleanUp()
{
	return true;
}

bool Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		facing_right = true;
		running = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		facing_right = false;
		running = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		running = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		running = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP && App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		running_timer = 0;
	}
	
	if (running)
		if (facing_right)
		{
			current_animation = &run_right;
			running_timer++;
			if (running_timer == 3)
			{
				position.x++;
				running_timer = 0;
			}
		}
		else
		{
			current_animation = &run_left;
			running_timer++;
			if (running_timer == 3)
			{
				position.x--;
				running_timer = 0;
			}
		}
	else
		if (facing_right)
			current_animation = &idle_right;
		else
			current_animation = &idle_left;

	right_col->SetPos(24 + position.x + 10, 20 + position.y);
	left_col->SetPos(20 + position.x, 20 + position.y);
	feet_col->SetPos(20 + position.x + 5, 20 + position.y + 24);

	SDL_Rect &current_frame = current_animation->GetCurrentFrame();
	App->render->Blit(player_sprites, position.x, position.y, &current_frame);
	return true;
}