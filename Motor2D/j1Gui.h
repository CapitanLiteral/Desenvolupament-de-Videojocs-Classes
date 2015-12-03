#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

enum UI_Events
{
	mouse_enter,
	mouse_out,
	mouseL_click,
	mouseR_click
};

// TODO 1: Create your structure of classes
class UI_Unit
{
public:
	iPoint position;

public:
	UI_Unit(const iPoint& pos);
	UI_Unit(const iPoint& pos, UI_Unit* parent);
	virtual ~UI_Unit();

	iPoint GetPos()const;
	void SetPosition(int x, int y);
	void Draw()const;
	virtual void Update(){}
	void SetParent(UI_Unit* _parent);

protected:
	virtual void Gui_Draw()const{}
	virtual void Gui_Draw_Debug()const{}

private:
	UI_Unit* parent;
};

class UI_Image : public UI_Unit
{
private:
	SDL_Texture* image;
	SDL_Rect section;

public:
	UI_Image(const iPoint& pos, SDL_Texture* img);
	UI_Image(const iPoint& pos, SDL_Rect sec, SDL_Texture* img);
	~UI_Image();

protected:
	void Gui_Draw()const;
};


class UI_Text : public UI_Unit
{
public:
	UI_Text(const iPoint& pos, const char* text);
	virtual ~UI_Text();

	void SetText(const char* text);

protected:
	void Gui_Draw()const;

private:
	SDL_Texture* text_texture;
};

class UI_Button : public UI_Unit
{
public:
	UI_Button(const iPoint& pos, const iPoint& size, j1Module* module);
	virtual ~UI_Button();

	void Update();
	void Gui_Draw_Debug()const;

private:
	iPoint size;
	j1Module* listener;
	bool mouse_over;
};

/*class UI_Input : public UI_Unit
{
public:
	UI_Input(const iPoint& pos, const iPoint& size);
	virtual ~UI_Input();

	void Update();
	void Gui_Draw_Debug()const;

private:
	iPoint size;
	j1Module* listener;
};*/
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

	// TODO 2: Create the factory methods
	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

	UI_Image* CreateImage(const iPoint& pos);
	UI_Image* CreateImage(const iPoint& pos, SDL_Rect sec);
	UI_Image* CreateImage(const iPoint& pos, SDL_Rect sec, const char* filename);

	UI_Text* CreateText(const iPoint& pos, const char* text);

	UI_Button* CreateButton(const iPoint& pos, const iPoint& size, j1Module* listenner);

	bool Delete(UI_Unit* img);

private:
	p2List<UI_Unit*> ui_elements;

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__