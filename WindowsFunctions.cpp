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

    SetFileAttributes(desktopIniPath.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
    SetFileAttributes(folderPath.c_str(), FILE_ATTRIBUTE_SYSTEM);

    // 2. Wymuś aktualizację ustawień folderu przez API // this made it acceptable dynamic
    SHFOLDERCUSTOMSETTINGS fcs = {0};
    fcs.dwSize = sizeof(fcs);
    fcs.dwMask = FCSM_ICONFILE;
    fcs.pszIconFile = (wchar_t*)iconPath.c_str();
    fcs.cchIconFile = 0;
    fcs.iIconIndex = 0;

    SHGetSetFolderCustomSettings(&fcs, folderPath.c_str(), FCS_FORCEWRITE);

    // 3. Wymuś odświeżenie całego systemu // this makes it "right at the time"
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST | SHCNF_FLUSH, NULL, NULL);
    SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH | SHCNF_FLUSHNOWAIT, folderPath.c_str(), NULL);

    // 4. Dodatkowo: Odśwież widok folderu (szczególnie przydatne w Bibliotekach)
    SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH, folderPath.c_str(), NULL);

    /// wtf i didn't noticed that, and I lost some stuff XD
    /// but deepseek is much better, going to complete task at all cost, and that was unexpected
    /// good thing that didn't restart my PC XDD
    // // 5. Opcjonalnie: Zrestartuj Eksplorator Windows (wymusza natychmiastową aktualizację)
    // system("taskkill /f /im explorer.exe");
    // system("start explorer.exe");
}
