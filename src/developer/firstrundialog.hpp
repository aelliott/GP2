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
#ifndef FIRSTRUNDIALOG_HPP
#define FIRSTRUNDIALOG_HPP

#include <QDialog>

namespace Ui {
    class FirstRunDialog;
}

namespace Developer {

class Graph;

class FirstRunDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirstRunDialog(QWidget *parent = 0);
    ~FirstRunDialog();

public slots:
    void next();
    void back();
    void setPage(int page);

    void nodeAdded();
    void nodeRemoved();
    void edgeAdded();
    void edgeRemoved();

private:
    Ui::FirstRunDialog *_ui;
    Graph *_initialGraph;
    Graph *_editingGraph;
    Graph *_largeGraph;
    Graph *_rhsGraph;
    int _page;
    int _previousPage;
    int _nodeCount;
    int _edgeCount;
    bool _addingNodesLock;
    bool _addingEdgesLock;
    bool _deletingElementsLock;
    bool _editingRhsLock;
};

}

#endif // FIRSTRUNDIALOG_HPP
