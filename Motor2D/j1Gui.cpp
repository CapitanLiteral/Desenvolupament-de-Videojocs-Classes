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
		}

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

GuiHSlider* j1Gui::CreateHSlider(const rectangle& bar, const rectangle& thumb, const rectangle& bar_offset, const iPoint& thumb_margins, float value)
{
	GuiHSlider* ret = NULL;

	ret = new GuiHSlider(bar, thumb, bar_offset, thumb_margins, value);
	elements.add(ret);

	return ret;
}

GuiVSlider* j1Gui::CreateVSlider(const rectangle& bar, const rectangle& thumb, const rectangle& bar_offset, const iPoint& thumb_margins, float value)
{
	GuiVSlider* ret = NULL;

	ret = new GuiVSlider(bar, thumb, bar_offset, thumb_margins, value);
	elements.add(ret);

	return ret;
}

GuiMCursor* j1Gui::CreateGuiMCursor(const char* filename, int margin_x, int margin_y)
{
	GuiMCursor* ret = NULL;
	SDL_Texture* texture = App->tex->Load(filename);

	if (texture != NULL)
	{
		ret = new GuiMCursor(texture, margin_x, margin_y);
		elements.add(ret);
	}	

	return ret;
}

GuiMCursor* j1Gui::CreateGuiMCursor(const char* filename, const rectangle& section, int margin_x, int margin_y)
{
	GuiMCursor* ret = NULL;
	SDL_Texture* texture = App->tex->Load(filename);

	if (texture != NULL)
	{
		ret = new GuiMCursor(texture, section, margin_x, margin_y);
		elements.add(ret);
	}

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

bool j1Gui::Save(pugi::xml_node& config)const
{
	for (p2List_item<Gui*>* tmp = elements.start; tmp != NULL; tmp = tmp->next)
	{
		if (tmp->data->draggable == true)
		{
			pugi::xml_node dragUIElement = config.append_child("draggableUIelements");
			dragUIElement.append_attribute("position_x") = tmp->data->GetLocalPos().x;
			dragUIElement.append_attribute("position_y") = tmp->data->GetLocalPos().y;
		}
	}

	return true;
}

bool j1Gui::Load(pugi::xml_node& config)
{
	pugi::xml_node son = config.first_child();
	for (p2List_item<Gui*>* tmp = elements.start; tmp != NULL && son != NULL; tmp = tmp->next)
	{
		if (tmp->data->draggable == true)
		{
			int x = son.attribute("position_x").as_int(0);
			int y = son.attribute("position_y").as_int(0);
			tmp->data->SetLocalPos(x, y);
			son = son.next_sibling();
		}
	}

	return true;
}

bool j1Gui::Focus(Gui* gui)
{
	bool ret = false;
	if (gui)
	{
		focus = gui;
		ret = true;
	}
	return ret;
}