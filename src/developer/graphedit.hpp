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
#ifndef GRAPHEDIT_HPP
#define GRAPHEDIT_HPP

#include <QWidget>

namespace Ui {
class GraphEdit;
}

namespace Developer {

class Graph;
class GraphWidget;

class GraphEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphEdit(QWidget *parent = 0);
    ~GraphEdit();

    void setGraph(Graph *graph);

public slots:
    void handleGraphHasFocus(GraphWidget *graphWidget);
    void handleGraphLostFocus(GraphWidget *graphWidget);

signals:
    void graphHasFocus(GraphWidget *graphWidget);
    void graphLostFocus(GraphWidget *graphWidget);
    
private:
    Ui::GraphEdit *_ui;
    Graph *_graph;
};

}

#endif // GRAPHEDIT_HPP
