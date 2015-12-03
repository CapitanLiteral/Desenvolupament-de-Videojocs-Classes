#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
class UI_Unit
{
public:
	iPoint position;

public:
	UI_Unit(const iPoint& pos);
	virtual ~UI_Unit();

	iPoint GetPos()const;
	void SetPosition(int x, int y);
	void Draw()const;

//protected:
	//virtual void Gui_Draw()const;
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

//protected:
	void Gui_Draw()const;
};


class UI_Text : public UI_Unit
{
public:
	UI_Text(const iPoint& pos, const char* text);
	virtual ~UI_Text();

	void SetText(const char* text);
	void Gui_Draw()const;

private:
	SDL_Texture* text_texture;
};
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

	bool Delete(UI_Unit* img);

private:
	p2List<UI_Unit*> ui_elements;

	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__