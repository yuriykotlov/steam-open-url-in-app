#include <iostream>
#include <string>

#include <windows.h>
#include <shellapi.h>
#include <conio.h>

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

std::string get_line(const std::string &copiedText){
    std::string text{};

    if(!copiedText.empty()){
        std::cout << "you have text on your clipboard: \"" + copiedText + "\"\n\n"
                  << "enter steam url target, or leave empty to use clipboard copy.\n => ";
    } else{
        std::cout << "your clipboard is empty.\n\n"
                  << "enter steam url target\n => ";
    }

    std::getline(std::cin, text);
    std::cout << '\n';

    if (text.empty()){
        text = copiedText;
    }
    return text;
}

void run_command(const std::string &target){
    HINSTANCE result = ShellExecuteA(nullptr, "open", target.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    // mostly likely will never throw error since STEAM_PREFIX is valid
    if(reinterpret_cast<INT_PTR>(result) <= 32){
        std::cout << "\nERROR: redirect failed.\n";
    }

    std::cout << "\nif steam doesn't open anything, your url was invalid or steam was closed.\n";
    std::cout << "[ press any button to close ]";
    _getch();
}

int main(){
    std::string target{STEAM_PREFIX + get_line(get_clipboard_text())};

    std::cout << "directing to \"" + target + "\" ...\n";

    run_command(target);

    return 0;
}