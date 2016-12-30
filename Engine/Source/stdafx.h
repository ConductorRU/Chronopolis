#pragma once
#include <iostream>
#include <clocale>
#include <ctime>
#include <stdio.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <typeinfo.h>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <functional>
#include <assert.h>
#include <crtdbg.h>
#include <thread>

#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <windows.h>

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace std;
typedef unsigned __int32 uint;
