#include <iostream>
#include <string>

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

bool get_answer(){
    std::string answer{};

    while(answer[0] != 'y' && answer[1] != 'n'){
        std::cout << "\ny/n: ";
        std::cin >> answer;
        std::cout << '\n';
    }
    
    return answer[0] == 'y';
}

std::string get_line(const std::string &copiedText){
    std::string text{};

    std::cout << (!copiedText.empty()
                    ? "you have text on your clipboard: \"" + copiedText + "\""
                    : "your clipboard is empty."
                 )
              << "\n\n";

    while(text.empty()){
        std::cout << "enter steam url target, or leave empty to use clipboard copy.\n => ";
        std::getline(std::cin, text);
        std::cout << '\n';

        if(text.empty()){
            text = get_clipboard_text();
            break;
        }
    }

    return text;
}

int main(){
    std::string target{get_line(get_clipboard_text())};

    std::cout << "directing to \"" + (STEAM_PREFIX + target) + "\" ...";

    //ShellExecuteA(nullptr, "open", (STEAM_PREFIX + target).c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    return 0;
}