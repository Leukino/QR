#ifndef __Player_H__
#define __Player_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
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

	void Animate(Animation& anim, const int first_coll, const int first_row,const int n);
	void OnCollision(Collider* c1, Collider* c2);

	Animation* current_animation = nullptr;

private:

	SDL_Texture* player_sprites = nullptr;

public:
	//PASS TO XML
	fPoint position;
	float initial_posX = 200.0f;
	float initial_posY = 400.0f;
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

	Collider* right_col;
	Collider* left_col;
	Collider* head_col;
	Collider* feet_col;
	iPoint rightcol_offset;
	iPoint leftcol_offset;
	iPoint footcol_offset;
	iPoint headcol_offset;

	bool facing_right;
	bool running;
	bool grounded;
	bool jumping;
	bool EXPUROSHON;
	bool sliding;
	bool wallhitR;
	bool wallhitL;
	int collissioncounter;
	int headcollissioncounter;
	int wallcolcounter;
	int timer;
	float run_vel;
	float exp_vel;
	float ground_friction;
	float slide_vel;
	float jump_vel;
	float a;
	float vo;
	float velocityY;
	

	
private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;
};

#endif // __Player_H__