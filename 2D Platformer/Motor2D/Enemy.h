#ifndef __Enemy_H__
#define __Enemy_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "EntityManager.h"

#include "Animation.h"
#include "j1Textures.h"

class Enemy : public Entity
{
public:

	Enemy();

	bool Update(float dt); 
	bool CleanUp(); 

	Animation* current_animation = nullptr;

	Animation idle;

private:

	SDL_Texture* enemy_sprites = nullptr;
};

#endif // __Enemy_H__
