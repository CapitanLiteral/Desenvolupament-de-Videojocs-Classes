#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
class UI_Unit
{
public:
	iPoint pos;
	iPoint size;
	int id;

public:
	UI_Unit(const iPoint& p, const iPoint& s);
	virtual ~UI_Unit();
};

class UI_Image : public UI_Unit
{
public:
	SDL_Texture* image;

public:
	UI_Image(const iPoint& p, const iPoint& s);
	UI_Image(const iPoint& p, const iPoint& s, SDL_Texture* img);
	~UI_Image();

	void Print();
};


class UI_Labbel : public UI_Unit
{

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

	UI_Image* CreateImage(const iPoint& p, const iPoint& s);
	UI_Image* CreateImage(const iPoint& p, const iPoint& s, SDL_Texture* img);

	bool Delete(UI_Image* img);

private:
	p2List<UI_Image*> image_units;
	int next_id;
	SDL_Texture* atlas;
	p2SString atlas_file_name;
};

#endif // __j1GUI_H__