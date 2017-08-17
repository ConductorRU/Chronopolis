#pragma once
#include <iostream>
#include <fstream>
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
#include <cstdlib>
#include <algorithm>
#include <sstream> 
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

//#define D3D10_IGNORE_SDK_LAYERS
#ifdef _DEBUG
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <windows.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace std;
typedef unsigned __int8 uchar;
typedef unsigned __int16 ushort;
typedef unsigned __int32 uint;
typedef unsigned __int64 ulong;
typedef size_t uxint;
