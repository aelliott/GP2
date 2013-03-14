#include "graph.hpp"

namespace Developer {

Graph::Graph(const QString &graphPath, QObject *parent)
    : GPFile(graphPath, parent)
{
}

bool Graph::save()
{
    return true;
}

bool Graph::saveAs(const QString &filePath)
{
    return GPFile::saveAs(filePath);
}

bool Graph::open()
{
    if(!GPFile::open())
        return false;

    return true;
}

}
