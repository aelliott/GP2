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
#include "quickrunwidget.hpp"
#include "ui_quickrunwidget.h"

#include "project.hpp"

#include <QDebug>

namespace Developer {

QuickRunWidget::QuickRunWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::QuickRunWidget)
{
    _ui->setupUi(this);
}

QuickRunWidget::~QuickRunWidget()
{
    delete _ui;
}

void QuickRunWidget::setProject(Project *project)
{
    _ui->quickRunProjectName->setText(project->name());

    //! \todo iterate across run configurations, also need to track them
}

void QuickRunWidget::run()
{
    qDebug() << "Run requested: " << _ui->quickRunConfigCombo->currentText();
}

}
