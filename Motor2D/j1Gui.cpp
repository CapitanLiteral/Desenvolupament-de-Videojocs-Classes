#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	next_id = 0;
	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_Image*>* Itmp = image_units.start;
	while (Itmp)
	{
		delete Itmp->data;
		Itmp = Itmp->next;
	}
	image_units.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

//--------------------------------------------------------------
UI_Image* j1Gui::CreateImage(const iPoint& p, const iPoint& s)
{
	UI_Image* ret = new UI_Image(p, s);
	
	ret->id = next_id;
	next_id++;
	image_units.add(ret);

	return ret;
}

UI_Image* j1Gui::CreateImage(const iPoint& p, const iPoint& s, SDL_Texture* img)
{
	UI_Image* ret = new UI_Image(p, s, img);

	ret->id = next_id;
	next_id++;
	image_units.add(ret);

	return ret;
}

bool j1Gui::Delete(UI_Image* img)
{
	p2List_item<UI_Image*>* tmp = image_units.At(image_units.find(img));

	if (image_units.del(tmp))
		return true;
	else
		return false;
}


//-----------------------------------------------
//-------UI_Unit---------------------------------
//-----------------------------------------------
UI_Unit::UI_Unit(const iPoint& p, const iPoint& s) : pos(p), size(s)
{}

UI_Unit::~UI_Unit()
{}

//-----------------------------------------------
//-------UI_Image--------------------------------
//-----------------------------------------------

UI_Image::UI_Image(const iPoint& p, const iPoint& s) : UI_Unit(p, s), image(NULL)
{}

UI_Image::UI_Image(const iPoint& p, const iPoint& s, SDL_Texture* img) : UI_Unit(p, s), image(img)
{}

UI_Image::~UI_Image()
{}

void UI_Image::Print()
{
	SDL_Rect sect;
	sect.x = size.x;
	sect.y = size.y;
	App->render->Blit(image, pos.x, pos.y, &sect);
}