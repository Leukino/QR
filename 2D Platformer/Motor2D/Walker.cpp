#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

Walker::Walker()
{
	LOG("it's a Walker");
}

bool Walker::Update(float dt)
{
	return true;
}

bool Walker::CleanUp()
{
	return true;
}

void Walker::OnCollision(Collider*, Collider*)
{

}