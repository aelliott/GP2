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

void PrettyTabWidget::setTabEnabled(const QString &menu, int index, bool enabled)
{
    _ui->prettyTabs->tab(menu, index)->setEnabled(enabled);
}

bool PrettyTabWidget::addMenu(const QString &menu)
{
    return _ui->prettyTabs->addMenu(menu);
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
