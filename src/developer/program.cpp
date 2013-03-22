/*!
 * \file
 */
#include "program.hpp"

#include <QDebug>

namespace Developer {

Program::Program(const QString &programPath, QObject *parent)
    : GPFile(programPath, parent)
{
    if(!programPath.isEmpty())
        open();
}

QString Program::name() const
{
    return _name;
}

void Program::setName(const QString &programName)
{
    _name = programName;
}

QString Program::program() const
{
    return _program;
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

    qDebug() << "Opening program file: " << absolutePath();

    setName(fileName());
    _program = _fp->readAll();

    qDebug() << "    Finished parsing program file: " << absolutePath();

    return true;
}

}
