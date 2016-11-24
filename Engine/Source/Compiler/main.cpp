#include "stdafx.h"
#include "Class.h"
#include "Script.h"
#include "Delegate.h"

struct Int
{
	int i = 0;
	void Add(int x, int y )
	{
		i = x + y;
	}
	int Get()
	{
		return i;
	}
};

void Vr()
{
	cout << "vr" << endl;
}

int main()
{
	//void *v = Int::Add;
	int x = 6;
	Int id;
	Delegate d;
	d.Connect<Int, int, int>(&Int::Add);
	d.Call(&id, x, 6);
	//d.Connect<Int, int(__thiscall Int::*)()>(&Int::Get);
	cout << x << endl;
	//d.Connect(&Vr);
	//d.Call();
	Script sc;
	sc.Run();
	cout << id.i;
	string code = "int i = 5, v = 6 + 2; i = 8;";
	Compiler comp;
	comp.Init();
	comp.Compile(code);
	int i;
	cin >> i;
	return 0;
}