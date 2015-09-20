#ifndef __j1FilesManager_H__
#define __j1FilesManager_H__

#include "j1Module.h"
#include "SDL\include\SDL.h"
#include "SDL_image\include\SDL_image.h"
#include "SDL_mixer\include\SDL_mixer.h"

class j1FilesManager : public j1Module
{
public:

	j1FilesManager();

	virtual ~j1FilesManager();

	bool Awake();

	bool Start();

	SDL_RWops* LoadFile(const char* file);

	bool CleanUp();

public:

	SDL_RWops* textures;

};


#endif