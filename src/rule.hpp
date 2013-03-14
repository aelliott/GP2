#ifndef RULE_HPP
#define RULE_HPP

#include "gpfile.hpp"

class Rule : public GPFile
{
    Q_OBJECT

public:
    explicit Rule(const QString &rulePath = QString(), QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();
};

#endif // RULE_HPP
