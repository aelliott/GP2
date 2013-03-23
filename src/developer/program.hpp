/*!
 * \file
 */
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "gpfile.hpp"

namespace Developer {

class Program : public GPFile
{
    Q_OBJECT

public:
    explicit Program(const QString &programPath = QString(), QObject *parent = 0);

    QString name() const;
    void setName(const QString &programName);
    QString program() const;
    void setProgram(const QString &programText);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();

private:
    QString _name;
    QString _program;
};

}

#endif // PROGRAM_HPP
