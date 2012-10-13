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
#include "prettytabbar.hpp"

#include <QDebug>

PrettyTabBar::PrettyTabBar(QWidget *parent)
    : QWidget(parent)
    , _menuCount(0)
{
    _layout = new QVBoxLayout(this);
    _layout->setMargin(0);
    _layout->addStretch();

    // Initialise the set of menus with a default menu
    _menus.clear();

    PrettyTabMenu *menu = new PrettyTabMenu(QString(), this);
    _menus["default"] = menu;
    _layout->insertWidget(0, menu);
    ++_menuCount;

    connect(menu, SIGNAL(tabTriggered(QString,int)), this,
            SLOT(tabTriggered(QString,int)));
}

PrettyTab *PrettyTabBar::tab(const QString &name, int index) const
{
    if(_menus.contains(name))
        return _menus[name]->tab(index);
    else
        return 0;
}

void PrettyTabBar::addMenu(const QString &name)
{
    // There must be a name provided
    if(name.isEmpty())
        return;

    // Don't overwrite existing menus
    if(_menus.contains(name))
        return;

    // Insert the new menu
    PrettyTabMenu *menu = new PrettyTabMenu(name, this);
    _menus[name] = menu;
    _layout->insertWidget(_menuCount, menu);
    ++_menuCount;

    connect(menu, SIGNAL(tabTriggered(QString,int)), this,
            SLOT(tabTriggered(QString,int)));
}

QPair<QString, int> PrettyTabBar::addTab(const QString &name, const QIcon &icon, const QString &menu)
{
    // Default to (unsurprisingly) the "default" menu if one is not provided
    QString selectedMenu;
    if(menu.isEmpty())
        selectedMenu = "default";
    else
        selectedMenu = menu;

    // Ensure that we are inserting into an existing menu, if not then create it
    if(!_menus.contains(selectedMenu))
        addMenu(menu);

    int index = _menus[selectedMenu]->addTab(name, icon);

    return QPair<QString, int>(selectedMenu, index);
}

bool PrettyTabBar::containsMenu(QString menu) const
{
    return _menus.contains(menu);
}

void PrettyTabBar::setCurrentIndex(QString menu, int index)
{
    // This is the publicly facing API which can be called at any time by an
    // external part of the program, therefore we must update the tab bar -
    // which will then push its update to the rest via the internal
    // tabTriggered method

    // First ensure that the menu exists
    if(!_menus.contains(menu))
        return;

    // Ensure that the menu has a tab at this index
    if(!_menus[menu]->contains(index))
        return;

    _menus[menu]->setCurrentIndex(index);
}

void PrettyTabBar::tabTriggered(QString menu, int index)
{
    // If there is no menu specified, we must be using "default"
    if(menu.isEmpty())
        menu = "default";

    // Clear all other menus to ensure that only the menu whose item has been
    // triggered will have a selected tab
    QMap<QString, PrettyTabMenu *>::iterator iter;
    for(iter = _menus.begin(); iter != _menus.end(); ++iter)
    {
        if(iter.key() != menu)
            iter.value()->clearSelection();
    }

    emit currentChanged(menu, index);
}
