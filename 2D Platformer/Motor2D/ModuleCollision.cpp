#include "ModuleCollision.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "Player.h"
#include "j1Render.h"
#include "j1Window.h"
#include <math.h>
#include <cstring>

ModuleCollision::ModuleCollision()
{
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_RIGHT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_WALL][COLLIDER_GROUND] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_PLAYER_RIGHT][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_PLAYER_LEFT][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_PLAYER_FOOT][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_PLAYER_HEAD][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_GROUND][COLLIDER_WALL] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_FOOT] = true;
	matrix[COLLIDER_GROUND][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_PLAYER_ATK][COLLIDER_WALL] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_PLAYER_RIGHT] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_PLAYER_LEFT] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_GROUND] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_PLAYER_FOOT] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_ENEMY_SHOT] = true;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_PLAYER_ATK][COLLIDER_PLAYER_ATK] = false;

	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_RIGHT] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_LEFT] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_FOOT] = true;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_ENEMY_SHOT] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_HEAD] = false;
	matrix[COLLIDER_ENEMY_SHOT][COLLIDER_PLAYER_ATK] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

bool ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Collision detection and callbacks 
	Collider* c1;
	Collider* c2;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if(colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		//int scale = App->win->GetScale();
		if(c1->rect.x > App->player->position.x - 50.0f && c1->rect.x < App->player->position.x + 50.0f && c1->rect.y > App->player->position.y - 50.0f && c1->rect.y < App->player->position.y + 100.0f)
		//if ((c1->rect.x * scale) > -App->render->camera.x && (c1->rect.x * scale) < -(App->render->camera.x - App->render->camera.w) && (c1->rect.y * scale) > -App->render->camera.y && (c1->rect.y * scale) < -(App->render->camera.y - App->render->camera.h))
			for(uint k = i+1; k < MAX_COLLIDERS; ++k)
			{
				// skip empty colliders
				if(colliders[k] == nullptr)
					continue;

				c2 = colliders[k];

				if(c1->CheckCollision(c2->rect) == true)
				{
					if(matrix[c1->type][c2->type] && c1->callback) 
						c1->callback->OnCollision(c1, c2);
					
					if(matrix[c2->type][c1->type] && c2->callback) 
						c2->callback->OnCollision(c2, c1);
				}
			}
	}


	return true;
}

// Called before render is available
bool ModuleCollision::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == false)
		return true;
	Uint8 alpha = 80;
	//int scale = App->win->GetScale();
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
			continue;
		if (colliders[i]->rect.x > App->player->position.x - 50.0f && colliders[i]->rect.x < App->player->position.x + 50.0f && colliders[i]->rect.y > App->player->position.y - 50.0f && colliders[i]->rect.y < App->player->position.y + 100.0f)
		//if ((colliders[i]->rect.x *scale) > -App->render->camera.x && (colliders[i]->rect.x * scale) < -(App->render->camera.x - App->render->camera.w) && (colliders[i]->rect.y * scale) > -App->render->camera.y && (colliders[i]->rect.y * scale) < -(App->render->camera.y - App->render->camera.h))
			switch(colliders[i]->type)
			{
				case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
				case COLLIDER_WALL: // blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
				break;
				case COLLIDER_PLAYER_RIGHT: // green
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
				case COLLIDER_PLAYER_LEFT: 
					App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
					break;
				case COLLIDER_GROUND: // red
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
				break;
				case COLLIDER_PLAYER_FOOT: // yellow
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
				break;
				case COLLIDER_PLAYER_HEAD: // yellow
					App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
					break;
				case COLLIDER_ENEMY_SHOT: // magenta
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
				break;
				case COLLIDER_PLAYER_ATK: // magenta
					App->render->DrawQuad(colliders[i]->rect, 255, 100, 100, alpha);
					break;
			}
	}

	return true;
}

// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* ModuleCollision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

void ModuleCollision::Delete_all()
{
	for (int i = 0; i < MAX_COLLIDERS; i++)
		if (colliders[i] != nullptr)
			colliders[i]->to_delete = true;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h + rect.y > r.y);
}

void Collider::set(SDL_Rect rec_, COLLIDER_TYPE type_, j1Module* callback_)
{
	//Collider* ret(rec, type, callback);
	this->rect.x = rec_.x;
	this->rect.y = rec_.y;
	this->rect.w = rec_.w;
	this->rect.h = rec_.h;
	this->type = type_;
	this->callback = callback_;
	return;
}