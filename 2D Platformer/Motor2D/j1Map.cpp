#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "EntityManager.h"
#include <math.h>
#include <cstring>
#include "brofiler/Brofiler/Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	first = true;
	folder.create(config.child("folder").child_value());

	return ret;
}

int id = 0;

void j1Map::DrawMapColliders()
{
	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					SDL_Rect r;
					COLLIDER_TYPE type;
					Entity* callback;
					iPoint pos = MapToWorld(x, y);
					for (uint i = 0; i < 210; ++i)
					{
						if (layer->collisions[i].id == tile_id - 1)
						{
							for (int e = 0; e < 20; e++)
							{
								r = layer->collisions[i].collider[e].rect;
								type = layer->collisions[i].collider[e].type;
								callback = layer->collisions[i].collider[e].callback;
								if (r.w != 0)
									App->collision->AddCollider({ pos.x + r.x, pos.y + r.y, r.w, r.h }, type, callback);
							}
						}
					}
				}
			}
		}
	}
}

void j1Map::DrawMapEntities()
{
	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.list.At(0)->data->value != 0)
			for (int y = 0; y < data.height; ++y)
				for (int x = 0; x < data.width; ++x)
				{
					int tile_id = layer->Get(x, y);
					if (tile_id > 0)
					{
						TileSet* tileset = GetTilesetFromTileId(tile_id);
						entity_type eType;
						iPoint pos = MapToWorld(x, y);
						for (uint i = 0; i < 210; ++i)
							if (layer->collisions[i].id == tile_id - 1)
							{
								eType = layer->collisions[i].eType;
								App->entities->CreateEntity(pos.x, pos.y, eType);
							}
					}
				}
	}
}

void j1Map::Draw()
{

	if (map_loaded == false)
		return;

	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.list.At(2)->data->value == 0)
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tile_id = layer->Get(x, y);
					if (tile_id > 0)
					{
						iPoint pos = MapToWorld(x, y);
						TileSet* tileset = GetTilesetFromTileId(tile_id);
						SDL_Rect r = tileset->GetTileRect(tile_id);
						App->render->Blit(tileset->texture, pos.x, pos.y, &r);
					}
				}
			}
	}
	first = false;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}
// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	data.tilesets.clear();
	data.layers.clear();
	App->entities->CleanUp();
	App->collision->CleanUp();
	map_file.reset();
	first = true;

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Tileset info
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet(); //create new Tileset Struct, load it's details and texture

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}
	// Layer info
	pugi::xml_node layercollset = map_file.child("map").child("tileset");
	pugi::xml_node layerset;
	for (layerset = map_file.child("map").child("layer"); layerset && ret; layerset = layerset.next_sibling("layer")) {

		MapLayer* newLayer = new MapLayer();
		if (ret == true)
		{
			ret = LoadLayer(layerset, newLayer);
			//ret = LoadLayerColliders(layercollset, newLayer);
		}
		data.layers.add(newLayer);
	}



	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}



	map_loaded = ret;
	DrawMapColliders();
	DrawMapEntities();
	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width * layer->height];
		memset(layer->data, 0, layer->width * layer->height);

		pugi::xml_node tsetnode = node.parent().child("tileset");
		int i = 0;
		SDL_Rect r;

		for (pugi::xml_node tile = tsetnode.child("tile"); tile.empty() == NULL; tile = tile.next_sibling("tile"))
		{
			if (!(tile.child("objectgroup").empty()))
			{
				int e = 0;
				for (pugi::xml_node tilenode = tile.child("objectgroup").child("object"); tilenode.empty() == NULL; tilenode = tilenode.next_sibling("object"))
				{
					int idesito = tilenode.parent().parent().attribute("id").as_int();
					r.x = tilenode.attribute("x").as_int();
					r.y = tilenode.attribute("y").as_int();
					r.w = tilenode.attribute("width").as_int();
					r.h = tilenode.attribute("height").as_int();

					COLLIDER_TYPE colltype = COLLIDER_NONE;
					entity_type enType = none;
					p2SString type = tilenode.attribute("type").as_string();

					if (SDL_strcmp(type.GetString(), "GROUND") == 0)
						colltype = COLLIDER_GROUND;
					else if (SDL_strcmp(type.GetString(), "WALL") == 0)
						colltype = COLLIDER_WALL;
					else if (SDL_strcmp(type.GetString(), "DANGER") == 0)
						colltype = COLLIDER_ENEMY_SHOT;
					else if (SDL_strcmp(type.GetString(), "coin") == 0)
						enType = coin;
					else if (SDL_strcmp(type.GetString(), "walker") == 0)
						enType = walker;
					else if (SDL_strcmp(type.GetString(), "bat") == 0)
						enType = bat;
					

					layer->collisions[i].collider[e].rect = r;
					LOG("rect set to: %d - %d - %d - %d number %d", r.x, r.y, r.w, r.h, e);
					layer->collisions[i].collider[e].type = colltype;
					layer->collisions[i].eType = enType;
					LOG("ENTITY_TYPE: %d", enType);
					layer->collisions[i].collider[e].callback = nullptr;
					layer->collisions[i].id = tilenode.parent().parent().attribute("id").as_int();
					LOG("id set to: %d", tilenode.parent().parent().attribute("id").as_int());
					e++;
				}
			}
			else
			{
				for (int e = 0; e < 20; e++)
				{
					layer->collisions[i].collider[e].rect = { 0, 0, 0, 0 };
					layer->collisions[i].collider[e].type = COLLIDER_NONE;
					layer->collisions[i].collider[e].callback = nullptr;
					layer->collisions[i].id = 0;
				}
			}
			i++;
		}

		i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			int gid = tile.attribute("gid").as_int(0);
			layer->data[i] = gid;
			//Loads all tile id's in the map
			i++;
		}
	}

	return ret;
}

bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			LOG("property name: %s", prop.attribute("name").as_string());
			p->value = prop.attribute("value").as_int();
			LOG("property value: %d", prop.attribute("value").as_int());

			properties.list.add(p);
		}
	}

	return ret;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(float x, float y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}
