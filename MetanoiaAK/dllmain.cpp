// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

class Console
{
public:
    Console()
    {
        if (!AllocConsole())
        {
            MessageBox(NULL, TEXT("Failed to create console"), TEXT("Metanoia"), MB_OK);
            return;
        }

        SetConsoleOutputCP(65001);
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        SetConsoleTitle(TEXT("Metanoia"));

        Log("Console started");
    }

    template <typename... TArgs>
    static void Log(TArgs &&...Args)
    {
        using Expand__ = int[];
        (void)Expand__ {
            0, (void(std::cout << Args), 0)...
        };
        std::cout << "\n";
    }

    ~Console()
    {
        HWND hWnd = GetConsoleWindow();
        if (hWnd == NULL)
        {
            MessageBox(NULL, TEXT("Failed to get console window"), TEXT("Metanoia"), MB_OK);
            return;
        }

        if (!ShowWindow(hWnd, 0))
        {
            MessageBox(NULL, TEXT("Failed to show console window"), TEXT("Metanoia"), MB_OK);
            return;
        }

        if (!FreeConsole())
        {
            MessageBox(NULL, TEXT("Failed to close console"), TEXT("Metanoia"), MB_OK);
            return;
        }
    }
};


extern "C"
__declspec(dllexport)
void __metanoia_main()
{
    Console console;

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
            break;

        if (GetAsyncKeyState(VK_LSHIFT) & 1)
            Console::Log("hello ", 1, " ", 2.34f, " ", 5.678);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
