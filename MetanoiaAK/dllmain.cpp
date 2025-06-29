// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Console.hpp"

#include "static_lambda/detour_lambda.hpp"


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

    auto func = foff<void* ()>((char*)0x770650);
    void* stats = func();


    auto replacement = [](auto original, void* _this) -> void*
    {
        Console::Log("yes");
        return original(_this);
    };

    sl::detour<void*(void*)> a(0x6D3530, replacement);


    while (true)
    {

        std::cout << get_string_offset(stats, 64) << " " << get_string_offset(stats, 92) << " " << *(float*)0x1E7B8A8 << " " << *(float*)0x1E7B8B0 << " " << *(float*)0x1E7B8AC << std::endl;

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
