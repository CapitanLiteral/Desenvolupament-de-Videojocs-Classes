#ifndef __j1FilesManager_H__
#define __j1FilesManager_H__

#include "j1Module.h"

class j1FilesManager : public j1Module
{
public:

	j1FilesManager();

	virtual ~j1FilesManager();

	bool Awake();

	bool Start();

	bool LoadFile(const char* file);

	bool CleanUp();

public:

};


#endif