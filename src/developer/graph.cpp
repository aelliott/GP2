/*!
 * \file
 */
#include "graph.hpp"
#include "graphparser.hpp"

#include <QSettings>
#include <QDomDocument>
#include <QFileDialog>
#include <QMessageBox>

namespace Developer {

Graph::Graph(const QString &graphPath, bool autoInitialise, QObject *parent)
    : GPFile(graphPath, parent)
    , _idCounter(1)
{
    if(autoInitialise && !graphPath.isEmpty())
        open();
}

Graph::Graph(const graph_t &inputGraph, QObject *parent)
    : GPFile(QString(), parent)
    , _idCounter(1)
{
    // We don't follow the normal open procedure here, since this is not coming
    // from a file. This is intended for create in-memory graph objects and
    // simply uses the GPFile derived object for consistency and avoiding
    // duplication of effort
    openGraphT(inputGraph);
}

bool Graph::save()
{
    // Some initial sanity checks
    QFileInfo info(_path);
    if(_path.isEmpty() || !_fp->isOpen() || _status == GPFile::ReadOnly)
    {
        qDebug() << "Couldn't save";
        return false;
    }

    _fp->close();
    _fp->open(QFile::Truncate | QFile::WriteOnly);
    qDebug() << "Saving graph file: " << _fp->fileName();

    // Determine how to save it
    GraphTypes type = DEFAULT_GRAPH_FORMAT;
    if(_path.endsWith(GP_GRAPH_ALTERNATIVE_EXTENSION))
        type = AlternativeGraph;
    if(_path.endsWith(GP_GRAPH_DOT_EXTENSION))
        type = DotGraph;
    if(_path.endsWith(GP_GRAPH_GXL_EXTENSION))
        type = GxlGraph;

    QString saveText;
    switch(type)
    {
    case AlternativeGraph:
        saveText = toAlternative();
        break;
    case GxlGraph:
        saveText = toGxl();
        break;
    case DotGraph:
    default:
        saveText = toDot();
        break;
    }

    int status = _fp->write(QVariant(saveText).toByteArray());
    if(status <= 0)
    {
        qDebug() << "    Save failed";
        return false;
    }

    _fp->close();
    _fp->open(QFile::ReadWrite);

    qDebug() << "    Save completed. Wrote " << status << " bytes";

    if(info.isWritable() && !_path.startsWith(":"))
        _status = Normal;
    else
        _status = ReadOnly;
    emit statusChanged(_status);
    return true;
}

bool Graph::saveAs(const QString &filePath)
{
    QString thePath = filePath;
    if(filePath.isEmpty())
    {
        QDir d = dir();
        QString dirPath;
        if(d.path().isEmpty())
            dirPath = QDir::homePath();
        else
            dirPath = d.absolutePath();

        thePath = QFileDialog::getSaveFileName(
                    0,
                    tr("Save Graph As..."),
                    dirPath,
                    tr("Graph Formats (*.gpg *.dot *.gxl)"));
        if(thePath.isEmpty())
            return false;
    }

    // Cache the path to the old file, if the save process fails then we should
    // restore the old one
    QString pathCache = _path;
    _path = thePath;
    open();
    if(!save())
    {
        // The save process failed
        qDebug() << "    Program could not be saved to " << thePath;
        qDebug() << "    Reopening previous file.";
        _path = pathCache;
        open();
        return false;
    }

    // Delete the old file as the move was successful
    QFile(pathCache).remove();

    // Update the file watcher
    return GPFile::saveAs(_path);
}

bool Graph::exportTo(const QString &filePath, GraphTypes outputType)
{
    QMessageBox::StandardButton ret = QMessageBox::question(
                0,
                tr("Retain Layout Information?"),
                tr("Do you wish to retain layout information in the exported "
                   "file?"),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes
                );

    bool keepLayout = (ret == QMessageBox::Yes);

    QString thePath = filePath;
    if(filePath.isEmpty())
    {
        QDir d = dir();
        QString dirPath;
        if(d.path().isEmpty())
            dirPath = QDir::homePath();
        else
            dirPath = d.absolutePath();

        QString filter;
        switch(outputType)
        {
        case AlternativeGraph:
            filter = tr("GP Graph Format (*.gpg)");
            break;
        case GxlGraph:
            filter = tr("GXL Format (*.gxl)");
            break;
        case DotGraph:
        default:
            filter = tr("Dot Format (*.gv *.dot)");
            break;
        }

        thePath = QFileDialog::getSaveFileName(
                    0,
                    tr("Export Graph To..."),
                    dirPath,
                    filter);
        if(thePath.isEmpty())
            return false;
    }

    qDebug() << "Exporting graph file: " << _fp->fileName();
    qDebug() << "    Destination file: " << thePath;

    QFile file(thePath);
    if(file.open(QFile::WriteOnly | QFile::Truncate))
    {
        if(!file.isWritable())
        {
            qDebug() << "    Could not write to destination file, export failed";
            return false;
        }

        QString graphText = toString(outputType, keepLayout);
        int status = file.write(QVariant(graphText).toByteArray());
        qDebug() << "    Export completed. Wrote " << status << " bytes";
        return true;
    }
    else
    {
        qDebug() << "    Could not open destination file, export failed";
        return false;
    }
}

bool Graph::open()
{
    if(!GPFile::open())
    {
        emit openComplete();
        return false;
    }

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
            graph = parseDotGraph(contents);
            break;
        }
    }

    if(!openGraphT(graph))
        return false;

    qDebug() << "    Finished parsing graph file: " << _path;

    emit openComplete();
    return true;
}

