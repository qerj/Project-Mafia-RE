//#include <MinHook.h> gonna use this later like its imperial vs metric system, or PRIMAL GL
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <TlHelp32.h>
#include <vector>
#include "HackHelper.h"

namespace game
{
	//00400000
	uintptr_t baseAddress = (uintptr_t)GetModuleHandleA("game.exe");
}
namespace gun
{
	uintptr_t* firstChain = (uintptr_t*)(game::baseAddress + 0x00247E1C);
	uintptr_t* secondChain = *(uintptr_t**)gun::firstChain;
	uintptr_t* thirdChain = *(uintptr_t**)((char*)secondChain + 0xE4);
	uintptr_t* finalGunClassChain = (uintptr_t*)((char*)thirdChain + 0x4A4);
}

namespace G_Indicator
{
	//gives us the start address of the AddConsoleText function.
	//we should really use the offset of the function
	uintptr_t AddConsoleText = /*(uintptr_t)game::baseAddress +*/ 0x5F9D50;
}

struct GunClass
{
	int curAmmo;//0x0
	int sideAmmo;//0x4
	char pad_0[4];
	int weaponId;//0xC
};



typedef const char* (__thiscall* AddConsoleTextC)(void* this_, const char* msg, int GLColor); //i tinks its opengl?? GLint


DWORD WINAPI HookThread(HMODULE hModule)
{
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	GunClass* weaponPtr = (GunClass*)gun::finalGunClassChain;
	AddConsoleTextC AddConsoleText = (AddConsoleTextC)(G_Indicator::AddConsoleText);
	AddConsoleText((void*)0x006BF980, "Mafia-Project Injected!", 2555555);

	while (!GetAsyncKeyState(VK_DELETE) & 1)
	{
		
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			std::cout << "Current gun ammo before: " << weaponPtr->curAmmo << std::endl;
			std::cout << "Current side ammo before: " << weaponPtr->sideAmmo << std::endl;
			weaponPtr->curAmmo = 1337;
			weaponPtr->sideAmmo = 1337;
			std::cout << "Current gun ammo after: " << weaponPtr->curAmmo << std::endl;
			std::cout << "Current side ammo after: " << weaponPtr->sideAmmo << std::endl;
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
		}



	
	}
	
	FreeConsole();
	fclose(f);
	FreeLibraryAndExitThread(hModule, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ulReasonForCall, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (ulReasonForCall == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)HookThread, hModule, NULL, nullptr));
	if (ulReasonForCall != DLL_PROCESS_ATTACH)
		return TRUE;

	return TRUE;
}
