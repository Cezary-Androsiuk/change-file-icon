#include "WindowsFunctions.h"

void setFolderIcon(cwstr folderPath, cwstr iconPath)
{
    // Utwórz lub edytuj plik desktop.ini
    std::wstring desktopIniPath = folderPath + L"\\desktop.ini";
    std::wofstream desktopIni(desktopIniPath.c_str()); // Teraz kompilator rozpozna std::wofstream
    if (desktopIni.is_open()) {
        desktopIni << L"[.ShellClassInfo]\n";
        desktopIni << L"IconResource=" << iconPath << L",0\n";
        desktopIni << L"[ViewState]\n";
        desktopIni << L"Mode=\n";
        desktopIni << L"Vid=\n";
        desktopIni << L"FolderType=Generic\n";

        desktopIni.close();
    }

    // Ustaw atrybuty pliku na ukryty i systemowy
    SetFileAttributes(desktopIniPath.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);

    // Ustaw atrybut folderu na systemowy
    SetFileAttributes(folderPath.c_str(), FILE_ATTRIBUTE_SYSTEM);

    // Powiadom system o zmianie
    SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH, folderPath.c_str(), NULL);
}
