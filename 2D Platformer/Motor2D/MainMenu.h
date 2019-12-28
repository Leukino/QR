#ifndef __MainMenu_H__
#define __MainMenu_H__

#include "j1Module.h"
#include "ModuleUI.h"

class MainMenu : public j1Module
{
public:

	MainMenu();
	virtual ~MainMenu();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	Button* PlayButton;
	Button* ExitButton;
};

#endif // __MainMenu_H__
