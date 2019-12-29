#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "j1Module.h"
#include "UIElements.h"


class ModuleUI : public j1Module
{
public:

	ModuleUI();
	virtual ~ModuleUI();

	bool Awake(pugi::xml_node& entities_data);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Button* CreateButton(iPoint pos, iPoint size, char* text = "", bool exists = true, bool lock = false);
	Console* CreateConsole(iPoint pos, iPoint size, bool exists = true);

	void DeleteUIElement(int index);

	p2List<UIElement*> UIElements;

private:

	UIElement* elem;
	int element_id;
	pugi::xml_document	config_file;
	p2SString			folder;
	bool				UI_loaded;
};


#endif // __jModuleUI_H__
