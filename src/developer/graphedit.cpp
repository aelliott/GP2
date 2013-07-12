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
#include "graphedit.hpp"
#include "ui_graphedit.h"

namespace Developer {

GraphEdit::GraphEdit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::GraphEdit)
    , _graph(0)
{
    _ui->setupUi(this);

    connect(_ui->graphicsView, SIGNAL(graphHasFocus(GraphWidget*)),
            this, SLOT(handleGraphHasFocus(GraphWidget*)));
    connect(_ui->graphicsView, SIGNAL(graphLostFocus(GraphWidget*)),
            this, SLOT(handleGraphLostFocus(GraphWidget*)));
}

GraphEdit::~GraphEdit()
{
    delete _ui;
}

void GraphEdit::setGraph(Graph *graph)
{
    _ui->graphicsView->setGraph(graph);
}

void GraphEdit::handleGraphHasFocus(GraphWidget *graphWidget)
{
    emit graphHasFocus(graphWidget);
}

void GraphEdit::handleGraphLostFocus(GraphWidget *graphWidget)
{
    emit graphLostFocus(graphWidget);
}

}
