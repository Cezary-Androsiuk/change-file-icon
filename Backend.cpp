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

void Backend::setSelectedFile(QUrl selectedFile)
{
    // qDebug() << "setSelectedFile" << selectedFile;

    m_selectedFile.clear();
    this->setIsFileSelected(false);

    if(!this->fileExist(selectedFile))
    {
        qDebug() << "file" << selectedFile << "not exist!";
        return;
    }

    if(!this->fileIsDir(selectedFile))
    {
        qDebug() << "file" << selectedFile << "is not directory!";
        return;
    }

    m_selectedFile = selectedFile;

    this->setIsFileSelected(true);
}

void Backend::setSelectedIcon(QUrl selectedIcon)
{
    // qDebug() << "setSelectedIcon" << selectedIcon;

    m_selectedIcon.clear();
    this->setIsIconSelected(false);

    if(!this->fileExist(selectedIcon))
    {
        qDebug() << "icon" << selectedIcon << "not exist!";
        return;
    }

    if(!this->iconIsValid(selectedIcon))
    {
        qDebug() << "icon" << selectedIcon << "is not valid!";
        return;
    }

    m_selectedIcon = selectedIcon;

    this->setIsIconSelected(true);
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

bool Backend::iconIsValid(const QUrl &url) const
{
    QString filePath = url.toLocalFile();
    QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();
    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix().toLower();
    return supportedFormats.contains(fileSuffix.toUtf8());
}
