#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
    , m_isFileSelected{false}
    , m_isIconSelected{false}
{}

void Backend::setIsFileSelected(bool isFileSelected)
{
    // if(m_isFileSelected == isFileSelected)
    //     return;

    m_isFileSelected = isFileSelected;
    emit this->isFileSelectedChanged();
}

bool Backend::getIsFileSelected() const
{
    return m_isFileSelected;
}

void Backend::setIsIconSelected(bool isIconSelected)
{
    // if(m_isIconSelected == isIconSelected)
    //     return;

    m_isIconSelected = isIconSelected;
    emit this->isIconSelectedChanged();
}

bool Backend::getIsIconSelected() const
{
    return m_isIconSelected;
}

const QString &Backend::getSelectedFile() const
{
    return m_selectedFile;
}

const QString &Backend::getSelectedIcon() const
{
    return m_selectedIcon;
}

void Backend::setSelectedFile(QUrl selectedFile)
{
    // qDebug() << "setSelectedFile" << selectedFile;

    m_selectedFile.clear();
    this->setIsFileSelected(false);

    if(!this->fileExist(selectedFile))
    {
        qDebug() << "file" << selectedFile << "not exist!";
        emit this->selectedFileChanged();
        return;
    }

    if(!this->fileIsDir(selectedFile))
    {
        qDebug() << "file" << selectedFile << "is not a directory!";
        emit this->selectedFileChanged();
        return;
    }

    m_selectedFile = selectedFile.toLocalFile();
    this->setIsFileSelected(true);
    emit this->selectedFileChanged();
}

void Backend::setSelectedIcon(QUrl selectedIcon)
{
    // qDebug() << "setSelectedIcon" << selectedIcon;

    m_selectedIcon.clear();
    this->setIsIconSelected(false);

    if(!this->fileExist(selectedIcon))
    {
        qDebug() << "icon" << selectedIcon << "not exist!";
        emit this->selectedIconChanged();
        return;
    }

    if(!ImageHandler::iconIsValidFormat(selectedIcon.toLocalFile()))
    {
        qDebug() << "icon" << selectedIcon << "is not valid format!";
        emit this->selectedIconChanged();
        return;
    }

    m_selectedIcon = selectedIcon.toLocalFile();
    this->setIsIconSelected(true);
    emit this->selectedIconChanged();
}

void Backend::setIconForFile()
{
    bool condition1 = !m_isFileSelected || !m_isIconSelected;
    bool condition2 = m_selectedFile.isEmpty() || m_selectedIcon.isEmpty();
    if(condition1 || condition2)
    {
        qDebug() << "file or icon is not selected";
        return;
    }

    QString validIconPath;
    if(!ImageHandler::handleFile(m_selectedIcon, validIconPath))
    {
        qDebug() << "cannot handle image file";
        return;
    }

    bool iconChangeCompleted = setFolderIcon(m_selectedFile.toStdWString(), validIconPath.toStdWString());
    if(!iconChangeCompleted)
    {
        qDebug() << "changing folder icon failed";
        return;
    }

    qDebug() << "icon changed successfully";

    this->setIsFileSelected(false);
    this->setIsIconSelected(false);
    m_selectedFile.clear();
    m_selectedIcon.clear();
}

bool Backend::fileExist(const QUrl &url) const
{
    QString filePath = url.toLocalFile();
    qDebug() << filePath;
    QFileInfo fileInfo(filePath);
    qDebug() << fileInfo;
    return fileInfo.exists();
}

bool Backend::fileIsDir(const QUrl &url) const
{
    QString filePath = url.toLocalFile();
    qDebug() << filePath;
    QFileInfo fileInfo(filePath);
    qDebug() << fileInfo;
    return fileInfo.isDir(); // fileInfo.isFile()
}






















