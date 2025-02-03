#ifndef WINDOWSFUNCTIONS_H
#define WINDOWSFUNCTIONS_H

#include <windows.h>
#include <shlobj.h>
#include <string>

typedef const std::wstring &cwstr;

void setFolderIcon(cwstr folderPath, cwstr iconPath);

#endif // WINDOWSFUNCTIONS_H
