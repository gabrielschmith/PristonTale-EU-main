#pragma once

#define D3D_DEBUG_INFO

//Libraries
//Windows API
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Imagehlp.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"rpcrt4.lib")
#pragma comment(lib,"Shlwapi.lib")

#pragma comment(lib,"Vfw32.lib")

//Network
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wininet.lib")

//Lua
#pragma comment(lib, "lua.lib")

//Directx
#pragma comment(lib,"d3d9.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx9d.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#pragma comment(lib,"dsound.lib")


#pragma warning(disable: 4101)


//Includes
//Standard:
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "targetver.h"

//STD:
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iomanip>
#include <algorithm>

//Windows Header Files:
#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <Mmsystem.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <vfw.h>
#include <intrin.h>
#include <aclapi.h>

#include <Wininet.h>
#include <WinSock2.h>

#include <memory>
#include <stack>
#include <time.h>
#include <Strsafe.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3d9types.h>
#include <dsound.h>

#include <sstream>
#include <fstream>

#include <cctype>
#include <functional>
#include <queue>
#include <Imagehlp.h>
#include <basetsd.h>
#include <rpc.h>

//COM
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

//User Header Files:
extern "C"
{
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
#include "Lua/lstate.h"
}

#include <dshow.h>
#include "qedit.h"
#pragma comment(lib,"Strmiids.lib")

//Custom Include:
#include "globals.h"

DWORD GetMajorAllocator();
DWORD GetMajorAllocatorP();

void * operator new[]( std::size_t s ) throw(std::bad_alloc);
void operator delete[]( void *p ) throw();
void * operator new(std::size_t s) throw(std::bad_alloc);
void operator delete(void *p) throw();

using namespace std;