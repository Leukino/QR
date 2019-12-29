#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

#include "ModuleCollision.h"
#include "EntityManager.h"
#include "j1Render.h"

Coin::Coin()
{
	LOG("Created Coin");
	col = App->collision->AddCollider({ (int)position.x, (int)position.y, 20, 20}, COLLIDER_COIN, this);
}

bool Coin::Update(float dt)
{
	col->SetPos((int)position.x, (int)position.y);
	App->render->DrawQuad({ (int)position.x, (int)position.y, 20, 20 }, 189, 183, 107);
	return true;
}

void Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_COIN && (c2->type == COLLIDER_PLAYER_FOOT || c2->type == COLLIDER_PLAYER_HEAD || c2->type == COLLIDER_PLAYER_RIGHT || c2->type == COLLIDER_PLAYER_LEFT))
	{
		col->to_delete = true;
		App->entities->DeleteEntity(index);
	}
}