#ifndef __Player_H__
#define __Player_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "EntityManager.h"
#include "Animation.h"

class Player : public j1Module
{
public:

	Player();
	~Player();

	bool Awake(pugi::xml_node& player_data);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Animate(Animation& anim, const int first_coll, const int first_row, const int n, float speed = 1.0f, bool loop = true);
	void OnCollision(Collider* c1, Collider* c2);

	Animation* current_animation = nullptr;

private:

	SDL_Texture* player_sprites = nullptr;

public:
	//PASS TO XML
	fPoint position;
	float initial_posX;
	float initial_posY;
	int sprite_wh;
	int xy_increase;
	int n_row;
	int n_coll;

	Animation idle_right;
	Animation idle_left;
	Animation run_right;
	Animation run_left;
	Animation jump_up_right;
	Animation jump_down_right;
	Animation jump_up_left;
	Animation jump_down_left;
	Animation slide_right;
	Animation slide_left;
	Animation attack_right;
	Animation attack_left;
	Animation air_atk1_right;
	Animation air_atk1_left;
	Animation air_atk2_right;
	Animation air_atk2_left;

	Collider* right_col;
	Collider* left_col;
	Collider* head_col;
	Collider* feet_col;
	Collider* player_atk;
	iPoint rightcol_offset;
	iPoint leftcol_offset;
	iPoint footcol_offset;
	iPoint headcol_offset;

	bool godmode;
	bool facing_right;
	bool running;
	bool grounded;
	bool jumping;
	bool air_atking;
	bool air_atk;
	bool sliding;
	bool attacking_idle;
	bool wallhitR;
	bool wallhitL;
	bool headcollided;
	int collissioncounter;
	int wallcolcounter;
	int air_atk_counter;
	float timer;
	float run_vel;
	float exp_vel;
	float ground_friction;
	float slide_vel;
	float jump_vel;
	float a;
	float vo;
	float velocityY;
	

	
private:

	void Gravity(float dt);
	void Move();
	void Idle();
	void Jump();
	void Attack();
	void AirAttack();
	void AirMove();
	void Slide(float dt);

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;
};

#endif // __Player_H__