#include "DC.h"
#include "Toolset/Class.h"
#include "Game/Game.h"
#pragma comment( lib, "Engine.lib" )
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine, int nCommandShow)
{
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);

	Game game;
	game.GetEngine()->CreateConsole();

	string code = "int i = 5, v = 6 + 2; i = 8;";
	game.Init2();
	game.Update();
	_CrtMemDumpAllObjectsSince(&_ms);
	return 0;
}