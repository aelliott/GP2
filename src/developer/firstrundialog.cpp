/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "firstrundialog.hpp"
#include "ui_firstrundialog.h"

#include <QFile>

#include "graphview/graphwidget.hpp"
#include "graphview/graphscene.hpp"
#include "graph.hpp"

namespace Developer
{

FirstRunDialog::FirstRunDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::FirstRunDialog)
    , _initialGraph(0)
    , _editingGraph(0)
    , _largeGraph(0)
    , _rhsGraph(0)
    , _page(0)
    , _previousPage(0)
    , _nodeCount(0)
    , _edgeCount(0)
    , _addingNodesLock(false)
    , _addingEdgesLock(false)
    , _deletingElementsLock(false)
    , _editingRhsLock(false)
{
    _ui->setupUi(this);

    // Load the help stylesheet and apply it to this widget
    QFile fp(":/stylesheets/helpdialog.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    setPage(0);
}

FirstRunDialog::~FirstRunDialog()
{
    delete _ui;
}

void FirstRunDialog::back()
{
    _previousPage = _page;
    setPage(_page-1);
}

void FirstRunDialog::next()
{
    _previousPage = _page;
    setPage(_page+1);
}

void FirstRunDialog::setPage(int page)
{
    _page = page;
    _ui->stackedWidget->setCurrentIndex(page);

    switch(page)
    {
    case 7: // RHS graphs
        if(_previousPage == 6)
        {
            _rhsGraph = new Graph(":/templates/example_graph_rhs.gpg");
            _ui->graphWidget->setGraph(_rhsGraph);
            _ui->graphWidget->setLinkedGraph(_initialGraph);
            // Cheat a bit and set it as read-writable to enable editing, really
            // we still can't save it but that's ok because we don't offer them
            // that option anyway
            _ui->graphWidget->graphScene()->setReadOnly(false);
        }
        _ui->backButton->setEnabled(true);
        _ui->nextButton->setVisible(false);
        _ui->beginButton->setEnabled(_editingRhsLock);
        _ui->beginButton->setVisible(true);
        break;
    case 6: // large graphs
        if(_previousPage == 5)
            _largeGraph = new Graph(":/templates/example_large_graph.gv");
        if(_previousPage == 7)
            _ui->graphWidget->setLinkedGraph(0);
         _ui->graphWidget->setGraph(_largeGraph);
         _ui->backButton->setEnabled(true);
         _ui->nextButton->setVisible(true);
         _ui->nextButton->setEnabled(true);
         _ui->beginButton->setVisible(false);
         break;
    case 5: // deleting elements
        if(_previousPage == 6)
            _ui->graphWidget->setGraph(_editingGraph);
        _nodeCount = 0;
        _edgeCount = 0;
        _ui->backButton->setEnabled(true);
        _ui->nextButton->setEnabled(_deletingElementsLock);
        _ui->beginButton->setVisible(false);
        break;
    case 4: // selecting elements
    case 3: // editing elements
        _ui->backButton->setEnabled(true);
        _ui->nextButton->setEnabled(true);
        _ui->beginButton->setVisible(false);
        break;
    case 2: // adding edges
        _edgeCount = 0;
        _ui->backButton->setEnabled(true);
        _ui->nextButton->setEnabled(_addingEdgesLock);
        _ui->beginButton->setVisible(false);
        break;
    case 1: // adding nodes
        if(_previousPage == 0)
        {
            _nodeCount = 0;
            _editingGraph = new Graph();

            connect(_editingGraph, SIGNAL(nodeAdded(Node*)),
                    this, SLOT(nodeAdded()));
            connect(_editingGraph, SIGNAL(nodeRemoved(QString)),
                    this, SLOT(nodeRemoved()));
            connect(_editingGraph, SIGNAL(edgeAdded(Edge*)),
                    this, SLOT(edgeAdded()));
            connect(_editingGraph, SIGNAL(edgeRemoved(QString)),
                    this, SLOT(edgeRemoved()));

            _ui->graphWidget->setGraph(_editingGraph);
        }
        _ui->backButton->setEnabled(true);
        _ui->nextButton->setEnabled(_addingNodesLock);
        _ui->beginButton->setVisible(false);
        break;
    case 0: // introduction
    default:
        if(_initialGraph == 0)
            _initialGraph = new Graph(":/templates/example_graph.gpg");
        _ui->graphWidget->setGraph(_initialGraph);
        _ui->backButton->setEnabled(false);
        _ui->nextButton->setEnabled(true);
        _ui->beginButton->setVisible(false);
        break;
    }
}

void FirstRunDialog::nodeAdded()
{
    if(_page == 1)
    {
        // We are counting nodes up to three
        ++_nodeCount;
        _ui->nodeCount->setText(QVariant(_nodeCount).toString());
        if(_nodeCount > 2)
        {
            _addingNodesLock = true;
            _ui->nodeCountIndicator->setPixmap(
                        QPixmap(":/icons/small_tick.png")
                        );
            _ui->nextButton->setEnabled(true);
        }
    }
}

void FirstRunDialog::nodeRemoved()
{
    if(_page == 5)
    {
        ++_nodeCount;
        _ui->nodeDeleteCount->setText(QVariant(_nodeCount).toString());
        if(_nodeCount > 2)
            _ui->nodeDeleteCountIndicator->setPixmap(
                        QPixmap(":/icons/small_tick.png")
                        );
        if(_nodeCount > 2 && _edgeCount > 2)
        {
            _deletingElementsLock = true;
            _ui->nextButton->setEnabled(true);
        }
    }
}

void FirstRunDialog::edgeAdded()
{
    if(_page == 2)
    {
        // We are counting edges up to three
        ++_edgeCount;
        _ui->edgeCount->setText(QVariant(_edgeCount).toString());
        if(_edgeCount > 2)
        {
            _addingEdgesLock = true;
            _ui->edgeCountIndicator->setPixmap(
                        QPixmap(":/icons/small_tick.png")
                        );
            _ui->nextButton->setEnabled(true);
        }
    }
}

void FirstRunDialog::edgeRemoved()
{
    if(_page == 5)
    {
        ++_edgeCount;
        _ui->edgeDeleteCount->setText(QVariant(_edgeCount).toString());
        if(_edgeCount > 2)
            _ui->edgeDeleteCountIndicator->setPixmap(
                        QPixmap(":/icons/small_tick.png")
                        );
        if(_nodeCount > 2 && _edgeCount > 2)
        {
            _deletingElementsLock = true;
            _ui->nextButton->setEnabled(true);
        }
    }
}

}
