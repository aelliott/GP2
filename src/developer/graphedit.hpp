/*!
 * \file
 */
#ifndef GRAPHEDIT_HPP
#define GRAPHEDIT_HPP

#include <QWidget>

namespace Ui {
class GraphEdit;
}

namespace Developer {

class Graph;

class GraphEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphEdit(QWidget *parent = 0);
    ~GraphEdit();

    void setGraph(Graph *graph);
    
private:
    Ui::GraphEdit *_ui;
    Graph *_graph;
};

}

#endif // GRAPHEDIT_HPP
