#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

#include "ModuleCollision.h"
#include "EntityManager.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Scene.h"

Player::Player()
{
	LOG("it's a Player");
	sprites = App->tex->Load("textures/Knight.png");
	initialpos.x = 200.0f;
	initialpos.y = 300.0f;
	position = initialpos;
	Animate(idle_right, 0, 0, 2, 0.01f);
	Animate(idle_left, 2, 0, 2, 0.01f);
	Animate(run_right, 4, 0, 6, 0.2f);
	Animate(run_left, 10, 0, 6, 0.2f);
	Animate(jump_up_right, 16, 0, 1);
	Animate(jump_down_right, 1, 1, 1);
	Animate(jump_up_left, 3, 1, 1);
	Animate(jump_down_left, 5, 1, 1);
	Animate(slide_right, 2, 1, 1);
	Animate(slide_left, 6, 1, 1);
	Animate(attack_right, 7, 1, 5, 0.15f, false);
	Animate(attack_left, 12, 1, 5, 0.15f, false);
	Animate(air_atk1_right, 0, 2, 5, 0.5f, false);
	Animate(air_atk2_right, 5, 2, 6, 0.5f, false);
	Animate(air_atk1_left, 11, 2, 5, 0.5f, false);
	Animate(air_atk2_left, 0, 3, 5, 0.5f, false);
	godmode = false;
	facing_right = true;
	running = false;
	grounded = false;
	jumping = false;
	attacking_idle = false;
	air_atk = false;
	air_atking = false;
	sliding = false;
	headcollided = false;
	timer = -1.0f;
	collissioncounter = 0;
	wallcolcounter = 0;
	air_atk_counter = 0;
	velocity = 2.0f;
	exp_vel = 4.0f;
	jump_vel = -10.0f;
	slide_vel = exp_vel;
	ground_friction = 0.15f;
	a = 0.5f;
	right_col = App->collision->AddCollider({ 0, 0, 5, 18 }, COLLIDER_PLAYER_RIGHT, this);
	left_col = App->collision->AddCollider({ 0, 0, 5, 18 }, COLLIDER_PLAYER_LEFT, this);
	feet_col = App->collision->AddCollider({ 0, 0, 13, 10 }, COLLIDER_PLAYER_FOOT, this);
	head_col = App->collision->AddCollider({ 0, 0, 10, 10 }, COLLIDER_PLAYER_HEAD, this);
	player_atk = App->collision->AddCollider({ 0, 0, 20, 30 }, COLLIDER_PLAYER_ATK, this);
	rightcol_offset.x = 34;
	rightcol_offset.y = 28;
	leftcol_offset.x = 20;
	leftcol_offset.y = 28;
	footcol_offset.x = 23;
	footcol_offset.y = 44;
	headcol_offset.x = 24;
	headcol_offset.y = 20;
}

bool Player::Update(float dt)
{
	if (App->changeGodMode) 
	{
		SetGodMode();
		App->changeGodMode = false;
	}
	//LOG("playerX: %f | playerY: %f", position.x, position.y);
	ResetColChecks();

	Inputs();

	if (!godmode)
		Gravity(dt);

	if (!attacking_idle)
	{
		if (running)
		{
			Move();
		}
		else
			Idle();
		if (jumping || (!grounded && !jumping))
			Jump();
	}
	else
	{
		Attack();
	}
	if (air_atking)
	{
		AirMove();

		if (air_atk)
		{
			AirAttack();
		}
		else
		{
			player_atk->set({ -10, -10, 20, 30 }, COLLIDER_PLAYER_ATK, this);
		}
	}

	if (sliding)
	{
		Slide();
	}

	right_col->SetPos(position.x + rightcol_offset.x, position.y + rightcol_offset.y);
	left_col->SetPos(position.x + leftcol_offset.x, position.y + leftcol_offset.y);
	feet_col->SetPos(position.x + footcol_offset.x, position.y + footcol_offset.y);
	head_col->SetPos(position.x + headcol_offset.x, position.y + headcol_offset.y);

	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
	App->render->Blit(sprites, position.x, position.y, &current_frame, SDL_FLIP_NONE);

	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (!godmode)
	{
		if (c1->type == COLLIDER_PLAYER_FOOT && c2->type == COLLIDER_GROUND)
		{
			if (jumping)
			{
				run_right.SetFrame(2);
				run_left.SetFrame(5);
				jumping = false;
			}

			if (air_atking)
				sliding = true;
			air_atking = false;
			air_atk = false;
			air_atk_counter = 2;

			if (!attacking_idle)
				player_atk->setRect({ -10, -10, 20, 30 });

			grounded = true;
			vo = 0.0f;
			if (c2->rect.y != 0)
				position.y = c2->rect.y - 53;
			collissioncounter++;
		}

		if (c1->type == COLLIDER_PLAYER_HEAD && c2->type == COLLIDER_GROUND)
			headcollided = true;

		if (c1->type == COLLIDER_PLAYER_RIGHT && c2->type == COLLIDER_WALL)
		{
			wallhitR = true;
			wallcolcounter++;
			position.x = c2->rect.x - 38;
		}

		if (c1->type == COLLIDER_PLAYER_LEFT && c2->type == COLLIDER_WALL)
		{
			wallhitL = true;
			wallcolcounter++;
			position.x = (c2->rect.x + c2->rect.w) - 21;
		}

		if ((c1->type == COLLIDER_PLAYER_ATK && c2->type == COLLIDER_ENEMY_SHOT))
		{
			if (air_atk_counter < 1)
				air_atk_counter++;
		}
		else if ((c1->type == COLLIDER_PLAYER_FOOT || c1->type == COLLIDER_PLAYER_HEAD) && c2->type == COLLIDER_ENEMY_SHOT)
			Dead();
	}
}

