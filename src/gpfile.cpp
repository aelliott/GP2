#include "gpfile.hpp"

GPFile::GPFile(const QString &filePath, QObject *parent)
    : QObject(parent)
    , _path(filePath)
    , _fp(0)
    , _status(GPFile::Modified)
{
}

QString GPFile::path() const
{
    return _path;
}

GPFile::FileStatus GPFile::status() const
{
    return _status;
}

bool GPFile::open()
{
    _fp->close();
    return _fp->open(QFile::ReadWrite);
}
