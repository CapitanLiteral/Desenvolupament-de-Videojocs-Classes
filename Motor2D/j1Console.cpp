#include "p2Defs.h"
#include "p2Log.h"

#include "j1FileSystem.h"
#include "j1Console.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "Gui.h"

#include "j1Console.h"


// -----j1Console--------------------------
j1Console::j1Console() : j1Module()
{
	name.create("console");
}

// Destructor
j1Console::~j1Console()
{}

bool j1Console::Awake(pugi::xml_node& config)
{
	bool ret = true;

	//Add here some commands


	return ret;
}

bool j1Console::Start()
{
	bool ret = true;

	InputText = App->gui->CreateInput({ 4, 351, 796, 43 }, "Command", 315, {0, 0}, false, 70);
	InputText->SetLocalPos(0, 0);
	InputText->interactive = true;
	InputText->can_focus = true;

	bool found = false;
	for (uint i = 0; i < tags.Count(); i++)
	{
		if (tags[i] == "Miscellaneous" && i != tags.Count() - 1)
		{
			found = true;
		}
		if (found && i != tags.Count() - 1)
		{
			tags[i] = tags[i + 1];
		}
	}
	if (found)
		tags[tags.Count() - 1] = "Miscellaneous";

	//Close();
	return ret;
}

bool j1Console::Update(float dt)
{
	bool ret = true;
	
	return ret;
}

bool j1Console::PostUpdate()
{
	bool ret = true;
	
	ret = !CloseGame;

	return ret;
}

// Called before quitting
bool j1Console::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1Console::OnGui(Gui* ui, GuiEvents event)
{
	if (ui == InputText)
	{
		if (event == GuiEvents::return_down)
		{
			GuiInputText* input = (GuiInputText*)ui;
			GetInput(input->GetString());
		}
	}
}

void AddCommand(Command* command);
uint AddCVar(const char* name, float* reference, j1Module* listener = NULL, bool serialize = false);
uint AddCVar(const char* name, int* reference, j1Module* listener = NULL, bool serialize = false);
uint AddCVar(const char* name, char* reference, j1Module* listener = NULL, bool serialize = false);
uint AddCVar(const char* name, bool* reference, j1Module* listener = NULL, bool serialize = false);

void GetInput(const char* src)
{
	p2DynArray<p2SString> S_input;
	CutString(src, &S_input);


}

void Open();
void Close();
void Clear();

void DisplayCommands(p2SString str)const;
void DisplayAllCommands()const;
void DisplayTags()const;

bool IsActive()const;

bool SaveCVars(pugi::xml_node&)const;
bool LoadCVars(pugi::xml_node&);

void CutString(const char* str, p2DynArray<p2SString>* dst);
Command* FindCommand(const char* str, uint nArgs)const;
CVar* FindCVar(const char* str);
void SetCVar(const char* calue);



// ------Command--------------------------
void Command::Function(const p2DynArray<p2SString>* args)
{
	LOG("Executing command function!");
}

// -----CVar------------------------------
CVar::CVar(const char*  name, float* ref, bool serialize)
{
	Name = name;
	type = cv_float;
	Serialize = serialize;
	Reference.fRef = ref;
	Value.fVal = *ref;
}

CVar::CVar(const char*  name, int* ref, bool serialize)
{
	Name = name;
	type = cv_int;
	Serialize = serialize;
	Reference.iRef = ref;
	Value.iVal = *ref;
}

CVar::CVar(const char*  name, char* ref, bool serialize)
{
	Name = name;
	type = cv_string;
	Serialize = serialize;
	Reference.cRef = ref;
	Value.cVal = ref;
}

CVar::CVar(const char*  name, bool* ref, bool serialize)
{
	Name = name;
	type = cv_bool;
	Serialize = serialize;
	Reference.bRef = ref;
	Value.bVal = *ref;
}

void CVar::LinkCommand(Command* _command)
{
	command = _command;
}

void CVar::Set(float val)
{
	if (type == cv_float)
	{
		*Reference.fRef = val;

		LOG("Set CVar %s to %f", Name.GetString(), val);
		if (Serialize)
		{
			Value.fVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->Function(NULL);
		}
	}
}

void CVar::Set(int val)
{
	if (type == cv_int)
	{
		*Reference.iRef = val;

		LOG("Set CVar %s to %f", Name.GetString(), val);
		if (Serialize)
		{
			Value.iVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->Function(NULL);
		}
	}
}

void CVar::Set(char* val)
{
	if (type == cv_string)
	{
		Reference.cRef = val;

		LOG("Set CVar %s to %f", Name.GetString(), val);
		if (Serialize)
		{
			Value.cVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->Function(NULL);
		}
	}
}

void CVar::Set(bool val)
{
	if (type == cv_bool)
	{
		*Reference.bRef = val;

		LOG("Set CVar %s to %f", Name.GetString(), val);
		if (Serialize)
		{
			Value.bVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->Function(NULL);
		}
	}
}

void CVar::SetListener(j1Module* _listener)
{
	listener = _listener;
}

CVarTypes CVar::GetType()
{
	return type;
}

p2SString CVar::GetName()
{
	return Name;
}

const j1Module* CVar::GetListener()const
{
	return listener;
}

void CVar::Read(void* val, CVarTypes expected)
{
	if(type == expected)
	{
		val = &Value;
	}
	else
	{
		LOG("Incorrect expected type!");
	}
}

bool CVar::Read(float* val)
{
	if (type == cv_float)
	{
		val = &Value.fVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::Read(int* val)
{
	if (type == cv_int)
	{
		val = &Value.iVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::Read(char* val)
{
	if (type == cv_string)
	{
		val = Value.cVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::Read(bool* val)
{
	if (type == cv_bool)
	{
		val = &Value.bVal;
		return true;
	}
	else
	{
		return false;
	}
}

void* CVar::ForceRead()
{
	return &Value;
}

void CVar::DisplayLog()
{
	switch (type)
	{
	case cv_float:
		LOG("Float value: %f", Value.fVal);
		break;
	case cv_int:
		LOG("Int value: %i", Value.iVal);
		break;
	case cv_string:
		LOG("String value: %s", Value.cVal);
		break;
	case cv_bool:
		LOG("Bool value: %b", Value.bVal);
		break;
	default:
		break;
	}
}