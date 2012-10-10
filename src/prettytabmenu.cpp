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
#include "prettytabmenu.hpp"

#include "prettytabheader.hpp"
#include <QDebug>

PrettyTabMenu::PrettyTabMenu(const QString &title, QWidget *parent)
    : QWidget(parent)
    , _title(title)
    , _tabCount(0)
{
    _layout = new QVBoxLayout(this);
    _layout->setMargin(0);
    _layout->setSpacing(0);

    // If we have been passed a title make an item for it and increase the tab
    // count as if it was a tab (though it won't respond to clicks)
    if(!title.isEmpty())
    {
        PrettyTabHeader *label = new PrettyTabHeader(title, this);
        _layout->insertWidget(0, label);
        ++_tabCount;
    }

    _mapper = new QSignalMapper(this);
    connect(_mapper, SIGNAL(mapped(int)), this, SLOT(tabTriggered(int)));
}

int PrettyTabMenu::addTab(const QString &name, const QIcon &icon)
{
    // Insert the new tab
    PrettyTab *tab = new PrettyTab(name, icon, this);
    _layout->insertWidget(_tabCount, tab);

    // Check if this is the first tab in the "default" menu, if it is then we
    // need to inform it that it is a special case in display terms
    if(_tabs.size() == 0 && (_title.isEmpty() || _title == "default"))
    {
        tab->setTop(true);
    }

    connect(tab, SIGNAL(pressed()), _mapper, SLOT(map()));
    _mapper->setMapping(tab, _tabCount);

    _tabs << tab;

    // return _tabCount++
    int ret = _tabCount;
    ++_tabCount;
    return ret;
}

QString PrettyTabMenu::title() const
{
    return _title;
}

bool PrettyTabMenu::contains(int index) const
{
    if(index < 0 || index >= _tabCount)
        return false;
    return true;
}

void PrettyTabMenu::clearSelection()
{
    for(int i = 0; i < _tabs.size(); ++i)
        _tabs.at(i)->clearSelection();
}

void PrettyTabMenu::setCurrentIndex(int index)
{
    // Update the current tab
    int internalIndex = index;
    if(!_title.isEmpty())
        --internalIndex;

    _tabs.at(internalIndex)->setSelected(true);
}

void PrettyTabMenu::tabTriggered(int index)
{
    // Update the current tab
    int internalIndex = index;
    if(!_title.isEmpty())
        --internalIndex;

    clearSelection();
    _tabs.at(internalIndex)->setSelected(true);

    // Emit a signal announcing this change of state
    if(_title.isEmpty())
        emit tabTriggered(QString("default"), index);
    else
        emit tabTriggered(_title, index);
}
