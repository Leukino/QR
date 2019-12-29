#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "EntityManager.h"
#include "ModuleUI.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();
	virtual ~j1Scene();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	void changeMap();

	void Reset();
	bool CleanUp();

	Entity* pl;

	Button* MenuButton;

private:

	int map;
};

#endif // __j1SCENE_H__