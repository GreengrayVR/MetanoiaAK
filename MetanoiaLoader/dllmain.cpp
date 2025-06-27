// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#pragma comment(lib, "winmm.lib")

DWORD WINAPI LoaderThread(LPVOID lpParameter)
{
	while (true)
	{
		if (::GetAsyncKeyState(VK_INSERT) & 1)
		{
			HMODULE hModule = ::LoadLibrary(TEXT("MetanoiaAK.dll"));
			if (hModule == NULL)
			{
				MessageBox(0, TEXT("MetanoiaAK.dll not found"), TEXT("Metanoia"), 0);
				continue;
			}

			using func_t = void(*)();
			func_t metanoia_main = (func_t)::GetProcAddress(hModule, "__metanoia_main");

			metanoia_main();

			::FreeLibrary(hModule);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(16)); 
	}

	return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		::CreateThread(0, 0, LoaderThread, 0, 0, 0);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

