#include "j1Textures.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "Gui.h"
#include "j1App.h"

// class Gui ---------------------------------------------------
Gui::Gui() : rect({0,0,0,0})
{
}

void Gui::SetLocalPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Gui::Center()
{
	int frame_w = (parent) ? parent->GetLocalRect().w : App->render->camera.w;
	int frame_h = (parent) ? parent->GetLocalRect().h : App->render->camera.h;

	SetLocalPos(frame_w / 2 - rect.w / 2, frame_h / 2 - rect.h / 2);
}

void Gui::SetSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

rectangle Gui::GetLocalRect() const
{
	return rect;
}

rectangle Gui::GetScreenRect() const
{
	if(parent != nullptr)
	{
		iPoint p = GetScreenPos();
		return {p.x, p.y, rect.w, rect.h};
	}
	return rect;
}

iPoint Gui::GetScreenPos() const
{
	if(parent != nullptr)
		return parent->GetScreenPos() + iPoint(rect.x, rect.y);
	else
		return iPoint(rect.x, rect.y);
}

iPoint Gui::GetLocalPos() const
{
	return iPoint(rect.x, rect.y);
}

void Gui::DebugDraw() const
{
	rectangle r = GetScreenRect();
	App->render->DrawQuad({r.x, r.y, r.w, r.h}, 255, 0, 0, 255, false, false);
}

void Gui::SetListener(j1Module* module)
{
	if(listener != nullptr)
		listener->OnGui(this, GuiEvents::listening_ends);

	listener = module;
}

void Gui::CheckInput(const Gui* mouse_hover, const Gui* focus)
{
	bool inside = (mouse_hover == this);

	if(mouse_inside != inside)
	{
		mouse_inside = inside;
		if(listener != nullptr)
			listener->OnGui(this, (inside) ? GuiEvents::mouse_enters : GuiEvents::mouse_leaves);
	}

	if(inside == true)
	{
		if(listener != nullptr)
		{
			if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_DOWN)
				listener->OnGui(this, GuiEvents::mouse_lclick_down);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_UP)
				listener->OnGui(this, GuiEvents::mouse_lclick_up);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == j1KeyState::KEY_DOWN)
				listener->OnGui(this, GuiEvents::mouse_rclick_down);

			if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == j1KeyState::KEY_UP)
				listener->OnGui(this, GuiEvents::mouse_lclick_up);
		}

		if(draggable == true && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == j1KeyState::KEY_REPEAT)
		{
			iPoint p = GetLocalPos();
			iPoint motion;
			App->input->GetMouseMotion(motion.x, motion.y);
			SetLocalPos(p.x + motion.x, p.y + motion.y);
		}
	}

	if(have_focus != (focus == this))
	{
		if(listener != nullptr)
		{
			if(focus == this)
				listener->OnGui(this, GuiEvents::gain_focus);
			else
				listener->OnGui(this, GuiEvents::lost_focus);
		}
		have_focus = (focus == this);
	}

	if(have_focus == true && listener != nullptr)
	{
		if(App->input->GetKey(SDL_SCANCODE_RETURN) == j1KeyState::KEY_DOWN)
			listener->OnGui(this, GuiEvents::mouse_lclick_down);

		if(App->input->GetKey(SDL_SCANCODE_RETURN) == j1KeyState::KEY_UP)
			listener->OnGui(this, GuiEvents::mouse_lclick_up);
	}
}

void Gui::SetParent(Gui* dad)
{
	this->parent = dad;
	dad->childs.add(this);
}

// class GuiImage ---------------------------------------------------
GuiImage::GuiImage(const SDL_Texture* texture) : Gui(), texture(texture)
{
	section.x = section.y = 0;
	App->tex->GetSize(texture, (uint&)section.w, (uint&)section.h);

	SetSize(section.w, section.h);
	type = GuiTypes::image;
}

// --------------------------
GuiImage::GuiImage(const SDL_Texture* texture, const rectangle& section) : Gui(), texture(texture), section(section)
{
	SetSize(section.w, section.h);
}

// --------------------------
GuiImage::~GuiImage()
{}

//---------------------------
rectangle GuiImage::GetSection()const
{
	return section;
}

// --------------------------
void GuiImage::SetSection(const rectangle& section)
{
	this->section = section;
}

// --------------------------
void GuiImage::Draw() const
{
	iPoint p = GetScreenPos();
	if(parent && parent->cut_childs)
	{
		rectangle r = parent->GetScreenRect();
		App->render->SetViewPort({r.x, r.y, r.w, r.h});
		p = GetLocalPos();
	}
	App->render->Blit(texture, p.x, p.y, (SDL_Rect*) &section, 0.0f);

	if(parent && parent->cut_childs)
		App->render->ResetViewPort();
}

//----------------------------

const SDL_Texture* GuiImage::GetTexture()const
{
	return texture;
}

// class GuiLabel ---------------------------------------------------
GuiLabel::GuiLabel(const char* text) : Gui()
{
	SetText(text);
	type = GuiTypes::label;
}

// --------------------------
GuiLabel::~GuiLabel()
{
	if(texture != nullptr && texture != App->gui->GetAtlas())
		App->tex->UnLoad(texture);
}

// --------------------------
void GuiLabel::SetText(const char* text)
{
	if(texture != nullptr)
		SDL_DestroyTexture(texture);

	texture = App->font->Print(text);
	int w, h;
	App->tex->GetSize(texture, (uint&)w, (uint&)h);
	SetSize(w, h);
}

