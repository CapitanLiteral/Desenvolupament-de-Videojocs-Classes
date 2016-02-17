#ifndef __GUI_H__
#define __GUI_H__

#include "p2Point.h"

#define CURSOR_WIDTH 2

enum GuiEvents
{
	listening_ends,
	mouse_enters,
	mouse_leaves,
	mouse_lclick_down,
	mouse_lclick_up,
	mouse_rclick_down,
	mouse_rclick_up,
	gain_focus,
	lost_focus,
	input_changed,
	input_submit,
	value_changed,
	return_down
};

enum GuiTypes
{
	unknown,
	image,
	label,
	button,
	input_text,
	load_bar,
	h_slider,
	v_slider,
	mouse_cursor
};

// ---------------------------------------------------
class Gui
{
public:
	Gui();
	virtual ~Gui()
	{}

	virtual void Draw() const
	{}
	virtual void DebugDraw() const;
	virtual void Update(const Gui* mouse_hover, const Gui* focus)
	{}
	void CheckInput(const Gui* mouse_hover, const Gui* focus);
	void SetLocalPos(int x, int y);
	void Center();
	rectangle GetScreenRect() const;
	rectangle GetLocalRect() const;
	iPoint GetScreenPos() const;
	iPoint GetLocalPos() const;
	void SetListener(j1Module* module);
	void SetParent(Gui* dad);

protected:
	void SetSize(int w, int h);

public:
	bool draggable = false;
	bool interactive = false;
	bool cut_childs = false;
	bool can_focus = false;
	bool active = true;
	Gui* parent = nullptr;
	p2List<Gui*> childs;
	GuiTypes type = GuiTypes::unknown;

protected:
	j1Module* listener = nullptr;
	bool have_focus = false;

private:
	bool mouse_inside = false;
	rectangle rect;
};

// ---------------------------------------------------
class GuiImage : public Gui
{
public:
	GuiImage(const SDL_Texture* texture);
	GuiImage(const SDL_Texture* texture, const rectangle& section);
	~GuiImage();

	void SetSection(const rectangle& section);
	rectangle GetSection()const;
	void Draw() const;
	const SDL_Texture* GetTexture()const;

private:

	rectangle section;
	const SDL_Texture* texture = nullptr;
};

// ---------------------------------------------------
class GuiLabel : public Gui
{
public:
	GuiLabel();
	GuiLabel(const char* text);
	~GuiLabel();

	void SetText(const char* text);
	const SDL_Texture* GetTexture() const;
	void Draw() const;

private:

	SDL_Texture* texture = nullptr;
};

// ---------------------------------------------------
// TODO 1: Create a new InputText UI element with it's own label and image
// and draw it
class GuiInputText : public Gui
{
public:
	GuiInputText(const char* default_text, uint width, const SDL_Texture* texture, const rectangle& section, const iPoint& offset = { 0, 0 },
		bool password = false, int _max_quantity = 0);
	~GuiInputText();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw() const;
	const char* GetString()const;

private:

	GuiLabel text;
	int max_quantity;
	bool password;
	GuiImage image;
	p2SString input;
	iPoint cursor_coords = { 0, 0 };
	int last_cursor = 0;
	bool had_focus = false;
	const char* def_text;
	bool show_def_text;
};

class GuiLoadBar : public Gui
{
public:
	GuiLoadBar(int value, const SDL_Texture* texture, const rectangle& bar_sect, const rectangle& cover_sect, bool show_text);
	~GuiLoadBar();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw()const;
	void Grow(int percentage);
	void Decrease(int percentage);

public:
	GuiImage bar;
	GuiImage cover;
	GuiLabel percentage;
private:
	iPoint bar_size;
	int value;	//How much the hole bar represents
	char values[100];
	float actual_value_percentage = 100;		//How much the bar each iteration must represent
	rectangle bar_section;
	bool show_text;
};

class GuiHSlider : public Gui
{
public:
	GuiHSlider(const rectangle& bar, const rectangle& thumb, const rectangle& offset, iPoint margins, float value);
	~GuiHSlider();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw() const;
	float GetValue() const;
	void SetSliderValue(float value);

private:

	GuiImage bar;
	GuiImage thumb;
	iPoint margins;
	int min_x = 0;
	int max_x = 0;
	int thumb_pos = 0;
	float slider_value = 0;
};

class GuiVSlider : public Gui
{
public:
	GuiVSlider(const rectangle& bar, const rectangle& thumb, const rectangle& offset, iPoint margins, float value);
	~GuiVSlider();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw() const;
	float GetValue() const;
	void SetSliderValue(float value);

private:

	GuiImage bar;
	GuiImage thumb;
	iPoint margins;
	int min_y = 0;
	int max_y = 0;
	int thumb_pos = 0;
	float slider_value = 0;
};


class GuiMCursor : public Gui
{
public:
	GuiMCursor(const SDL_Texture* texture, int margin_x, int margin_y);
	GuiMCursor(const SDL_Texture* texture, const rectangle& section, int margin_x, int margin_y);
	~GuiMCursor();

	void SetSection(const rectangle& section);
	rectangle GetSection()const;
	void Draw() const;
	void Update(const Gui* mouse_hover, const Gui* focus);
	const SDL_Texture* GetTexture()const;

private:
	rectangle section;
	const SDL_Texture* curs = nullptr;
	iPoint margin;
};

#endif // __GUI_H__
