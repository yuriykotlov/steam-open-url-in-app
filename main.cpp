#include <iostream>

#include <windows.h>
#include <shellapi.h>

#define STEAM_PREFIX "steam://openurl/"

int main(){
    // std::string target{};

    // std::cout << "enter steam url target: ";
    // std::cin >> target;

    //ShellExecuteA(nullptr, "open", (STEAM_PREFIX + target).c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    auto result = ShellExecuteA(nullptr, "open", "powershell -command \"Get-Clipboard\"", nullptr, nullptr, SW_SHOWNORMAL);

    std::cout << result;

    return 0;
}