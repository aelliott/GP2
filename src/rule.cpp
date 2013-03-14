#include "rule.hpp"

Rule::Rule(const QString &rulePath, QObject *parent)
    : GPFile(rulePath, parent)
{
}

bool Rule::save()
{
    return true;
}

bool Rule::saveAs(const QString &filePath)
{
    return GPFile::saveAs(filePath);
}

bool Rule::open()
{
    if(!GPFile::open())
        return false;

    return true;
}
