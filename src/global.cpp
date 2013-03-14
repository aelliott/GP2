#include "global.hpp"

#include <QString>
#include <QDebug>

const QString GPVersionToString(GPVersions version)
{
    switch(version)
    {
        case GP1:
            return QString("gp1");
        case GP2:
            return QString("gp2");
        case RootedGP2:
            return QString("rootedgp2");
        default:
            qDebug() << "Unknown version passed: " << version;
            return QString("");
    }
}

GPVersions stringToGPVersion(const QString &version)
{
    if(version == "gp1")
        return GP1;

    if(version == "gp2")
        return GP2;

    if(version == "rootedgp2")
        return RootedGP2;

    qDebug() << "Unknown version string passed: " << version;
    qDebug() << "Defaulting to Rooted GP2";
    return RootedGP2;
}