QRect Graph::canvas() const
{
    return _canvas;
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

Edge *Graph::edgeFrom(const QString &id) const
{
    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        Edge *e = *iter;
        if(e->from()->id() == id)
            return e;
    }

    return 0;
}

Edge *Graph::edgeTo(const QString &id) const
{
    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        Edge *e = *iter;
        if(e->to()->id() == id)
            return e;
    }

    return 0;
}

std::vector<Node *> Graph::nodes() const
{
    return _nodes;
}

std::vector<Edge *> Graph::edges(const QString &id) const
{
    if(id.isEmpty())
        return _edges;
    else
    {
        std::vector<Edge *> result;

        if(node(id) != 0)
        {
            std::vector<Edge *> from = edgesFrom(id);
            std::vector<Edge *> to = edgesTo(id);
            for(size_t i = 0; i < from.size(); ++i)
                result.push_back(from.at(i));
            for(size_t i = 0; i < to.size(); ++i)
                result.push_back(to.at(i));
        }

        return result;
    }
}

std::vector<Edge *> Graph::edgesFrom(const QString &id) const
{
    std::vector<Edge *> result;

    if(node(id) != 0)
    {
        for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
        {
            Edge *e = *iter;
            if(e->from()->id() == id)
                result.push_back(e);
        }
    }

    return result;
}

std::vector<Edge *> Graph::edgesTo(const QString &id) const
{
    std::vector<Edge *> result;

    if(node(id) != 0)
    {
        for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
        {
            Edge *e = *iter;
            if(e->to()->id() == id)
                result.push_back(e);
        }
    }

    return result;
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

QString Graph::toString(int outputType, bool keepLayout) const
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
        return toGxl(keepLayout);
        break;
    case AlternativeGraph:
        if(!keepLayout)
            qDebug() << "The GP graph format includes layout information in "
                     << "all cases, ignoring request to omit it.";
        return toAlternative();
        break;
    case DotGraph:
    case DefaultGraph:
    default:
        return toDot(keepLayout);
        break;
    }
}

QString Graph::toGxl(bool keepLayout) const
{
    QDomImplementation impl;
    QDomDocumentType gxlDoctype = impl.createDocumentType(
                "gxl",
                "http://www.gupro.de/GXL/gxl-1.0.dtd",
                "http://www.gupro.de/GXL/gxl-1.0.dtd"
                );

    QDomDocument doc(gxlDoctype);
    QDomElement root = doc.createElement("gxl");
    doc.appendChild(root);

    QDomElement graph = doc.createElement("graph");
    if(keepLayout)
    {
        graph.setAttribute("canvasWidth", _canvas.width());
        graph.setAttribute("canvasHeight", _canvas.height());
    }
    root.appendChild(graph);

    for(nodeConstIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        Node *n = *iter;
        if(n->isPhantomNode())
            continue;
        QDomElement node = doc.createElement("node");

        node.setAttribute("id", n->id());
        node.setAttribute("label", n->label().toString());
        if(n->isRoot())
            node.setAttribute("root", "true");
        if(keepLayout)
        {
            node.setAttribute("position", QVariant(n->pos().x()).toString()
                              + "," + QVariant(n->pos().y()).toString());
        }

        graph.appendChild(node);
    }

    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        Edge *e = *iter;
        if(e->isPhantomEdge())
            continue;
        QDomElement edge = doc.createElement("edge");

        edge.setAttribute("id", e->id());
        edge.setAttribute("label", e->label().toString());
        edge.setAttribute("from", e->from()->id());
        edge.setAttribute("to", e->to()->id());

        graph.appendChild(edge);
    }

    return doc.toString();
}

