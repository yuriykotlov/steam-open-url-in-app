#include <iostream>

#include <windows.h>
#include <shellapi.h>

#define STEAM_PREFIX "steam://openurl/"

std::string get_clipboard_text(){
    if(!OpenClipboard(nullptr)){
        CloseClipboard();
        return {};
    };

    HANDLE data = GetClipboardData(CF_TEXT);
    if(data == nullptr){
        CloseClipboard();
        return {};
    }

    const char *pszText = static_cast<char*>(GlobalLock(data));
    if(pszText == nullptr){
        CloseClipboard();
        return {};
    }

    // [personal note]
    // originally tried to use string_view, but it is a pointer to the string
    // supplied, so after unlocking the data/closing the clipboard the pointer
    // is not guaranteed to be the same value or exist at all, so we need to
    // copy the value instead
    std::string copiedText{pszText};

    GlobalUnlock(data);
    CloseClipboard();
    
    return copiedText;
}

int main(){
    // std::string target{};

    // std::cout << "enter steam url target: ";
    // std::cin >> target;

    //ShellExecuteA(nullptr, "open", (STEAM_PREFIX + target).c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    // auto result = ShellExecuteA(
    //     nullptr,
    //     "open",
    //     "powershell",
    //     "-command \"Get-Clipboard\"",
    //     nullptr,
    //     SW_SHOWNORMAL
    // );

    // std::cout << (INT_PTR)result;

    std::string copiedText{get_clipboard_text()};

    std::cout << copiedText;

    return 0;
}