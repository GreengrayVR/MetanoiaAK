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



bool IsReadableAddress(void* addr, size_t size = sizeof(DWORD)) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(addr, &mbi, sizeof(mbi))) {
        DWORD protect = mbi.Protect;
        bool isReadable = (protect & PAGE_READONLY) ||
            (protect & PAGE_READWRITE) ||
            (protect & PAGE_WRITECOPY) ||
            (protect & PAGE_EXECUTE_READ) ||
            (protect & PAGE_EXECUTE_READWRITE) ||
            (protect & PAGE_EXECUTE_WRITECOPY);

        return mbi.State == MEM_COMMIT && isReadable;
    }
    return false;
}

void* SafeRead(void* addr) {
    if (IsReadableAddress(addr))
        return *(void**)addr;
    else
        return nullptr;
}

extern "C"
__declspec(dllexport)
void __metanoia_main()
{
    Console console;

    // 0x1015A30 -> maybe update function
    // CEGUIWindowManager_getWindow_maybe_sub_1013430
    // dword_1E81370 -> CEGUIWindowManager Instance

    //auto CEGUIWindowManager_getWindow = ((int(*)(void*, void*))0x1013430);


#if 0 // get server name and player name
    auto func = foff<void* ()>((char*)0x770650);
    void* stats = func();

    auto get_string_offset = [](void* address, size_t offset) {
        std::string str;
        for (size_t i = 0; ; i++)
        {
            if (auto c = read_safe<char>(poff<char>(address, offset + i)))
            {
                if (c.value() == '\0')
                    break;

                str.push_back(c.value());
            }
            else
            {
                break;
            }
        }
        return str;
    };

    Console::Log("'", get_string_offset(stats, 92), "'");
    Console::Log("'", get_string_offset(stats, 64), "'");
#endif


    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
            break;

        if (GetAsyncKeyState(VK_LSHIFT) & 1)
        {

            //for (size_t i = 0; i < 1000; i++)
            //    if (auto c = read_safe<char>(poff<char>(stats, i)))
            //        if (32 <= c.value() && c.value() <= 127)
            //            std::cout << i << " " << c.value() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
