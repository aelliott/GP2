#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "gpfile.hpp"

class Program : public GPFile
{
    Q_OBJECT
public:
    explicit Program(const QString &programPath = QString(), QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // PROGRAM_HPP
