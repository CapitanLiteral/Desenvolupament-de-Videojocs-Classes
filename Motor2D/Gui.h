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
	input_submit
};

enum GuiTypes
{
	unknown,
	image,
	label,
	button,
	load_bar
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
	virtual void Update()
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
protected:
	GuiTypes type = GuiTypes::unknown;
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
	GuiInputText(const char* default_text, uint width, const SDL_Texture* texture, const rectangle& section, const iPoint& offset = {0, 0});
	~GuiInputText();

	void Update(const Gui* mouse_hover, const Gui* focus);
	void Draw() const;

private:

	GuiLabel text;
	GuiImage image;
	p2SString input;
	iPoint cursor_coords = {0, 0};
	int last_cursor = 0;
	bool had_focus = false;
};

class GuiLoadBar : public Gui
{
public:
	GuiLoadBar(float value, const SDL_Texture* texture, const rectangle& bar_sect, const rectangle& cover_sect);
	~GuiLoadBar();

	void Update();
	void Draw()const;
	void SetBar(float actual_value);

public:
	GuiImage bar;
	GuiImage cover;
private:
	iPoint bar_size;
	float value;	//How much the hole bar represents
	float actual_value;		//How much the bar each iteration must represent
	rectangle bar_section;
};

#endif // __GUI_H__
