/*!
 * \file
 */
#include "runconfiguration.hpp"
#include "ui_runconfiguration.h"

#include <QFile>

namespace Developer {

RunConfiguration::RunConfiguration(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::RunConfiguration)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/runconfiguration.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->detailsWidget->setVisible(false);
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

}
