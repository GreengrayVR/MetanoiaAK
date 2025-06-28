#pragma once

class Console
{
public:
    Console();
    ~Console();

    template <typename... TArgs>
    static void Log(TArgs &&...Args)
    {
        using Expand__ = int[];
        (void)Expand__ {
            0, (void(std::cout << Args), 0)...
        };
        std::cout << "\n";
    }
};
