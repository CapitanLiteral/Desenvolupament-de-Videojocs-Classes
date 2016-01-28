#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;
class GuiImage;
class GuiLoadBar;
class GuiLabel;
class GuiHSlider;
class GuiMCursor;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called when UI event is raised
	void OnGui(Gui* ui, GuiEvents event);

private:
	SDL_Texture* debug_tex = NULL;
	GuiImage* button = NULL;
	GuiLoadBar* bar = NULL;
	Gui* window = NULL;
	GuiHSlider* sliderH = NULL;
	GuiLabel* title = NULL;

	GuiImage* image = NULL;
	GuiImage* help = NULL;

	int life = 1000;
	p2SString map_name;
	GuiMCursor* curs = NULL;

};

#endif // __j1SCENE_H__