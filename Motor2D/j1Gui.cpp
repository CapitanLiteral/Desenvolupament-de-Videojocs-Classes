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
	p2List_item<UI_Unit*>* tmp = ui_elements.start;

	for (; tmp; tmp = tmp->next)
	{
		tmp->data->Draw();
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_Unit*>* tmp = ui_elements.start;
	while (tmp)
	{
		delete tmp->data;
		tmp = tmp->next;
	}
	ui_elements.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------

//--------------------------------------------------------------
UI_Image* j1Gui::CreateImage(const iPoint& pos)
{
	UI_Image* ret = new UI_Image(pos, atlas);
	
	ui_elements.add(ret);

	return ret;
}

UI_Image* j1Gui::CreateImage(const iPoint& pos, SDL_Rect sec)
{
	UI_Image* ret = new UI_Image(pos, sec, atlas);

	ui_elements.add(ret);

	return ret;
}

UI_Image* j1Gui::CreateImage(const iPoint& pos, SDL_Rect sec, const char* filename)
{
	UI_Image* ret = NULL;
	SDL_Texture* tex = App->tex->Load(filename);

	if (tex != NULL)
	{
		ret = new UI_Image(pos, sec, tex);
		ui_elements.add(ret);
	}
	return ret;
}

UI_Text* j1Gui::CreateText(const iPoint& pos, const char* text)
{
	UI_Text* ret = new UI_Text(pos, text);

	ui_elements.add(ret);

	return ret;
}

bool j1Gui::Delete(UI_Unit* elem)
{
	p2List_item<UI_Unit*>* tmp = ui_elements.At(ui_elements.find(elem));

	if (ui_elements.del(tmp))
		return true;
	else
		return false;
}


//-----------------------------------------------
//-------UI_Unit---------------------------------
//-----------------------------------------------
UI_Unit::UI_Unit(const iPoint& p) : position(p)
{}

UI_Unit::~UI_Unit()
{}

iPoint UI_Unit::GetPos()const
{
	return position;
}
void UI_Unit::SetPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

void UI_Unit::Draw()const
{
	//Gui_Draw();
}

//-----------------------------------------------
//-------UI_Image--------------------------------
//-----------------------------------------------

UI_Image::UI_Image(const iPoint& pos, SDL_Texture* img) : UI_Unit(pos), image(img)
{
	section.x = section.y = 0;
	App->tex->GetSize(image, (uint&) section.x, (uint&)section.y);
}

UI_Image::UI_Image(const iPoint& pos, SDL_Rect sec, SDL_Texture* img) : UI_Unit(pos), section(sec), image(img)
{}

UI_Image::~UI_Image()
{}

void UI_Image::Gui_Draw()const
{
	App->render->Blit(image, position.x, position.y, &section);
}

//-----------------------------------------------
//-------UI_Text---------------------------------
//-----------------------------------------------

UI_Text::UI_Text(const iPoint& pos, const char* text) : UI_Unit(pos)
{
	text_texture = App->font->Print(text);
}

UI_Text::~UI_Text()
{}

void UI_Text::SetText(const char* text)
{
	if (text_texture != NULL)
		SDL_DestroyTexture(text_texture);
	text_texture = App->font->Print(text);
}

void UI_Text::Gui_Draw()const
{
	App->render->Blit(text_texture, position.x, position.y);
}