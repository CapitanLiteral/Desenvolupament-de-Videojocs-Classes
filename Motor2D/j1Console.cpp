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
	AddCommand(&c_Quit);
	AddCommand(&c_Close);

	return ret;
}

bool j1Console::Start()
{
	bool ret = true;

	InputText = App->gui->CreateInput({ 4, 351, 796, 43 }, "", 315, {0, 0}, false, 70);
	InputText->SetLocalPos(0, 300);
	InputText->interactive = true;
	InputText->can_focus = true;
	InputText->SetListener(this);
	InputText->active = false;

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
	
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (IsActive())
		{
			Close();
		}
		else
		{
			Open();
		}
	}

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
			InputText->Clear();
		}
	}
}

void j1Console::AddCommand(Command* command)
{
	commandList.add(command);

	bool found = false;
	for (uint i = 0; i < tags.Count(); i++)
	{
		if (command->Tag == tags[i])
			found = true;
	}
	if (!found)
		tags.PushBack(command->Tag);
}

uint j1Console::AddCVar(const char* name, float* reference, j1Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->SetListener(listener);
	CVarList.add(ret);
	return CVarList.count() - 1;
}

uint j1Console::AddCVar(const char* name, int* reference, j1Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->SetListener(listener);
	CVarList.add(ret);
	return CVarList.count() - 1;
}

uint j1Console::AddCVar(const char* name, char* reference, j1Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->SetListener(listener);
	CVarList.add(ret);
	return CVarList.count() - 1;
}

uint j1Console::AddCVar(const char* name, bool* reference, j1Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->SetListener(listener);
	CVarList.add(ret);
	return CVarList.count() - 1;
}

void j1Console::GetInput(const char* src)
{
	p2DynArray<p2SString> S_input;
	CutString(src, &S_input);

	Command* com = FindCommand(S_input[0].GetString(), S_input.Count() - 1);
	if (com)
	{
		com->Function(&S_input);
	}
	else
	{
		CVar* cVar = FindCVar(S_input.At(0)->GetString());
		if (cVar)
		{
			if (S_input.Count() == 1)
			{
				cVar->DisplayLog();
			}
			else if (S_input.Count() == 2 || S_input.Count() == 3)
			{
				if (S_input.Count() == 3)
				{
					cVar->Serialize = std::stoi(S_input.At(2)->GetString());
				}
				p2SString* str = S_input.At(1);
				cVar->Set(str);
			}
			else
			{
				LOG("Sent too many arguments");
			}
		}
		else
		{
			LOG("'%s' is not an intern command.", S_input[0].GetString());
		}
	}
	S_input.Clear();
}

void j1Console::Output(char* str)
{
	GuiLabel* out = App->gui->CreateLabel(str);
	//Set the labbel position according to the scroll
	output.PushBack(out);
}

void j1Console::Open()
{
	InputText->active = true;
	Active = true;
	App->gui->Focus(InputText);
}

void j1Console::Close()
{
	InputText->active = false;
	Active = false;
	InputText->Clear();
}

void j1Console::Clear()
{

}

void j1Console::DisplayCommands(p2SString str)const
{

}

void j1Console::DisplayAllCommands()const
{

}

void j1Console::DisplayTags()const
{

}

bool j1Console::IsActive()const
{
	return Active;
}

bool j1Console::SaveCVars(pugi::xml_node&)const
{
	bool ret = true;

	return ret;
}

bool j1Console::LoadCVars(pugi::xml_node&)
{
	bool ret = true;

	return ret;
}

void j1Console::CutString(const char* str, p2DynArray<p2SString>* dst)
{
	p2SString strr(str);
	p2SString tmp;

	bool onComas = false;

	for (uint i = 0; i < strr.Length(); i++)
	{
		if (!onComas && strr.GetString()[i] == ' ')
		{
			tmp = strr;
			tmp.Cut(i, 0);
			dst->PushBack(tmp);
			strr.Cut(0, i);
			i = 0;			
		}

		if (strr.GetString()[i] == '"')
		{
			onComas = !onComas;
			if (onComas)
			{
				strr.Cut(0, 0);
				i--;
			}
			else
			{
				strr.Cut(i, i);
				i--;
			}
		}
	}
	dst->PushBack(strr);
}

Command* j1Console::FindCommand(const char* str, uint nArgs)const
{
	p2List_item<Command*>* item;
	Command* ret = NULL;

	for (item = commandList.start; item && !ret; item = item->next)
	{
		if (item->data->CommandName == str)
		{
			ret = item->data;
		}
	}

	if (ret)
	{
		if (nArgs > ret->nArgs)
		{
			LOG("Too many arguments. Command %s must recive %i arguments and you sent %i arguments.", ret->CommandName, ret->nArgs, nArgs);
			ret = NULL;
		}
	}

	return ret;
}

CVar* j1Console::FindCVar(const char* str)
{
	CVar* ret = NULL;
	p2List_item<CVar*>* tmp = CVarList.start;

	while (tmp)
	{
		if (tmp->data->GetName() == str)
		{
			ret = tmp->data;
			break;
		}
		tmp = tmp->next;
	}
	return ret;
}

void j1Console::SetCVar(const char* calue)
{

}



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

void j1Console::C_Quit::Function(const p2DynArray<p2SString>* arg)
{
	App->console->CloseGame = true;
}

void j1Console::C_Close::Function(const p2DynArray<p2SString>* arg)
{
	App->console->Close();
}