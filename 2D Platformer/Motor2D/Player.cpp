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
	position.x = initial_posX;
	position.y = initial_posY;
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

bool Player::Awake(pugi::xml_node& player_data)
{
	pugi::xml_node setup = player_data.child("player_data").child("setup");
		pugi::xml_node animate = setup.child("animate");
		sprite_wh = 60;
		xy_increase = 61;
			n_row = animate.attribute("n_row").as_int();
			n_coll = animate.attribute("n_coll").as_int();
	pugi::xml_node animations = player_data.child("player_data").child("animations");
		//pugi::xml_node idle_r = animations.child("idle_right");
		//Animate(idle_right, idle_r.attribute("first_col").as_int(), idle_r.attribute("first_row").as_int(), idle_r.attribute("n_sprites").as_int());
		//pugi::xml_node idle_l = animations.child("idle_left");
		//Animate(idle_left, idle_l.attribute("first_col").as_int(), idle_l.attribute("first_row").as_int(), idle_l.attribute("n_sprites").as_int());
		//idle_right.speed = animations.child("idle").attribute("speed").as_float();
		//idle_left.speed = animations.child("idle").attribute("speed").as_float();
		//pugi::xml_node run_r = animations.child("run_right");
		//Animate(run_right, run_r.attribute("first_col").as_int(), run_r.attribute("first_row").as_int(), run_r.attribute("n_sprites").as_int());
		//pugi::xml_node run_l = animations.child("run_left");
		//Animate(run_left, run_l.attribute("first_col").as_int(), run_l.attribute("first_row").as_int(), run_l.attribute("n_sprites").as_int());
		//pugi::xml_node j_u_r = animations.child("jump_up_right");
		//Animate(jump_up_right, j_u_r.attribute("first_col").as_int(), j_u_r.attribute("first_row").as_int(), j_u_r.attribute("n_sprites").as_int());
		//pugi::xml_node j_d_r = animations.child("jump_down_left");
		//Animate(jump_down_right, j_d_r.attribute("first_col").as_int(), j_d_r.attribute("first_row").as_int(), j_d_r.attribute("n_sprites").as_int());
		//pugi::xml_node j_u_l = animations.child("jump_up_left");
		//Animate(jump_up_left, j_u_l.attribute("first_col").as_int(), j_u_l.attribute("first_row").as_int(), j_u_l.attribute("n_sprites").as_int()); //don't juul in schuul
		//Animate(jump_down_left, 5, 1, 1);
		//Animate(slide_right, 2, 1, 1);
		//Animate(slide_left, 6, 1, 1);
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
	Animate(attack_right, 7, 1, 5);
	Animate(attack_left, 12, 1, 5);
	Animate(air_atk1_right, 0, 2, 5);
	Animate(air_atk2_right, 5, 2, 6);
	Animate(air_atk1_left, 11, 2, 5);
	Animate(air_atk2_left, 0, 3, 5);

	pugi::xml_node gameplay = player_data.child("player_data").child("gameplay");
	pugi::xml_node checkers = gameplay.child("checkers");
	facing_right = checkers.attribute("facing_right").as_bool();
	running = checkers.attribute("running").as_bool();
	grounded = checkers.attribute("grounded").as_bool();
	jumping = checkers.attribute("jumping").as_bool();
	air_atking = checkers.attribute("EXPUROSHON").as_bool();
	sliding = checkers.attribute("sliding").as_bool();
	pugi::xml_node counters = gameplay.child("counters");
	timer = counters.attribute("timer").as_int();
	attack_timer = 0;
	collissioncounter = counters.attribute("collisioncounter").as_int();
	wallcolcounter = counters.attribute("wallcolcounter").as_int();
	headcollided = false;
	attacking_idle = false;
	pugi::xml_node velocities = gameplay.child("velocities");
	run_vel = 2.0f;
	exp_vel = 4.0f;
	jump_vel = -10.0f;

	ground_friction = 0.15f;
	a = 0.5f;

	right_col = App->collision->AddCollider({ 0, 0, 5, 18 }, COLLIDER_PLAYER_RIGHT, this);
	left_col = App->collision->AddCollider({ 0, 0, 5, 18 }, COLLIDER_PLAYER_LEFT, this);
	feet_col = App->collision->AddCollider({ 0, 0, 13, 10 }, COLLIDER_PLAYER_FOOT, this);
	head_col = App->collision->AddCollider({ 0, 0, 10, 10 }, COLLIDER_PLAYER_HEAD, this);
	rightcol_offset.x = 34;
	rightcol_offset.y = 28;
	leftcol_offset.x = 20;
	leftcol_offset.y = 28;
	footcol_offset.x = 23;
	footcol_offset.y = 44;
	headcol_offset.x = 24;
	headcol_offset.y = 20;

	slide_vel = exp_vel;

	idle_right.speed = 0.01f;
	idle_left.speed = 0.01f;
	run_right.speed = 0.2f;
	run_left.speed = 0.2f;
	attack_right.speed = 0.1f;
	attack_left.speed = 0.1f;
	air_atk1_right.speed = 0.5f;
	air_atk2_right.speed = 0.5f;
	air_atk1_left.speed = 0.5f;
	air_atk2_left.speed = 0.5f;
	air_atk1_right.loop = false;
	air_atk2_right.loop = false;
	air_atk1_left.loop = false;
	air_atk2_left.loop = false;

	return true;
}

