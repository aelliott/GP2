/*!
 * Copyright (c) 2012 Alex Elliott <alex@alex-elliott.co.uk>
 *
 * This file is part of SlackTracker
 *
 * SlackTracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Expression editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Expression editor. If not, see <http://www.gnu.org/licenses/>.
 */
#include "prettytabwidget.hpp"
#include "ui_prettytabwidget.h"

#include <QFile>
#include <QDebug>

PrettyTabWidget::PrettyTabWidget(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::PrettyTabWidget)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/tabwidget.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    connect(_ui->prettyTabs, SIGNAL(currentChanged(QString,int)), this,
            SLOT(tabTriggered(QString,int)));
}

PrettyTabWidget::~PrettyTabWidget()
{
    delete _ui;
}

void PrettyTabWidget::addMenu(const QString &menu)
{
    _ui->prettyTabs->addMenu(menu);
}

QPair<QString, int> PrettyTabWidget::addTab(QWidget *page, const QIcon &icon, const QString &name, const QString &menu)
{
    QString selectedMenu;
    if(menu.isEmpty())
        selectedMenu = "default";
    else
        selectedMenu = menu;

    if(!_ui->prettyTabs->containsMenu(selectedMenu))
        _ui->prettyTabs->addMenu(selectedMenu);

    QPair<QString, int> index = _ui->prettyTabs->addTab(name, icon, menu);
    int pageNumber = _ui->stackedWidget->addWidget(page);

    _pageMapping.insert(index, pageNumber);
    _nameMapping.insert(index, name);

    if(selectedMenu == "default" && index.second == 0)
        setCurrentIndex(selectedMenu, 0);

    return index;
}

void PrettyTabWidget::setCurrentIndex(QString menu, int index)
{
    _ui->prettyTabs->setCurrentIndex(menu, index);
}

void PrettyTabWidget::tabTriggered(QString menu, int index)
{
    QPair<QString, int> tabIndex(menu, index);

    if(!_pageMapping.contains(tabIndex))
        return;

    _ui->stackedWidget->setCurrentIndex(_pageMapping[tabIndex]);

    emit currentChanged(_nameMapping[tabIndex]);
}
