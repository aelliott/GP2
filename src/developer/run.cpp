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
#include "run.hpp"
#include "ui_run.h"

#include "project.hpp"
#include "runconfiguration.hpp"

namespace Developer {

Run::Run(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Run)
    , _initial(true)
{
    _ui->setupUi(this);
}

Run::~Run()
{
    delete _ui;
}

Project *Run::project() const
{
    return _project;
}

void Run::setProject(Project *proj)
{
    if(proj == 0)
    {
        qDebug() << "Run::setProject() handed null pointer, ignoring";
        return;
    }

    _project = proj;

    QLayoutItem *item;
    while((item = _ui->runConfigurations->layout()->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }

    QVector<RunConfig *> runConfigs = _project->runConfigurations();
    if(runConfigs.size() < 1)
    {
        _initial = true;
        QLabel *label = new QLabel(tr("There are currently no run "
                                      "configurations set, click the button "
                                      "below to add one"),
                                   _ui->runConfigurations);
        _ui->runConfigurations->layout()->addWidget(label);
    }
    else
    {
        // Populate with run configurations
    }
    _ui->addRunConfigurationButton->setEnabled(true);
}

RunConfiguration *Run::addRunConfiguration(bool addToProject)
{
    if(_initial)
    {
        _initial = false;

        QLayoutItem *item;
        while((item = _ui->runConfigurations->layout()->takeAt(0)) != 0)
        {
            delete item->widget();
            delete item;
        }
    }

    RunConfiguration *runConfig = new RunConfiguration(_project,
                                                       _ui->runConfigurations);
    _ui->runConfigurations->layout()->addWidget(runConfig);

    if(addToProject)
    {
        // Add the run configuration to the project
    }

    return runConfig;
}

}
