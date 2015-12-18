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



//----Cvars
enum cvarType
{
	f_float,
	i_int,
	c_char,
	b_bool
};

struct cvar
{
	p2SString name;
	cvarType type;
	union value
	{
		float f;
		int i;
		char* c;
		bool b;
	}val;
	bool serielize;

	cvar(const char* c, float _value, cvarType _type)
	{
		name.create(c);
		type = _type;
		val.f = _value;
	}
	cvar(const char* c, int _value, cvarType _type)
	{
		name.create(c);
		type = _type;
		val.i = _value;
	}
	cvar(const char* c, char* _value, cvarType _type)
	{
		name.create(c);
		type = _type;
		val.c = _value;
	}
	cvar(const char* c, bool _value, cvarType _type)
	{
		name.create(c);
		type = _type;
		val.b = _value;
	}
};
template<class TYPE>
CreateCVar(const char* c, TYPE value, cvarType _type)
{
	switch (_type)
	{
	case f_float:
		cvar var(c, _value<float>, _type);
		break;
	case i_int:
		cvar var(c, _value<int>, _type);
		break;
	case c_char:
		cvar var(c, _value<char*>, _type);
		break;
	case b_bool:
		cvar var( c, _value<bool>, _type);
		break;
	default:
		break;
	}
}