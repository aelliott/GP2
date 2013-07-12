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
#include "runconfiguration.hpp"
#include "ui_runconfiguration.h"

#include "project.hpp"

#include <QFile>

namespace Developer {

RunConfiguration::RunConfiguration(Project *proj, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::RunConfiguration)
    , _project(proj)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/runconfiguration.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->detailsWidget->setVisible(false);

    updatePrograms();
    updateGraphs();
}

RunConfiguration::~RunConfiguration()
{
    delete _ui;
}

void RunConfiguration::toggleDetails()
{
    _ui->detailsWidget->setVisible(!_ui->detailsWidget->isVisible());

    if(_ui->detailsWidget->isVisible())
        _ui->arrowImage->setPixmap(QPixmap(":/icons/bullet_arrow_down.png"));
    else
        _ui->arrowImage->setPixmap(QPixmap(":/icons/bullet_arrow_right.png"));
}

void RunConfiguration::updatePrograms()
{
    _ui->programCombo->clear();
    QVector<Program *> programs = _project->programs();
    for(QVector<Program *>::iterator iter = programs.begin();
        iter != programs.end(); ++iter)
    {
        Program *prog = *iter;
        if(prog == 0)
        {
            qDebug() << "Null pointer in RunConfiguration::updatePrograms(), "
                        "ignoring.";
        }
        _ui->programCombo->addItem(prog->name());
    }
}

void RunConfiguration::updateGraphs()
{
    _ui->targetGraphCombo->clear();
    QVector<Graph *> graphs = _project->graphs();
    for(QVector<Graph *>::iterator iter = graphs.begin();
        iter != graphs.end(); ++iter)
    {
        Graph *graph = *iter;
        if(graph == 0)
        {
            qDebug() << "Null pointer in RunConfiguration::updateGraphs(), "
                        "ignoring.";
        }
        _ui->targetGraphCombo->addItem(graph->fileName());
    }
}

}
