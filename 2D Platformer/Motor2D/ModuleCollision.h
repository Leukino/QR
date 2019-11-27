#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 10000

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER_RIGHT,
	COLLIDER_PLAYER_LEFT,
	COLLIDER_GROUND,
	COLLIDER_PLAYER_FOOT,
	COLLIDER_PLAYER_HEAD,
	COLLIDER_ENEMY_SHOT,
	COLLIDER_PLAYER_ATK,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}
	Collider()
	{
		rect.x = 0;
		rect.y = 0;
		rect.w = 0;
		rect.h = 0;
		type = COLLIDER_NONE;
		callback = nullptr;
	}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	void set(SDL_Rect rec, COLLIDER_TYPE type, j1Module* callback);
};

class ModuleCollision : public j1Module
{
public:
	friend class j1Map;
	ModuleCollision();
	~ModuleCollision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	void Delete_all();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);

private:

	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __ModuleCollision_H__