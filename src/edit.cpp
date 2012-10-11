#include "edit.hpp"
#include "ui_edit.h"

#include <QFile>

Edit::Edit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Edit)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);
}

Edit::~Edit()
{
    delete _ui;
}

void Edit::fileClicked(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if(parent == 0)
        return;

    // Handle a clicked rule
    if(parent->text(0) == tr("Rules"))
    {
        _ui->stackedWidget->setCurrentIndex(0);
    }

    // Handle a clicked program
    if(parent->text(0) == tr("Programs"))
    {
        _ui->stackedWidget->setCurrentIndex(1);
    }

    // Handle a clicked graph
    if(parent->text(0) == tr("Graphs"))
    {
        _ui->stackedWidget->setCurrentIndex(2);
    }
}
