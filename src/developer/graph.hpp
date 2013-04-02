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
#include <QRect>

namespace Developer {

class Graph : public GPFile
{
    Q_OBJECT

public:
    Graph(const QString &graphPath = QString(), QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);

    bool open();

    QRect canvas() const;
    Node *node(const QString &id) const;
    Edge *edge(const QString &id) const;
    Edge *edgeFrom(const QString &id) const;
    Edge *edgeTo(const QString &id) const;
    std::vector<Node *> nodes() const;
    std::vector<Edge *> edges(const QString &id = QString()) const;
    std::vector<Edge *> edgesFrom(const QString &id) const;
    std::vector<Edge *> edgesTo(const QString &id) const;

    bool contains(const QString &id) const;
    bool containsNode(const QString &id) const;
    bool containsEdge(const QString &id) const;

    QString toString(int outputType = DefaultGraph) const;
    QString toGxl() const;
    QString toDot() const;
    QString toAlternative() const;

public slots:
    void setCanvas(const QRect &rect);
    Node *addNode(const QString &label = QString(), const QPointF &pos = QPointF());
    Edge *addEdge(Node *from, Node *to, const QString &label = QString());

protected:
    // Protected member functions
    QString newId();

    // Protected member variables
    int _idCounter;
    QRect _canvas;
    std::vector<Node *> _nodes;
    std::vector<Edge *> _edges;

    // Some convenience typedefs (not going to tie in C++11 as a requirement)
    typedef std::vector<Node *>::iterator nodeIter;
    typedef std::vector<Edge *>::iterator edgeIter;
    typedef std::vector<Node *>::const_iterator nodeConstIter;
    typedef std::vector<Edge *>::const_iterator edgeConstIter;
};

}

#endif // GRAPH_HPP
