#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "gpfile.hpp"

// This includes "node.hpp" by proxy
#include "edge.hpp"

class Graph : public GPFile
{
    Q_OBJECT

public:
    Graph(const QString &graphPath = QString(), QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();
};

#endif // GRAPH_HPP
