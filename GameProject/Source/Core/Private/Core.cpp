#include <iostream>

class CoreModule {
public:
    const char* GetName()  { return "Core"; }
    void Startup()  { std::cout << "Core started\n"; }
    void Shutdown()  { std::cout << "Core shutdown\n"; }
};