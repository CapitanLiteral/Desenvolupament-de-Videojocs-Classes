#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1FileSystem.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	player_x = player_y = 10;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	map.create(config.child("map").child_value());
	camera_speed_x = config.child("cameraspeed").attribute("x").as_int();
	camera_speed_y = config.child("cameraspeed").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(map.GetString());
	App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	debug_tex = App->tex->Load("textures/path.png");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	//Delete file
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->fs->deleteFile("save_game.xml");

	// Save / Load 
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if(App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	// Move the camera
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += camera_speed_y;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= camera_speed_y;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += camera_speed_x;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= camera_speed_x;

	//Volume
	if ((App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) && App->audio->music_volume < 128)
		App->audio->music_volume += 4;

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN && App->audio->music_volume > 0)
		App->audio->music_volume -= 4;

	// Move the player
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		player_x--;
		player_y--;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		player_x++;
		player_y++;
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		player_x--;
		player_y++;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		player_x++;
		player_y--;
	}

	// Map render
	App->map->Draw();

	// Player placeholder
	iPoint p = App->map->MapToWorld(player_x, player_y);
	App->render->Blit(debug_tex, p.x, p.y);

	// Set Title
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d Volume: %i Camera.x: %i Camera.y: %i",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y,
		App->audio->music_volume,
		App->render->camera.x,
		App->render->camera.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// Load Game State
bool j1Scene::Load(pugi::xml_node& data)
{
	player_x = data.child("playerpos").attribute("x").as_int();
	player_y = data.child("playerpos").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("playerpos");

	cam.append_attribute("x") = player_x;
	cam.append_attribute("y") = player_y;

	return true;
}