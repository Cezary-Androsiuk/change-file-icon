#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QUrl>

#include <QFileInfo>
#include <QImageReader>

#include "WindowsFunctions.h"
#include "ImageHandler.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isFileSelected READ getIsFileSelected NOTIFY isFileSelectedChanged FINAL)
    Q_PROPERTY(bool isIconSelected READ getIsIconSelected NOTIFY isIconSelectedChanged FINAL)
    Q_PROPERTY(QString selectedFile READ getSelectedFile NOTIFY selectedFileChanged FINAL)
    Q_PROPERTY(QString selectedIcon READ getSelectedIcon NOTIFY selectedIconChanged FINAL)

public:
    explicit Backend(QObject *parent = nullptr);

    void setIsFileSelected(bool isFileSelected);
    bool getIsFileSelected() const;

    void setIsIconSelected(bool isIconSelected);
    bool getIsIconSelected() const;

    const QString &getSelectedFile() const;
    const QString &getSelectedIcon() const;

public slots:
    void setSelectedFile(QUrl selectedFile);
    void setSelectedIcon(QUrl selectedIcon);

    void setIconForFile();

private:
    bool fileExist(const QUrl &url) const;
    bool fileIsDir(const QUrl &url) const;

signals:
    void isFileSelectedChanged();
    void isIconSelectedChanged();
    void selectedFileChanged();
    void selectedIconChanged();

private:
    bool m_isFileSelected;
    QString m_selectedFile;
    bool m_isIconSelected;
    QString m_selectedIcon;
};

#endif // BACKEND_H