bool Player::CleanUp()
{
	right_col->to_delete = true;
	left_col->to_delete = true;
	feet_col->to_delete = true;
	head_col->to_delete = true;
	player_atk->to_delete = true;
	return true;
}

void Player::ResetColChecks()
{
	if (collissioncounter == 0)
		grounded = false;
	collissioncounter = 0;
	if (wallcolcounter == 0)
	{
		wallhitR = false;
		wallhitL = false;
	}
	wallcolcounter = 0;
}

void Player::SetGodMode()
{
	godmode = !godmode;
	vo = 0.0f;
	timer = 0;
	facing_right = true;
	running = false;
	running = false;
	grounded = false;
	jumping = false;
	air_atking = false;
	air_atk = false;
	headcollided = false;
	sliding = false;
}

void Player::Inputs()
{
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		SetGodMode();
	}
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		Dead();
	if (!godmode)
	{
		if (!air_atking && !sliding)
		{
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !wallhitR)
			{
				facing_right = true;
				running = true;
			}

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !wallhitL)
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

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
			run_right.Reset();

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
			run_left.Reset();

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && !jumping && !sliding && !attacking_idle)
		{
			jumping = true;
			grounded = false;
			vY = jump_vel;
			vo = vY;
		}
	}
	else
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x += App->SyncVelocity(velocity * 4);

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x -= App->SyncVelocity(velocity * 4);

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			position.y -= App->SyncVelocity(velocity * 4);
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			position.y += App->SyncVelocity(velocity * 4);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		air_atk_counter++;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (grounded && !jumping && !air_atking && !sliding & !attacking_idle)
		{
			attacking_idle = true;
		}
		if (!grounded && !wallhitL && !wallhitR && !air_atk)
		{
			if (!air_atking)
			{
				running = false;
				air_atking = true;
				air_atk = true;
			}
			else
				air_atk = true;
			air_atk_counter--;
			air_atk1_left.Reset();
			air_atk1_right.Reset();
			air_atk2_left.Reset();
			air_atk2_right.Reset();
		}
	}
}

void Player::Gravity(float dt)
{
	if (!grounded)
	{
		timer++;
		vY = vo + a * timer * dt / 0.016f;
		if (vY > 10.0f)
			vY = 10.0f;
		if (sliding)
			sliding = false;
	}
	else
	{
		headcollided = false;
		vY = 0.0f;
		if (!sliding)
		{
			timer = 0;
		}
	}

	if (headcollided && vY < 0)
		timer += 2;
	else
		position.y += App->SyncVelocity(vY);
}

void Player::Move()
{
	if (facing_right && !wallhitR)
	{
		if (!jumping)
			current_animation = &run_right;
		position.x += App->SyncVelocity(velocity);
	}
	if (!facing_right && !wallhitL)
	{
		if (!jumping)
			current_animation = &run_left;
		position.x -= App->SyncVelocity(velocity);
	}
}

void Player::Idle()
{
	if (facing_right && !jumping)
		current_animation = &idle_right;
	else if (!jumping)
		current_animation = &idle_left;
}

void Player::Jump()
{
	if (facing_right)
		current_animation = &jump_up_right;
	else
		current_animation = &jump_up_left;
}

void Player::Attack()
{
	if (attack_right.GetFrameNum() != attack_left.GetFrameNum())
		if (facing_right)
			attack_left.SetFrame(attack_right.GetFrameNum());
		else
			attack_right.SetFrame(attack_left.GetFrameNum());

	if (attack_right.GetFrameNum() == 4 || attack_left.GetFrameNum() == 4)
	{
		attacking_idle = false;
		attack_right.Reset();
		attack_left.Reset();
	}
	else
		if (facing_right)
			current_animation = &attack_right;
		else
			current_animation = &attack_left;

	if (attack_right.GetFrameNum() == 2 || attack_left.GetFrameNum() == 2)
		if (facing_right)
			player_atk->setRect({ (int)position.x + 39, (int)position.y + 20, 20, 30 });
		else
			player_atk->setRect({ (int)position.x, (int)position.y + 20, 20, 30 });

	if (attack_right.GetFrameNum() == 3 || attack_left.GetFrameNum() == 3)
		player_atk->setRect({ -10, -10, 20, 30 });
}