QString Graph::toDot(bool keepLayout) const
{
    QString result = "digraph " + baseName() + " {";
    result += "\n    node [shape=ellipse];";
    if(keepLayout)
    {
        result += "\n    graph [bb=\"0,0,"
                + QVariant(_canvas.width()).toString() + ","
                + QVariant(_canvas.height()).toString() + "\"];";
    }

    for(nodeConstIter iter = _nodes.begin(); iter != _nodes.end(); ++iter)
    {
        Node *n = *iter;
        if(n->isPhantomNode())
            continue;

        QString id = n->id();
        QRegExp rx("[^a-zA-Z0-9_]");
        if(rx.indexIn(id) != -1)
            id = QString("\"") + id + "\"";

        result += "\n    " + id + " [";
        result += "label=\"" + n->label().toString() + "\"";
        if(n->isRoot())
            result += ",root=\"true\"";
        if(keepLayout)
        {
            result += ",pos=\"" + QVariant(n->pos().x()).toString() + ","
                    + QVariant(n->pos().y()).toString() + "\"";
        }
        result += "];";
    }

    for(edgeConstIter iter = _edges.begin(); iter != _edges.end(); ++iter)
    {
        Edge *e = *iter;
        if(e->isPhantomEdge())
            continue;

        QString fromId = e->from()->id();
        QRegExp rx("[^a-zA-Z0-9_]");
        if(rx.indexIn(fromId) != -1)
            fromId = QString("\"") + fromId + "\"";

        QString toId = e->to()->id();
        if(rx.indexIn(toId) != -1)
            toId = QString("\"") + toId + "\"";

        result += "\n    " + fromId + " -> " + toId + " [";
        result += "id=\"" + e->id() + "\"";
        result += ",label=\"" + e->label().toString() + "\"";
        result += "];";
    }

    result += "\n}\n";

    return result;
}

QString Graph::toAlternative() const
{
    // First add the canvas
    QString result = "(";
    result += QVariant(_canvas.width()).toString();
    result += ",";
    result += QVariant(_canvas.height()).toString();
    result += ")";

    // Add the nodes
    bool first = true;
    for(size_t i = 0; i < _nodes.size(); ++i)
    {
        Node *n = _nodes.at(i);
        if(n->isPhantomNode())
            continue;

        if(first)
        {
            first = false;
            result += "\n    | ";
        }
        else
            result += ",\n      ";

        result += "(" + n->id() + ", " + n->label().toString() + ", ("
                + QVariant(n->pos().x()).toString()
                + ", " + QVariant(n->pos().y()).toString() + ") )";
    }

    // Add the edges
    first = true;
    for(size_t i = 0; i < _edges.size(); ++i)
    {
        Edge *e = _edges.at(i);
        if(e->isPhantomEdge())
            continue;

        if(first)
        {
            first = false;
            result += "\n    | ";
        }
        else
            result += ",\n      ";

        result += "(" + e->id() + ", " + e->from()->id() + ", " + e->to()->id()
                + ", " + e->label().toString() + ")";
    }

    result += "\n";

    return result;
}

void Graph::setCanvas(const QRect &rect)
{
    _canvas = rect;
}

Edge *Graph::addEdge(const QString &id, Node *from, Node *to, const List &label)
{
    if(contains(id))
        return 0;

    Edge *e = new Edge(id, from, to, label, this);
    _edges.push_back(e);

    emit edgeAdded(e);

    return e;
}

Edge *Graph::addEdge(Node *from, Node *to, const List &label)
{
    // Is there already a node or edge with this label?

    // Are the two nodes provided real nodes tracked by this graph

    Edge *e = new Edge(newId(), from, to, label, this);
    _edges.push_back(e);

    _status = Modified;
    emit statusChanged(Modified);
    emit edgeAdded(e);

    return e;
}

