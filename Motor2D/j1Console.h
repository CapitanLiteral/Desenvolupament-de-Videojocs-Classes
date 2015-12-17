#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"

enum ComandType
{
	quit,
	undefined
};

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console();

	bool Awake();
	bool Update();
	bool CleanUp();

	void AddComand(const char* comand, ComandType _type, j1Module* listener);
	void RecibeComand(const char* comand);

private:
};

#endif