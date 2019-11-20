/*#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

class ModuleFadeToBlack : public j1Module
{
public:
	ModuleFadeToBlack();
	~ModuleFadeToBlack();

	bool Start();
	bool Update();
	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool IsFading() const;

private:
	j1Module * moduleoff;
	j1Module * moduleon;
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__MODULEFADETOBLACK_H__*/