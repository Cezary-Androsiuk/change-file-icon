#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QDebug>

#include <QDir>
#include <QFile>
#include <QImage>
#include <QFileInfo>
#include <QImageReader>
#include <QCryptographicHash>

#define ICONS_DIRECTORY QString("./icons")

class ImageHandler : public QObject
{
    Q_OBJECT
public:
    explicit ImageHandler(QObject *parent = nullptr);

    static bool handleFile(const QString &filePath, QString &resultPath);

private:
    static bool calculateFileHash(const QString &filePath, QByteArray &result);

    static bool convertImageToIcon(const QString &imagePath, const QString &iconPath);

public:
    static bool iconIsValidFormat(const QString &filePath);
    static bool iconIsIco(const QString &filePath);

private:
    static bool loadImage(const QString &imagePath, QImage &image);
};

#endif // IMAGEHANDLER_H
