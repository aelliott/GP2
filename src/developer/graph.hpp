/*!
 * \file
 */
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "gpfile.hpp"
#include "global.hpp"

// This includes "node.hpp" by proxy
#include "edge.hpp"
#include <vector>

namespace Developer {

class Graph : public GPFile
{
    Q_OBJECT

public:
    Graph(const QString &graphPath = QString(), QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();

    QString toString(int outputType = DefaultGraph) const;
    QString toGxl() const;
    QString toDot() const;
    QString toAlternative() const;

public slots:
    bool addNode(const QString &label = QString());
    bool addEdge(Node *from, Node *to, const QString &label = QString());

private:
    std::vector<Node> _nodes;
    std::vector<Edge> _edges;
};

}

#endif // GRAPH_HPP
