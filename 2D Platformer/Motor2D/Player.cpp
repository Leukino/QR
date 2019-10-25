#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
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
	graphics = App->tex->Load("Sprites/textures/Knight.png");
	position.x = 10;
	position.y = 10;
	return true;
}

bool Player::Awake(pugi::xml_node& conf)
{
	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::Update()
{}