#ifndef WINDOWSFUNCTIONS_H
#define WINDOWSFUNCTIONS_H

#include <QDebug>
#include <QList>

#include <windows.h>
#include <shlobj.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

typedef const std::wstring &cwstr;
typedef QList<std::wstring> WStringList;

bool readDesktopIniIfExist(cwstr filePath, WStringList &lines);

bool setFolderIcon(cwstr folderPath, cwstr iconPath);

#endif // WINDOWSFUNCTIONS_H
