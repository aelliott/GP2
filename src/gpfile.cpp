#include "gpfile.hpp"

GPFile::GPFile(const QString &filePath, QObject *parent)
    : QObject(parent)
    , _path(filePath)
    , _fp(filePath)
{
}

QString GPFile::path() const
{
    return _path;
}

bool GPFile::open()
{
    _fp.close();
    return _fp.open(QFile::ReadWrite);
}
