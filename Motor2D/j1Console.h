#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"

enum ComandType
{
	quit,
	undefined
};

struct Comand
{
public:
	Comand(const char* name)
	{
		comandName.create(name);
	}
	Comand(const char* name, ComandType _type)
	{
		comandName.create(name);
		type = _type;
	}
	~Comand()
	{}

public:
	j1Module* listener;
	ComandType type = undefined;
	p2SString comandName;	
};

class j1Console : public j1Module
{
public:
	j1Console();
	~j1Console();

	bool Awake();
	bool Update();
	bool CleanUp();

	void AddComand(const char* comand, ComandType _type);
	void RecibeComand(const char* comand);

private:
	p2List<Comand*> comands;
};

#endif