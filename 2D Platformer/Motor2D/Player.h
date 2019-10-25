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
	void Update();
	bool CleanUp();

	Animation* current_animation = nullptr;

private:

	SDL_Texture* graphics = nullptr;

public:
	//PASS TO XML
	iPoint position;
	int speed = 3;

private:

	pugi::xml_document	player_file;
	p2SString			folder;
	bool				player_loaded;
};

#endif // __Player_H__