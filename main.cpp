#include <iostream>
#include <string>
#include <format>

#include <windows.h>
#include <shellapi.h>
#include <conio.h>

#define STEAM_PREFIX "steam://openurl/"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void enable_ansi_codes() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
}

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

void run_command(const std::wstring &target){
    HINSTANCE result = ShellExecuteW(nullptr, L"open", target.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    // mostly likely will never throw error since STEAM_PREFIX is valid
    if(reinterpret_cast<INT_PTR>(result) <= 32){
        std::cout << "\nERROR: redirect failed.\n";
    }

    std::cout << "\nif steam doesn't open anything, your url was invalid or steam was closed.\n";
}

int main(){
    enable_ansi_codes();

    char option{};
    
    while(std::tolower(option) != 'q'){
        std::cout << "\033[H\033[2J" << std::flush;

        std::string target{STEAM_PREFIX + get_line(get_clipboard_text())};

        std::cout << "directing to \"" + target + "\" ...\n";

        // convert string to wstring
        run_command(
            {target.begin(), target.end()}
        );
        
        std::cout << "[ press Q to close or ENTER for a new url ]\n\n";
        option = std::cin.get();
    }

    return 0;
}