#include "program.hpp"

namespace Developer {

Program::Program(const QString &programPath, QObject *parent)
    : GPFile(programPath, parent)
{
}

QString Program::name() const
{
    return _name;
}

void Program::setName(const QString &programName)
{
    _name = programName;
}

bool Program::save()
{
    return true;
}

bool Program::saveAs(const QString &filePath)
{
    return GPFile::saveAs(filePath);
}

bool Program::open()
{
    if(!GPFile::open())
        return false;

    return true;
}

}
