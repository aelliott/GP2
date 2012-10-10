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
