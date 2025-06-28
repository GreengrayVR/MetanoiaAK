#include "pch.h"
#include "Console.hpp"

Console::Console()
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

Console::~Console()
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
