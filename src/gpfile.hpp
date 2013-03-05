#ifndef GPFILE_HPP
#define GPFILE_HPP

#include <QObject>
#include <QString>
#include <QFile>

class GPFile : public QObject
{
    Q_OBJECT
public:
    GPFile(const QString &filePath = QString(), QObject *parent = 0);

    QString path() const;

    virtual bool save() = 0;
    virtual bool saveAs(const QString &filePath) = 0;

protected:
    bool open();

    /*!
     * This is the path to the project file, the project directory is the
     * directory which contains this file and is therefore easily derived from
     * this variable.
     */
    QString _path;
    QFile _fp;
};

#endif // GPFILE_HPP
