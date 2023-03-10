//#include <MinHook.h>
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





//yields zero so idk
			//randomFunc ranFunc = reinterpret_cast<randomFunc>(0x006036D0);
			//ranFunc(0x006C099C, 0, 1, 40, "syskill");
//if (GetAsyncKeyState(VK_F5) & 1)
	//{ //it seems as if removing the extra pointer in the LPVOID, It fixed the hook error problem for some reason the hook still works the same without the error message
		//if (MH_CreateHook((LPVOID)0x2143EA81390, &AddOxygenHook, (LPVOID*)fnAddOxygen) != MH_OK) MessageBoxA(0, "Create Hook Error", 0, 0);
		//if (MH_EnableHook((LPVOID)0x2143EA81390) != MH_OK) MessageBoxA(0, "EnableHook Error", 0,0);
	//}
	// 
//float(__thiscall* fnAddOxygen)(float, float);
//
//float __stdcall AddOxygenHook(float degreeAngle, float ratio)
//{
//	degreeAngle = 6.0f;
//	ratio = 420.0f;
//	return 2.0f * atan(tan(0.5f * degreeAngle * 0.017453292f) * ratio) * 57.29578f;
//}
//fnAddOxygen HookName = fnAddOxygen(moduleBase + 0x00121C2C); //makes this to be a function to hook

//random function hook 6036D0