void Player::AirAttack()
{
	if (facing_right)
	{
		current_animation = &air_atk1_right;
		if (air_atk1_right.GetFrameNum() == 4 && air_atk_counter != 0)
		{
			if (air_atk2_right.GetFrameNum() == 0)
			{
				vY = jump_vel;
				vo = vY;
				timer = 0;
			}
			current_animation = &air_atk2_right;
		}
		if (air_atk2_right.GetFrameNum() == 5)
			air_atk = false;
		if (current_animation == &air_atk1_right)
		{
			if (air_atk1_right.GetFrameNum() == 0)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 10, 20, 10 });
			if (air_atk1_right.GetFrameNum() == 4)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 54, 20, 20 });
			if (air_atk1_right.GetFrameNum() >= 1 && air_atk1_right.GetFrameNum() <= 3)
				player_atk->setRect({ (int)position.x + 39, (int)position.y + 20, 20, 30 });
		}
		else if (current_animation == &air_atk2_right)
		{
			if (air_atk2_right.GetFrameNum() == 5)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 10, 20, 10 });
			if (air_atk2_right.GetFrameNum() == 0 || air_atk2_right.GetFrameNum() == 1)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 54, 20, 20 });
			if (air_atk2_right.GetFrameNum() >= 2 && air_atk2_right.GetFrameNum() <= 4)
				player_atk->setRect({ (int)position.x, (int)position.y + 20, 20, 30 });
		}
	}
	else
	{
		current_animation = &air_atk1_left;
		if (air_atk1_left.GetFrameNum() == 4 && air_atk_counter != 0)
		{
			if (air_atk2_left.GetFrameNum() == 0)
			{
				vY = jump_vel;
				vo = vY;
				timer = 0;
			}
			current_animation = &air_atk2_left;
		}
		if (air_atk2_left.GetFrameNum() == 4)
			air_atk = false;
		if (current_animation == &air_atk1_left)
		{
			if (air_atk1_left.GetFrameNum() == 0)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 10, 20, 10 });
			else if (air_atk1_left.GetFrameNum() == 4)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 54, 20, 20 });
			else if (air_atk1_left.GetFrameNum() >= 1 && air_atk1_left.GetFrameNum() <= 3)
				player_atk->setRect({ (int)position.x, (int)position.y + 20, 20, 30 });
		}
		else if (current_animation == &air_atk2_left)
		{
			if (air_atk2_left.GetFrameNum() == 4)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 10, 20, 10 });
			else if (air_atk2_left.GetFrameNum() == 0)
				player_atk->setRect({ (int)position.x + 18, (int)position.y + 54, 20, 20 });
			else if (air_atk2_left.GetFrameNum() >= 1 && air_atk2_left.GetFrameNum() <= 3)
				player_atk->setRect({ (int)position.x + 39, (int)position.y + 20, 20, 30 });
		}
	}
}

void Player::AirMove()
{
	if (facing_right)
		if (!wallhitR)
		{
			position.x += App->SyncVelocity(exp_vel);
			if (!air_atk)
				current_animation = &jump_down_right;
		}
		else
		{
			air_atking = false;
			player_atk->setRect({ -10, -10, 20, 30 });
		}
	else
		if (!wallhitL)
		{
			position.x -= App->SyncVelocity(exp_vel);
			if (!air_atk)
				current_animation = &jump_down_left;
		}
		else
		{
			air_atking = false;
			air_atking = false;
			player_atk->setRect({ -10, -10, 20, 30 });
		}
}

void Player::Slide()
{
	air_atking = false;

	if (facing_right)
	{
		if (!wallhitR)
		{
			current_animation = &slide_right;
			position.x += App->SyncVelocity(slide_vel);
		}
		else
		{
			sliding = false;
			slide_vel = exp_vel;
			timer = 0;
		}
	}
	else
	{
		if (!wallhitL)
		{
			current_animation = &slide_left;
			position.x -= App->SyncVelocity(slide_vel);
		}
		else
		{
			sliding = false;
			slide_vel = exp_vel;
			timer = 0;
		}
	}
	slide_vel -= App->SyncVelocity(ground_friction);
	if (slide_vel < 0.1f)
	{
		sliding = false;
		slide_vel = exp_vel;
	}
}

void Player::Dead()
{
	position.x = initialpos.x;
	position.y = initialpos.y;
	vo = 0.0f;
	timer = 0;
	facing_right = true;
	running = false;
	running = false;
	grounded = false;
	jumping = false;
	air_atking = false;
	air_atk = false;
	headcollided = false;
	sliding = false;
	App->scene->Reset();
	CleanUp();
}
