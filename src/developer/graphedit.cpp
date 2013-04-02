/*!
 * \file
 */
#include "graphedit.hpp"
#include "ui_graphedit.h"

namespace Developer {

GraphEdit::GraphEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::GraphEdit)
    , _graph(0)
{
    _ui->setupUi(this);
}

GraphEdit::~GraphEdit()
{
    delete _ui;
}

void GraphEdit::setGraph(Graph *graph)
{
    _ui->graphicsView->setGraph(graph);
}

}
