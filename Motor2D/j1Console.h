#ifndef __j1CONSOLE_H__
#define __j1CONSOLE_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2DynArray.h"

class GuiInputText;
class GuiLabel;

#define LINE_SPACE 10

enum CVarTypes
{
	cv_float,
	cv_int,
	cv_string,
	cv_bool
};

// ---------------------------------------------------
struct Command
{
public:
	Command(){}
	Command(char* str, char* des, uint args, char* newTag = "Miscellaneous"){ CommandName = str; Description = des; nArgs = args; Tag = newTag; }
	
public:
	p2SString CommandName;
	p2SString Description;
	p2SString Tag;

	uint nArgs;

	virtual void Function(const p2DynArray<p2SString>* args);
};


// ---------------------------------------------------
class CVar
{
public:
	CVar(const char*  name, float* ref, bool serialize);
	CVar(const char*  name, int* ref, bool serialize);
	CVar(const char*  name, char* ref, bool serialize);
	CVar(const char*  name, bool* ref, bool serialize);

public:
	bool Serialize;

private:
	p2SString Name;
	p2SString Description;
	p2SString Tag;
	CVarTypes type;
	Command* command;
	j1Module* listener;

	union
	{
		float* fRef;
		int* iRef;
		char* cRef;
		bool* bRef;
	}Reference;

	union
	{
		float fVal;
		int iVal;
		char* cVal;
		bool bVal;
	}Value;

public:
	void LinkCommand(Command* _command);

	void Set(float val);
	void Set(int val);
	void Set(char* val);
	void Set(bool val);

	void SetListener(j1Module* listener);

	CVarTypes GetType();
	p2SString GetName();
	const j1Module* GetListener()const;

	void Read(void* val, CVarTypes expected);
	bool Read(float* val);
	bool Read(int* val);
	bool Read(char* val);
	bool Read(bool* val);

	void* ForceRead();

	void DisplayLog();
};

// ---------------------------------------------------
class j1Console : public j1Module
{
public:

	j1Console();

	// Destructor
	virtual ~j1Console();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called when UI event is raised
	void OnGui(Gui* ui, GuiEvents event);

	void AddCommand(Command* command);
	uint AddCVar(const char* name, float* reference, j1Module* listener = NULL, bool serialize = false);
	uint AddCVar(const char* name, int* reference, j1Module* listener = NULL, bool serialize = false);
	uint AddCVar(const char* name, char* reference, j1Module* listener = NULL, bool serialize = false);
	uint AddCVar(const char* name, bool* reference, j1Module* listener = NULL, bool serialize = false);

	void GetInput(const char* src);

	void Output(char* str);

	void Open();
	void Close();
	void Clear();

	void DisplayCommands(p2SString str)const;
	void DisplayAllCommands()const;
	void DisplayTags()const;

	bool IsActive()const;

	bool SaveCVars(pugi::xml_node&)const;
	bool LoadCVars(pugi::xml_node&);

private:
	void CutString(const char* str, p2DynArray<p2SString>* dst);
	Command* FindCommand(const char* str, uint nArgs)const;
	CVar* FindCVar(const char* str);
	void SetCVar(const char* value);

private:
	p2List<Command*> commandList;
	p2List<CVar*> CVarList;
	p2DynArray<p2SString> tags;

	//UI
	Gui* BigRectangle;
	Gui* SmallRectangle;
	GuiInputText* InputText = NULL;

	p2DynArray<GuiLabel*> output;

	bool Active = false;
	bool DragText = false; //Not sure to use it right now

	int TextStart = 0;
	int OutputHeight = 0;

	bool CloseGame = false;



	struct C_Quit : public Command
	{
		C_Quit() : Command("quit", "Quit the game.", 0, "Console"){}
		void Function(const p2DynArray<p2SString>* arg);
	}c_Quit;

	struct C_Close : public Command
	{
		C_Close() : Command("close", "Close console.", 0, "Console"){}
		void Function(const p2DynArray<p2SString>* arg);
	}c_Close;


};

#endif // __j1CONSOLE_H__