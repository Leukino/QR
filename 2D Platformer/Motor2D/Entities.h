#ifndef __Entities_H__
#define __Entities_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "Animation.h"

struct Collider;

enum entity_type
{
	player,
	enemy,
	walker,
	bat,
	dynamic,
	static_,
	coin,
	none
};

class Entity
{
public:

	Entity();

	virtual bool Update(float dt) { return true; }
	virtual bool CleanUp() { return true; }

	fPoint position;
	entity_type type;
	bool exists;
	int index;
	Collider* col;

	virtual void OnCollision(Collider*, Collider*) {}

};

class Dynamic :public Entity
{
public:

	Dynamic();
	//~Dynamic();
	void Animate(Animation& anim, const int first_coll, const int first_row, const int n, float speed = 1.0f, bool loop = true);

	fPoint initialpos;
	float velocity;

	Animation* current_animation = nullptr;

protected:

	SDL_Texture* sprites = nullptr;
};

class Player : public Dynamic
{
public:

	Player();

	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*);
	void SetGodMode();
	void Dead();

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
	int coins;
	float timer;
	float exp_vel;
	float ground_friction;
	float slide_vel;
	float jump_vel;
	float a;
	float vo;
	float vY;


private:

	inline void ResetColChecks();
	inline void Inputs();
	inline void Gravity(float dt);
	inline void Move();
	inline void Idle();
	inline void Jump();
	inline void Attack();
	inline void AirAttack();
	inline void AirMove();
	inline void Slide();
	
};

class Enemy :public Dynamic
{
public:

	Enemy();

protected:

	bool fliped;
};

class Walker : public Enemy
{
public:

	Walker();

	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*);

	Animation walk;
};

class Bat : public Enemy
{
public:

	Bat();

	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*);


	Animation rest;
	Animation fly;

};

class Static : public Entity
{
public:
	Static();
};

class Coin : public Static
{
public:
	Coin();

	bool Update(float dt);
	void OnCollision(Collider* c1, Collider* c2);
};



#endif // __Entities_H__

