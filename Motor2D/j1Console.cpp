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

void j1Console::AddComand(const char* comand, ComandType _type)
{
	Comand ret(comand, _type);
	comands.add(&ret);
}

void j1Console::RecibeComand(const char* comand)
{
	Comand ret(comand);
	p2SString str(comand);
	for (p2List_item<Comand*>* i = comands.start; i; i = i->next)
	{
		if (i->data->comandName == str)
			i->data->listener->OnComand(i->data->type);
	}
}