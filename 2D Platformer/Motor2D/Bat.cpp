#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

Bat::Bat()
{
	LOG("it's a Bat");
}

bool Bat::Update(float dt)
{
	return true;
}

bool Bat::CleanUp()
{
	return true;
}

void Bat::OnCollision(Collider*, Collider*)
{

}