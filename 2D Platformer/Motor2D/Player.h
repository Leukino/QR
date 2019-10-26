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

	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Animate(Animation& anim, const int first_coll, const int first_row,const int n);
	//void OnCollision(Collider* c1, Collider* c2);

	Animation* current_animation = nullptr;

private:

	SDL_Texture* player_sprites = nullptr;

public:
	//PASS TO XML
	iPoint position;
	int speed = 3;
	int sprite_wh = 60;
	int xy_increase = 61;
	int n_row = 2;
	int n_coll = 17;

	Animation idle_right;
	Animation idle_left;
	Animation run_right;
	Animation run_left;

	Collider* right_col;
	Collider* left_col;
	Collider* head_col;
	Collider* feet_col;

	bool facing_right = true;
	bool running = false;
	int running_timer;
	

private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;
};

#endif // __Player_H__