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
	position.x = 100.0f;
	position.y = 100.0f;
	//player height=34 widht=15

	right_col = App->collision->AddCollider({ 0, 0, 5, 34 }, COLLIDER_PLAYER_RIGHT, this);
	left_col = App->collision->AddCollider({ 0, 0, 5, 34 }, COLLIDER_PLAYER_LEFT, this);
	feet_col = App->collision->AddCollider({ 0, 0, 5, 10 }, COLLIDER_PLAYER_FOOT, this);
	floor_col = App->collision->AddCollider({ 0, 200, 3000, 100 }, COLLIDER_GROUND, this);

	slide_vel = exp_vel;

	Animate(idle_right, 0, 0, 2);
	Animate(idle_left, 2, 0, 2);
	Animate(run_right, 4, 0, 6);
	Animate(run_left, 10, 0, 6);
	Animate(jump_up_right, 16, 0, 1);
	Animate(jump_down_right, 1, 1, 1);
	Animate(jump_up_left, 3, 1, 1);
	Animate(jump_down_left, 5, 1, 1);
	Animate(slide_right, 2, 1, 1);
	Animate(slide_left, 6, 1, 1);

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
	if (collissioncounter == 0)
		grounded = false;
	collissioncounter = 0;

	if (!EXPUROSHON)
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
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		running = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
	{
		running = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !jumping)
	{
		jumping = true;
		grounded = false;
		velocityY = jump_vel;
		vo = velocityY;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumping && !EXPUROSHON)
	{
		EXPUROSHON = true;
		velocityY = jump_vel;
		vo = velocityY;
		timer = 0;

	}

	if (running)
		if (facing_right)
		{
			if (!jumping)
				current_animation = &run_right;
			position.x += run_vel;
		}
		else
		{
			if (!jumping)
				current_animation = &run_left;
			position.x -= run_vel;
		}
	else
		if (facing_right && !jumping)
			current_animation = &idle_right;
		else if (!jumping)
			current_animation = &idle_left;
	if (jumping)
			if (facing_right)
				current_animation = &jump_up_right;
			else
				current_animation = &jump_up_left;

	if (EXPUROSHON && !sliding)
		if (facing_right)
		{
			current_animation = &jump_down_right;
			position.x += exp_vel;
		}
		else
		{ 
			current_animation = &jump_down_left;
			position.x -= exp_vel;
		}	

	if (!grounded)
	{
		timer++;
		velocityY = vo + a*timer;
	}
	else
	{
		velocityY = 0.0f;
		if (!sliding)
		{
			timer = 0;
		}
	}
	position.y += velocityY;

	if (sliding)
	{
		timer++;
		if (timer < 90)
		{
			if (facing_right)
			{
				current_animation = &slide_right;
				position.x += slide_vel;
			}
			else
			{
				current_animation = &slide_left;
				position.x -= slide_vel;
			}
			slide_vel -= 0.005f;
			if (slide_vel < 0.1f)
				slide_vel = 0.1;
		}
		else
		{
			sliding = false;
			EXPUROSHON = false;
			slide_vel = exp_vel;
			timer = 0;
		}
	}

	right_col->SetPos(24 + position.x + 10, 20 + position.y);
	left_col->SetPos(20 + position.x, 20 + position.y);
	feet_col->SetPos(20 + position.x + 5, 20 + position.y + 24);

	SDL_Rect &current_frame = current_animation->GetCurrentFrame();
	App->render->Blit(player_sprites, position.x, position.y, &current_frame);
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER_FOOT && c2->type == COLLIDER_GROUND)
	{
		if (jumping)
		{
			run_right.SetFrame(2);
			run_left.SetFrame(5);
			jumping = false;
		}
		if (EXPUROSHON && !sliding)
		{
			sliding = true;
			timer = 0;
		}
		grounded = true;
		vo = 0.0f;
		position.y = c2->rect.y - 53;
		collissioncounter++;
	}
}