#include "p2Defs.h"
#include "p2Log.h"
#include "Entities.h"
#include <math.h>
#include <cstring>

Dynamic::Dynamic()
{
	LOG("it's Dynamic");
}

void Dynamic::Animate(Animation& anim, const int first_coll, const int first_row, const int n, float speed, bool loop)
{
	int coll = first_coll;
	int row = first_row;
	for (int i = 0; i < n; i++)
	{
		anim.PushBack({ 61 * coll, 61 * row, 60, 60 });
		coll++;
		if (coll == 17)
		{
			coll = 0;
			row++;
		}
	}
	anim.speed = speed;
	anim.loop = loop;
}