/*!
 * \file
 */
#include "graph.hpp"

#include <QSettings>
#include <QDomDocument>

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

QString Graph::toString(int outputType) const
{
    QSettings settings;

    if(outputType == DefaultGraph)
        outputType = settings.value(
                    "Graphs/DefaultGraphFormat",
                    DEFAULT_GRAPH_FORMAT
                    ).toInt();

    switch(outputType)
    {
    case GxlGraph:
        return toGxl();
        break;
    case AlternativeGraph:
        return toAlternative();
        break;
    case DotGraph:
    case DefaultGraph:
    default:
        return toDot();
        break;
    }
}

QString Graph::toGxl() const
{
    QDomDocument doc("gxl");
    QDomElement root = doc.createElement("graph");
    doc.appendChild(root);

    return doc.toString();
}

QString Graph::toDot() const
{
    return "";
}

QString Graph::toAlternative() const
{
    return "";
}

bool Graph::addEdge(Node *from, Node *to, const QString &label)
{
    // Is there already a node or edge with this label?

    // Are the two nodes provided real nodes tracked by this graph

    Edge e(from, to, label);
    _edges.push_back(e);
    return true;
}

bool Graph::addNode(const QString &label)
{
    // Is there already a node or edge with this label?

    Node n(label);
    _nodes.push_back(n);
    return true;
}

}
