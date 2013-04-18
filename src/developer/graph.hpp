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
    Graph(const QString &graphPath = QString(),
          bool autoInitialise = true,
          QObject *parent = 0);
    Graph(const graph_t &inputGraph, QObject *parent = 0);

    bool save();
    bool saveAs(const QString &filePath);
    bool exportTo(const QString &filePath, GraphTypes outputType);

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
    QStringList nodeIdentifiers() const;
    QStringList edgeIdentifiers() const;
    QStringList variables() const;

    bool contains(const QString &id) const;
    bool containsNode(const QString &id) const;
    bool containsEdge(const QString &id) const;

    QString toString(int outputType = DefaultGraph, bool keepLayout = true) const;
    QString toGxl(bool keepLayout = true) const;
    QString toDot(bool keepLayout = true) const;
    QString toAlternative() const;


signals:
    void graphChanged();
    void nodeAdded(Node *n);
    void edgeAdded(Edge *e);
    void nodeRemoved(QString id);
    void edgeRemoved(QString id);
    void openComplete();

public slots:
    void setCanvas(const QRect &rect);
    Node *addNode(const QString &id, const List &label = List(), const QPointF &pos = QPointF());
    Node *addNode(const List &label = List(), const QPointF &pos = QPointF());
    Edge *addEdge(const QString &id, Node *from, Node *to, const List &label = List());
    Edge *addEdge(Node *from, Node *to, const List &label = List());

    /*!
     * \brief Remove the node with the given ID from the graph if it exists
     *
     * Node removal can be permissive or strict, with the default being
     * permissive. When deletion is permissive a node which has inbound or
     * outbound edges causes deletion to cascade to those edges. If deletion is
     * strict then the removal will fail and return false unless the node has no
     * incident edges.
     *
     * \param   id      The ID of the node to remove
     * \param   strict  True if removal should fail when the node has incident
     *      edges, false if any incident edges should be automatically rmeoved
     * \return  True if the removal was successful, false otherwise
     */
    bool removeNode(const QString &id, bool strict = false);
    bool removeEdge(const QString &id);

protected slots:
    void trackChange();

protected:
    // Protected member functions
    bool openGraphT(const graph_t &inputGraph);
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
