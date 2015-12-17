#include "j1Console.h"

j1Console::j1Console() : j1Module()
{
	name.create("console");
}

j1Console::~j1Console()
{}

bool j1Console::Awake()
{
	bool ret = true;

	return ret;
}

bool j1Console::CleanUp()
{
	bool ret = true;

	return ret;
}

bool j1Console::Update()
{
	bool ret = true;

	return ret;
}

void j1Console::AddComand(const char* comand, ComandType _type, j1Module* listener)
{
}

void j1Console::RecibeComand(const char* comand)
{
}