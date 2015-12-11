#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"

class Gui;
class GuiImage;
class GuiLabel; 
class GuiInputText;
class GuiLoadBar;

struct SDL_Texture;

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	GuiImage* CreateImage(const char* filename);
	GuiImage* CreateImage(const rectangle& atlas_section);
	GuiLabel* CreateLabel(const char* text);
	GuiInputText* CreateInput(const rectangle& section, const char* default_text, uint width, const iPoint& offset = { 0, 0 }, bool password = false, int max_quantity = 0);
	GuiLoadBar* CreateBar(float value, const rectangle& bar_sect, const rectangle& cover_sect, bool show_text = false);
	bool DeleteGuiElement(Gui* elem);
	void DisableGuiElement(Gui* elem);
	void EnableGuiElement(Gui* elem);

	const Gui* FindMouseHover() const;
	const SDL_Texture* GetAtlas() const;

private:

	bool debug = false;
	p2List<Gui*> elements;
	const Gui* focus = nullptr;
	SDL_Texture* atlas = nullptr;
	p2SString atlas_file_name;

	GuiImage* mouse_cursor;
	iPoint mouse_pos;
};

#endif // __j1GUI_H__