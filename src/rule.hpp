#ifndef RULE_HPP
#define RULE_HPP

#include "gpfile.hpp"

class Rule : public GPFile
{
    Q_OBJECT
public:
    explicit Rule(const QString &rulePath = QString(), QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // RULE_HPP
