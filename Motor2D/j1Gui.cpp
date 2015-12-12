#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	//debug = true;
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

	SDL_Texture* curs = App->tex->Load("curs.png");
	mouse_cursor = new GuiImage(curs, rectangle{ 6, 0, 22, 32 });
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	mouse_cursor->SetLocalPos(mouse_pos.x, mouse_pos.y);

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	const Gui* mouse_hover = FindMouseHover();
	if(mouse_hover && 
	   mouse_hover->can_focus == true && 
	   App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_DOWN)
		focus = mouse_hover;

	p2List_item<Gui*>* item;

	// if TAB find the next item and give it the focus
	if(App->input->GetKey(SDL_SCANCODE_TAB) == j1KeyState::KEY_DOWN)
	{
		int pos = elements.find((Gui*) focus);
		if(pos > 0)
		{
			focus = nullptr;
			item = elements.At(pos);
			if(item)
				item = item->next;
			for(item; item; item = item->next )
				if (item->data->can_focus == true && item->data->active == true)
				{
					focus = item->data;
					break;
				}
		}
		if(focus == nullptr)
		{
			for(item = elements.start; item; item = item->next)
				if (item->data->can_focus == true && item->data->active == true)
				{
					focus = item->data;
					break;
				}
		}
	}

	// Now the iteration for input and update
	for(item = elements.start; item; item = item->next)
		if (item->data->interactive == true && item->data->active == true)
			item->data->CheckInput(mouse_hover, focus);

	for(item = elements.start; item; item = item->next)
		if (item->data->active == true)
		{
			item->data->Update(mouse_hover, focus);
			item->data->Update();
		}

	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
	mouse_cursor->SetLocalPos(mouse_pos.x, mouse_pos.y);

	return true;
}

// Called after all Updates
const Gui* j1Gui::FindMouseHover() const
{
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	for (p2List_item<Gui*>* item = elements.end; item; item = item->prev)
	{
		if (item->data->interactive == true)
		{
			if (item->data->GetScreenRect().Contains(mouse.x, mouse.y))
				return item->data;
		}
	}

	return nullptr;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
	{
		if (item->data->active == true)
		{
			item->data->Draw();
			if (debug == true)
				item->data->DebugDraw();
		}
	}

	mouse_cursor->Draw();

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
		RELEASE(item->data);

	elements.clear();

	delete mouse_cursor;

	return true;
}

// Create a simple image
GuiImage* j1Gui::CreateImage(const char* filename)
{
	GuiImage* ret = NULL;
	SDL_Texture* texture = App->tex->Load(filename);

	if(texture != NULL)
	{
		ret = new GuiImage(texture);
		elements.add(ret);
	}

	return ret;
}

// Create a simple image
GuiImage* j1Gui::CreateImage(const rectangle& section)
{
	GuiImage* ret = NULL;

	ret = new GuiImage(atlas, section);
	elements.add(ret);

	return ret;
}

// Create a simple image
GuiLabel* j1Gui::CreateLabel(const char* text)
{
	GuiLabel* ret = NULL;

	if(text != NULL)
	{
		ret = new GuiLabel(text);
		elements.add(ret);
	}

	return ret;
}

GuiInputText* j1Gui::CreateInput(const rectangle& section, const char* default_text, uint width, const iPoint& offset, bool password, int max_quantity)
{
	GuiInputText* ret = NULL;

	ret = new GuiInputText(default_text, width, atlas, section, offset, password, max_quantity);
	elements.add(ret);

	return ret;
}

GuiLoadBar* j1Gui::CreateBar(float value, const rectangle& bar_sect, const rectangle& cover_sect, bool show_text)
{
	GuiLoadBar* ret = NULL;

	ret = new GuiLoadBar(value, atlas, bar_sect, cover_sect, show_text);
	elements.add(ret);

	return ret;
}

GuiHSlider* j1Gui::CreateHSlider(const rectangle& bar, const rectangle& thumb, const rectangle& bar_offset, const iPoint& thumb_margins)
{
	GuiHSlider* ret = NULL;

	ret = new GuiHSlider(bar, thumb, bar_offset, thumb_margins);
	elements.add(ret);

	return ret;
}


// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::DisableGuiElement(Gui* elem)
{
	if (elem != NULL)
	{
		if (elem->childs.count() > 0)
		{
			for (p2List_item<Gui*>* i = elem->childs.end; i; i = i->prev)
			{
				DisableGuiElement(i->data);
			}
		}
		elem->active = false;
	}
}

void j1Gui::EnableGuiElement(Gui* elem)
{
	if (elem != NULL)
	{
		if (elem->childs.count() > 0)
		{
			for (p2List_item<Gui*>* i = elem->childs.end; i; i = i->prev)
			{
				EnableGuiElement(i->data);
			}
		}
		elem->active = true;
	}
}

bool j1Gui::DeleteGuiElement(Gui* elem)
{
	bool ret = false;
	if (elem != NULL)
	{
		if (elem->childs.count() > 0)
		{
			for (p2List_item<Gui*>* i = elem->childs.end; i; i = i->prev)
			{
				DeleteGuiElement(i->data);
			}
		}
		int pos = elements.find(elem);

		p2List_item<Gui*>* tmp = elements.At(pos);
		RELEASE(tmp->data);
		if (elements.del(tmp))
			ret = true;
	}
	return ret;
}