// --------------------------
const SDL_Texture* GuiLabel::GetTexture() const
{
	return texture;
}

// --------------------------
void GuiLabel::Draw() const
{
	iPoint p = GetScreenPos();
	App->render->Blit(texture, p.x, p.y, NULL, 0.0f);
}

// class GuiInputText ---------------------------------------------------
GuiInputText::GuiInputText(const char* default_text, uint width, const SDL_Texture* texture, const rectangle& section, const iPoint& offset, bool pass, int _max_quantity)
	: Gui(), text(default_text), input(default_text), image(texture, section)
{
	type = GuiTypes::input_text;
	SetSize(width, text.GetScreenRect().h);
	text.parent = this;
	image.parent = this;
	image.SetLocalPos(offset.x, offset.y);
	max_quantity = _max_quantity;
	password = pass;
	def_text = default_text;
	show_def_text = true;

	// Calculate the Y so we have it ready
	App->font->CalcSize("A", cursor_coords.x, cursor_coords.y);
	if (max_quantity == 0)
		max_quantity = width / cursor_coords.x;
	cursor_coords.x = 0;
}

// --------------------------
GuiInputText::~GuiInputText()
{
}

// --------------------------
void GuiInputText::Update(const Gui* mouse_hover, const Gui* focus)
{
	if (interactive == false)
		return;

	bool inside = (mouse_hover == this);
	bool have_focus = (focus == this);

	// TODO : focus
	if (had_focus != have_focus)
	{
		if (have_focus == true)
		{
			if (show_def_text == true)
			{
				input.Clear();
				show_def_text = false;
			}
			App->input->StartTextInput(nullptr, input);
		}
		else
		{
			if (input.Length() == 0)
			{
				input = def_text;
				show_def_text = true;
				text.SetText(input.GetString());
			}
			if (focus->type != GuiTypes::input_text)
				App->input->EndTextInput();
		}

		had_focus = have_focus;
	}

	static p2SString selected(100);

	if (have_focus == true)
	{
		// TODO 5: Calculate where the cursor has to be placed and update your label
		// in the InputText ui element
		int cursor, selection;
		p2SString user_input = App->input->GetTextInput(cursor, selection);

		if (input != user_input || cursor != last_cursor)
		{
			if (input != user_input)
			{
				if (user_input.Length() > max_quantity)
				{
					user_input = input;
					cursor = last_cursor;
					App->input->TextInputTooLong();
				}

				input = user_input;
				if (password == true)
				{
					p2SString pass_hided;
					for (int x = input.Length(); x > 0; x--)
						pass_hided.Insert(0, "*");
					text.SetText(pass_hided.GetString());
				}
				else
					text.SetText(user_input.GetString());

				if (listener != nullptr)
					listener->OnGui(this, GuiEvents::input_changed);
			}

			last_cursor = cursor;
			if (cursor > 0)
			{
				if (input.Length() >= selected.GetCapacity())
					selected.Reserve(input.Length() * 2);
				if (password == true)
				{
					p2SString pass_hided;
					for (int x = input.Length(); x > 0; x--)
						pass_hided.Insert(0, "*");
					pass_hided.SubString(0, cursor, selected);
					App->font->CalcSize(selected.GetString(), cursor_coords.x, cursor_coords.y);
				}
				else
				{
					input.SubString(0, cursor, selected);
					App->font->CalcSize(selected.GetString(), cursor_coords.x, cursor_coords.y);
				}
			}
			else
			{
				cursor_coords.x = 0;
			}
		}

		if (selection != 0 && listener != nullptr)
		{
			listener->OnGui(this, GuiEvents::input_submit);
		}
	}
}

// --------------------------
void GuiInputText::Draw() const
{
	// render image
	image.Draw();

	// render text
	if (input.Length() > 0)
		text.Draw();

	// render cursor
	if (have_focus == true)
	{
		// TODO 2: Draw cursor when focus is received
		// use a simple DrawQuad. For the size and position use
		// App->font->CalcSize
		iPoint pos = GetScreenPos();
		App->render->DrawQuad({ pos.x + (cursor_coords.x - (CURSOR_WIDTH / 2)), pos.y, CURSOR_WIDTH, cursor_coords.y }, 255, 255, 255, 255, true, false);
	}
}

GuiLoadBar::GuiLoadBar(int value, const SDL_Texture* texture, const rectangle& bar_sect, const rectangle& cover_sect) :
value(value), bar(texture, bar_sect), cover(texture, cover_sect), bar_section(bar_sect), actual_value(value)
{
	type = GuiTypes::load_bar;
	cover.SetParent(this);
	bar.SetParent(&cover);
	bar_size.x = bar.GetSection().w;
	bar_size.y = bar.GetSection().h;
	SetSize(cover_sect.w, cover_sect.h);
}

GuiLoadBar::~GuiLoadBar()
{
}

void GuiLoadBar::SetBar(int _value)
{
	actual_value = _value;
}

void GuiLoadBar::Update()
{
	bar_section.w = ((bar_size.x * actual_value) / value);
	bar_section.h = bar_size.y;
	bar.SetSection(bar_section);
}

void GuiLoadBar::Draw()const
{
	cover.Draw();
	bar.Draw();
}