Node *Graph::addNode(const QString &id, const List &label, const QPointF &pos)
{
    if(contains(id))
        return 0;

    Node *n = new Node(id, label, pos, this);
    _nodes.push_back(n);

    emit nodeAdded(n);

    return n;
}

Node *Graph::addNode(const List &label, const QPointF &pos)
{
    // Is there already a node or edge with this label?
    // do we care if there is?

    Node *n = new Node(newId(), label, pos, this);
    if(_nodes.size() == 0)
        n->setIsRoot(true);
    _nodes.push_back(n);

    _status = Modified;
    emit statusChanged(Modified);
    emit nodeAdded(n);

    return n;
}

bool Graph::removeEdge(const QString &id)
{
    // Get the edge to delete
    Edge *e = edge(id);

    // If we failed to find it then we can't delete it
    if(e == 0)
        return false;

    edgeIter iter = _edges.begin();
    while(iter != _edges.end()
          && (*iter)->id() != e->id())
        ++iter;

    if(iter == _edges.end())
    {
        // This shouldn't have happened, we found it earlier...
        qDebug() << QString("Unexpected failure in Graph::removeEdge(%1)").arg(
                        id).toStdString().c_str();
        return false;
    }

    _edges.erase(iter);
    delete e;
    emit edgeRemoved(id);
    return true;
}

bool Graph::removeNode(const QString &id, bool strict)
{
    // Get the node to delete
    Node *n = node(id);

    // If we failed to find it then we can't delete it
    if(n == 0)
        return false;

    // Are there incident edges?
    std::vector<Edge *> nodeEdges = n->edges();
    if(nodeEdges.size() > 0)
    {
        // If we are being strict then this removal fails due to the incident
        // edges, these must be removed first
        if(strict)
            return false;

        for(edgeIter iter = nodeEdges.begin(); iter != nodeEdges.end(); ++iter)
        {
            if(!removeEdge((*iter)->id()))
            {
                qDebug() << "Removal of incident edge failed: " << (*iter)->id();
                return false;
            }
        }
    }

    nodeIter iter = _nodes.begin();
    while(iter != _nodes.end()
          && (*iter)->id() != n->id())
        ++iter;

    if(iter == _nodes.end())
    {
        // This shouldn't have happened, we found it earlier...
        qDebug() << QString("Unexpected failure in Graph::removeNode(%1)").arg(
                        id).toStdString().c_str();
        return false;
    }

    _nodes.erase(iter);
    delete n;
    emit nodeRemoved(id);
    return true;
}

bool Graph::openGraphT(const graph_t &inputGraph)
{
    for(size_t i = 0; i < inputGraph.nodes.size(); ++i)
    {
        node_t node = inputGraph.nodes.at(i);
        if(contains(node.id.c_str()))
        {
            qDebug() << "    Duplicate ID found: " << node.id.c_str();
            qDebug() << "    Graph parsing failed.";
            emit openComplete();
            return false;
        }

        Node *n = new Node(node.id.c_str(), List(node.label),
                           QPointF(node.xPos, node.yPos), this);
        emit nodeAdded(n);
        _nodes.push_back(n);
    }

    for(size_t i = 0; i < inputGraph.edges.size(); ++i)
    {
        edge_t edge = inputGraph.edges.at(i);
        if(contains(edge.id.c_str()))
        {
            qDebug() << "    Duplicate ID found: " << edge.id.c_str();
            qDebug() << "    Graph parsing failed.";
            emit openComplete();
            return false;
        }

        Node *from = node(edge.from.c_str());
        Node *to = node(edge.to.c_str());

        if(from == 0)
        {
            qDebug() << "    Edge " << edge.id.c_str() << " references non-existent node "
                     << edge.from.c_str();
            qDebug() << "    Graph parsing failed.";
            emit openComplete();
            return false;
        }

        if(to == 0)
        {
            qDebug() << "    Edge " << edge.id.c_str() << " references non-existent node "
                     << edge.to.c_str();
            qDebug() << "    Graph parsing failed.";
            emit openComplete();
            return false;
        }

        Edge *e = new Edge(edge.id.c_str(), from, to, List(edge.label), this);
        emit edgeAdded(e);
        _edges.push_back(e);
    }

    return true;
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
