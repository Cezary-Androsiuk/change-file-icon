#include "WindowsFunctions.h"

bool readDesktopIniIfExist(cwstr filePath, WStringList &lines)
{
    if(!std::filesystem::exists(filePath))
        return false;

    std::wifstream file(filePath.c_str());
    if(!file.is_open()){
        qDebug() << "unable to read file"<<filePath;
        return false;
    }

    std::wstring line;
    while(std::getline(file, line))
        lines.append(line);

    file.close();

    return true;
}

int findString(const WStringList &list, cwstr value)
{
    for(int i=0; i<list.size(); i++)
    {
        if(list[i] == value)
            return i;
    }
    return -1;
}

int findStartWithString(const WStringList &list, cwstr value)
{
    QString valueConv = QString::fromStdWString(value);
    for(int i=0; i<list.size(); i++)
    {
        QString lineValue = QString::fromStdWString(list[i]);
        if(lineValue.startsWith(valueConv))
            return i;
    }
    return -1;
}

bool setFolderIcon(cwstr folderPath, cwstr iconPath)
{
    const std::wstring desktopIniPath = folderPath + L"\\desktop.ini";

    WStringList previousDesktopIniFileLines;

    if(readDesktopIniIfExist(desktopIniPath, previousDesktopIniFileLines))
    {
        /// desktop.ini already exist
        /// override icon data or add them
        qDebug() << "desktop ini exist";

        if(!std::filesystem::remove(desktopIniPath))
        {
            qDebug() << "deleting file failed"<<desktopIniPath;
            return false;
        }

        std::wofstream desktopIni(desktopIniPath.c_str()); /// overrides existing file
        if (!desktopIni.is_open()) {
            qDebug() << "unable to write desktop.ini file";
            return false;
        }

        /// if [.ShellClassInfo] not exist
        /// IconResource also shouldn't

        int firstLineIndex = findString(previousDesktopIniFileLines, L"[.ShellClassInfo]");
        if(firstLineIndex == -1)
        {
            /// add two lines at very end
            qDebug() << "desktop ini NOT contains '[.ShellClassInfo]'";

            for(const auto &line : previousDesktopIniFileLines)
                desktopIni << line << L"\n";

            desktopIni << L"[.ShellClassInfo]\n";
            desktopIni << L"IconResource=" << iconPath << L",0\n";
        }
        else
        {
            qDebug() << "desktop ini contains '[.ShellClassInfo]' at" << firstLineIndex;
            int secondLineIndex = findStartWithString(previousDesktopIniFileLines, L"IconResource=");

            if(secondLineIndex == -1)
            {
                qDebug() << "desktop ini NOT contains line that starts with 'IconResource='";
                /// add second line behind first line
                for(int i=0; i<previousDesktopIniFileLines.size(); i++)
                {
                    desktopIni << previousDesktopIniFileLines[i] << L"\n";
                    if(i == firstLineIndex)
                        desktopIni << L"IconResource=" << iconPath << L",0\n";
                }
            }
            else
            {
                qDebug() << "desktop ini contains line that starts with 'IconResource=' at" << secondLineIndex;
                /// replace second line
                previousDesktopIniFileLines[secondLineIndex] = L"IconResource=" + iconPath + L",0\n";
                for(const auto &line : previousDesktopIniFileLines)
                    desktopIni << line << L"\n";
            }
        }
        qDebug() << "saved file, closing...";
        desktopIni.close();
    }
    else
    {
        /// desktop.ini not exist yet
        qDebug() << "desktop ini NOT exist";

        std::wofstream desktopIni(desktopIniPath.c_str());
        if (!desktopIni.is_open()) {
            qDebug() << "unable to write desktop.ini file";
            return false;
        }

        desktopIni << L"[.ShellClassInfo]\n";
        desktopIni << L"IconResource=" << iconPath << L",0\n";

        qDebug() << "saved file, closing...";
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
    return true;
}