bool Player::CleanUp()
{
	return true;
}

bool Player::Update(float dt)
{
	//LOG("player: %f x, %f y", position.x, position.y);
	if (collissioncounter == 0)
		grounded = false;
	collissioncounter = 0;
	if (wallcolcounter == 0)
	{
		wallhitR = false;
		wallhitL = false;
	}
	wallcolcounter = 0;

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
		velocityY = jump_vel;
		vo = velocityY;
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

	if (!attacking_idle)
	{
		if (running)
		{
			if (facing_right && !wallhitR)
			{
				if (!jumping)
					current_animation = &run_right;
				position.x += run_vel;
			}
			if (!facing_right && !wallhitL)
			{
				if (!jumping)
					current_animation = &run_left;
				position.x -= run_vel;
			}
		}
		else
			if (facing_right && !jumping)
				current_animation = &idle_right;
			else if (!jumping)
				current_animation = &idle_left;
		if (jumping || (!grounded && !jumping))
			if (facing_right)
				current_animation = &jump_up_right;
			else
				current_animation = &jump_up_left;
	}
	else
	{
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

	//	if (attack_right.GetFrameNum() == 2 || attack_left.GetFrameNum() == 2)
	//		if (facing_right)
	//			player_atk = App->collision->AddCollider({ (int)position.x + 20, (int)position.y + 20, 20, 30 }, COLLIDER_PLAYER_ATK, this);
	//		else
	//			player_atk = App->collision->AddCollider({ (int)position.x, (int)position.y + 20, 20, 30 }, COLLIDER_PLAYER_ATK, this);
	//
	//	if (attack_right.GetFrameNum() == 3 || attack_left.GetFrameNum() == 3)
	//		player_atk->to_delete = true;
	}
	if (air_atking)
	{
		if (facing_right)
			if (!wallhitR)
				position.x += exp_vel;
			else
				air_atking = false;
		else
			if (!wallhitL)
				position.x -= exp_vel;
			else
				air_atking = false;
		if (air_atk)
		{
			if (facing_right)
			{
				current_animation = &air_atk1_right;
				if (air_atk1_right.GetFrameNum() == 4 && air_atk_counter != 0)
				{
					if (air_atk2_right.GetFrameNum() == 0)
					{
						velocityY = jump_vel;
						vo = velocityY;
						timer = 0;
					}
					current_animation = &air_atk2_right;
				}
				if (air_atk2_right.GetFrameNum() == 5)
					air_atk = false;
			}
			else
			{
				current_animation = &air_atk1_left;
				if (air_atk1_left.GetFrameNum() == 4 && air_atk_counter != 0)
				{
					if (air_atk2_left.GetFrameNum() == 0)
					{
						velocityY = jump_vel;
						vo = velocityY;
						timer = 0;
					}
					current_animation = &air_atk2_left;
				}
				if (air_atk2_left.GetFrameNum() == 4)
					air_atk = false;
			}
		}
		else
			if (facing_right)			
				current_animation = &jump_down_right;
			else
				current_animation = &jump_down_left;
	}

	if (!grounded)
	{
		timer++;
		velocityY = vo + a*timer;
		if (velocityY > 10.0f)
			velocityY = 10.0f;
		if (sliding)
			sliding = false;
	}
	else
	{
		headcollided = false;
		velocityY = 0.0f;
		if (!sliding)
		{
			timer = 0;
		}
	}
	if (headcollided && velocityY < 0)
		timer += 2;
	else
		position.y += velocityY;

	if (sliding)
	{
		timer++;
		air_atking = false;
		if (timer < 50)
		{
			if (facing_right)
			{
				if (!wallhitR)
				{
					current_animation = &slide_right;
					position.x += slide_vel;
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
					position.x -= slide_vel;
				}
				else
				{
					sliding = false;
					slide_vel = exp_vel;
					timer = 0;
				}
			}
			slide_vel -= ground_friction;
			if (slide_vel < 0.1f)
				slide_vel = 0.1;
		}
		else
		{
			sliding = false;
			slide_vel = exp_vel;
			timer = 0;
		}
	}

	right_col->SetPos(position.x + rightcol_offset.x, position.y + rightcol_offset.y);
	left_col->SetPos(position.x + leftcol_offset.x, position.y + leftcol_offset.y);
	feet_col->SetPos(position.x + footcol_offset.x, position.y + footcol_offset.y);
	head_col->SetPos(position.x + headcol_offset.x, position.y + headcol_offset.y);
	
	SDL_Rect& current_frame = current_animation->GetCurrentFrame();
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
		if (air_atking && !sliding)
		{
			air_atking = false;
			air_atk = false;
			sliding = true;
			timer = 0;
		}
		if(c2->rect.y != 0)
			position.y = c2->rect.y - 53;
		grounded = true;
		air_atk = false;
		air_atk_counter = 2;
		vo = 0.0f;
		collissioncounter++;
	}
	if (c1->type == COLLIDER_PLAYER_HEAD && c2->type == COLLIDER_GROUND)
	{
		headcollided = true;
	}
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
	if ((c1->type == COLLIDER_PLAYER_FOOT || c1->type == COLLIDER_PLAYER_HEAD) && c2->type == COLLIDER_ENEMY_SHOT)
	{
		position.x = initial_posX;
		position.y = initial_posY;
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
}