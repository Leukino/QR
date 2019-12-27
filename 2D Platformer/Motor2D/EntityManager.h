#ifndef __EntityManager_H__
#define __EntityManager_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Entities.h"

class EntityManager : public j1Module
{
public:

	EntityManager();
	~EntityManager();

	bool Awake(pugi::xml_node& entities_data);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Entity* CreateEntity(float x, float y, entity_type type, bool exists = true);
	void DeleteEntity(int index);

	p2List<Entity*> Entities;

private:

	Entity* entity;
	int entity_id;
	pugi::xml_document	config_file;
	p2SString			folder;
	bool				entities_loaded;
};

#endif // __EntityManager_H__
