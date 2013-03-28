/*!
 * \file
 */
#include "graph.hpp"
#include "graphparser.hpp"

#include <QSettings>
#include <QDomDocument>

namespace Developer {

Graph::Graph(const QString &graphPath, QObject *parent)
    : GPFile(graphPath, parent)
    , _idCounter(1)
{
    if(!graphPath.isEmpty())
        open();
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

    qDebug() << "Opening graph file: " << _path;

    QString contents = _fp->readAll();
    std::string contentsString = contents.toStdString();
    graph_t graph;

    // With graphs we don't mind if the file is completely new, just accept it
    if(contents.isEmpty())
    {
        graph.canvasX = 0;
        graph.canvasY = 0;
    }
    else
    {
        GraphTypes type = DEFAULT_GRAPH_FORMAT;
        if(_path.endsWith(GP_GRAPH_ALTERNATIVE_EXTENSION))
            type = AlternativeGraph;
        if(_path.endsWith(GP_GRAPH_DOT_EXTENSION))
            type = DotGraph;
        if(_path.endsWith(GP_GRAPH_GXL_EXTENSION))
            type = GxlGraph;

        switch(type)
        {
        case AlternativeGraph:
            graph = parseAlternativeGraph(contentsString);
            break;
        case GxlGraph:
            graph = parseGxlGraph(contents);
            break;
        case DotGraph:
        default:
            graph = parseDotGraph(contentsString);
            break;
        }
    }

    qDebug() << "    Finished parsing graph file: " << _path;

    return true;
}

Node *Graph::node(const QString &id) const
{
    for(nodeConstIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        Node *n = *iter;
        if(n->id() == id)
            return n;
    }

    return 0;
}

Edge *Graph::edge(const QString &id) const
{
    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        Edge *e = *iter;
        if(e->id() == id)
            return e;
    }

    return 0;
}

bool Graph::contains(const QString &id) const
{
    return (containsNode(id) || containsEdge(id));
}

bool Graph::containsNode(const QString &id) const
{
    return (node(id) != 0);
}

bool Graph::containsEdge(const QString &id) const
{
    return (edge(id) != 0);
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

Edge *Graph::addEdge(Node *from, Node *to, const QString &label)
{
    // Is there already a node or edge with this label?

    // Are the two nodes provided real nodes tracked by this graph

    Edge *e = new Edge(newId(), from, to, label);
    _edges.push_back(e);
    return e;
}

Node *Graph::addNode(const QString &label)
{
    // Is there already a node or edge with this label?

    Node *n = new Node(newId(), label);
    if(_nodes.size() == 0)
        n->setIsRoot(true);
    _nodes.push_back(n);
    return n;
}

QString Graph::newId()
{
    // Just find the first free integer, return as a string as the grammar
    // allows for string identifiers
    while(contains(QVariant(_idCounter).toString()))
        ++_idCounter;

    return QVariant(_idCounter).toString();
}

}
