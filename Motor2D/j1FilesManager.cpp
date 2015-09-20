#include "p2Log.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1FilesManager.h"

#include "PhysFS/include/physfs.h"
#pragma comment( lib, "PhysFS/libx86/physfs.lib")

j1FilesManager::j1FilesManager() : j1Module()
{
	name.create("filesM");
}

j1FilesManager::~j1FilesManager()
{}

bool j1FilesManager::Awake()
{
	LOG("Loading Files");
	bool ret = true;

	if (PHYSFS_init(NULL) == 0)
	{
		LOG("Could not create the FilesManager! PHYFS_ERROR: %s\n", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}

bool j1FilesManager::Start()
{
	LOG("FilesManager start");

	return true;
}


bool j1FilesManager::CleanUp()
{
	LOG("Destroying FilesManager");
	PHYSFS_deinit();
	return true;
}

bool j1FilesManager::LoadFile(const char* file)
{
	bool ret = true;

	return ret;
}