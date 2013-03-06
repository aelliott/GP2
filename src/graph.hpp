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
};

#endif // GRAPH_HPP
