#include "gpfile.hpp"

#include <QStringList>
#include <QDebug>
#include <QFileInfo>

namespace Developer {

GPFile::GPFile(const QString &filePath, QObject *parent)
    : QObject(parent)
    , _path(filePath)
    , _fileWatcher(0)
    , _fp(0)
    , _status(GPFile::Modified)
{
    _fileWatcher = new QFileSystemWatcher(this);
    connect(_fileWatcher, SIGNAL(fileChanged(QString)),
            this, SLOT(fileChanged(QString)));

    if(!_path.isEmpty())
        open();
}

GPFile::~GPFile()
{
    // Only delete the file pointer if one has been created, in an unsaved
    // discard this may still be 0
    if(_fp != 0)
        delete _fp;
    delete _fileWatcher;
}

QString GPFile::path() const
{
    return _path;
}

QString GPFile::absolutePath() const
{
    QFileInfo f(_path);
    return f.absoluteFilePath();
}

QString GPFile::fileName() const
{
    if(_path.isEmpty())
        return QString();

    QFileInfo info(_path);
    if(!info.exists())
        return QString();

    return info.fileName();
}

QDir GPFile::dir() const
{
    if(_path.isEmpty())
        return QDir();

    QFile f(_path);
    if(!f.exists())
        return QDir();

    QFileInfo info(f);
    return info.dir();
}

GPFile::FileStatus GPFile::status() const
{
    return _status;
}

bool GPFile::saveAs(const QString &filePath)
{
    // The majority of the work in this function should be in the derived
    // classes, but we need to update the _fileWatcher regardless assuming that
    // the save process has worked without a failure

    // Clear the watcher of any existing path
    if(_fileWatcher->files().count() > 0)
        _fileWatcher->removePaths(_fileWatcher->files());
    _fileWatcher->addPath(filePath);

    // There's no reason for an error in this logic
    return true;
}

bool GPFile::open()
{
    if(_fp != 0)
    {
        _fp->close();
        delete _fp;
    }

    // Clear the watcher of any existing path
    if(_fileWatcher->files().count() > 0)
        _fileWatcher->removePaths(_fileWatcher->files());

    _fp = new QFile(_path);
    // If this is a new file then we stick with GPFile::Modified as a status as
    // it needs to be saved or discarded, otherwise we stick with GPFile::Normal
    // - except in the case where we have been told to open a non-null path and
    // it does not exist, in which case we mark the file as GPFile::Deleted
    if(_fp->exists())
    {
        _status = GPFile::Normal;
        // It exists, so we should watch it
        _fileWatcher->addPath(_path);
    }
    else
    {
        if(_path.isEmpty())
            _status = GPFile::Modified;
        else
            _status = GPFile::Deleted;
    }

    return _fp->open(QFile::ReadWrite);
}

void GPFile::fileChanged(const QString &filePath)
{
    if(filePath != _path)
    {
        qDebug() << "File path mismatch:";
        qDebug() << "_path = " << _path;
        qDebug() << "filePath = " << filePath;
    }

    //NOTE:
    // This might need logic for when a change is made within the application
    // The status should only change for external edits
    _status = GPFile::ExternallyModified;
}

}
