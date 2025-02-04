#include "ImageHandler.h"

ImageHandler::ImageHandler(QObject *parent)
    : QObject{parent}
{}

bool ImageHandler::handleFile(const QString &filePath, QString &resultPath)
{
    /// NOTE:
    /// Files are stored under name that represents their data hash

    QByteArray byteHash;
    if(!ImageHandler::calculateFileHash(filePath, byteHash))
    {
        qDebug() << "creating hash for "<<filePath<<" failed!";
        return false;
    }

    QString hexHash = byteHash.toHex();

    /// ensure that directory exist
    if(!QDir().mkpath(ICONS_DIRECTORY))
    {
        qDebug() << "unable to create path for icons directory";
        return false;
    }

    QDir tmpResultPath(ICONS_DIRECTORY + "/" + hexHash + ".ico");
    qDebug() << "result ico path:" << tmpResultPath.path();
    qDebug() << "result ico abs path:" << tmpResultPath.absolutePath();

    if(QFile::exists(tmpResultPath.path()))
    {
        qDebug() << "icon already exist in in icons directory";
        resultPath = tmpResultPath.absolutePath();
        return true;
    }

    /// if ICO then copy to icons directoy
    if(ImageHandler::iconIsIco(filePath))
    {
        if(!QFile::copy(filePath, tmpResultPath.path()))
        {
            qDebug() << "copying image to icons directory failed";
            return false;
        }
    }
    /// otherwise convert and save at icons directory
    else
    {
        if(!convertImageToIcon(filePath, tmpResultPath.path()))
        {
            qDebug() << "convering image to icon failed";
            return false;
        }
    }

    resultPath = tmpResultPath.absolutePath();
    return true;
}

bool ImageHandler::calculateFileHash(const QString &filePath, QByteArray &result)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "unable to read file"<<filePath;
        return false;
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    if(hash.addData(&file)){
        result = hash.result();
        return true;
    }

    qDebug() << "reading data failed"<< filePath;
    return false;
}

bool ImageHandler::convertImageToIcon(const QString &imagePath, const QString &iconPath)
{
    QImage sourceImage;
    if(!ImageHandler::loadImage(imagePath, sourceImage))
    {
        qDebug() << "unable to load image";
        return false;
    }

    if(!sourceImage.save(iconPath))
    {
        qDebug() << "saving ico file failed";
        return false;
    }
    return true;
}

bool ImageHandler::iconIsValidFormat(const QString &filePath)
{
    QList<QByteArray> supportedFormats = QImageReader::supportedImageFormats();
    // for(const auto &i : supportedFormats)
    //     qDebug() << i;
    QFileInfo fileInfo(filePath);
    QString fileSuffix = fileInfo.suffix().toLower();
    return supportedFormats.contains(fileSuffix.toUtf8());
}

bool ImageHandler::iconIsIco(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    return fileInfo.suffix().toLower() == "ico";
}

bool ImageHandler::loadImage(const QString &imagePath, QImage &image)
{
    QImageReader reader(imagePath);
    if(!reader.canRead()){
        qDebug() << "cannot load image" << imagePath;
        return false;
    }
    image = reader.read();
    return true;
}
