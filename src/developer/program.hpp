#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "gpfile.hpp"

namespace Developer {

class Program : public GPFile
{
    Q_OBJECT

public:
    explicit Program(const QString &programPath = QString(), QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();
};

}

#endif // PROGRAM_HPP
