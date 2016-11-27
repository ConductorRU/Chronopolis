#include "DC.h"
#include "Game/Game.h"
#pragma comment( lib, "Engine.lib" )
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine, int nCommandShow)
{
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);

	Game game;
	game.Init();
	game.Update();

	_CrtMemDumpAllObjectsSince(&_ms);
	return 0;
}