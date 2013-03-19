/*!
 * \file
 */
#include "results.hpp"
#include "ui_results.h"

namespace Developer {

Results::Results(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Results)
{
    ui->setupUi(this);
}

Results::~Results()
{
    delete ui;
}

}
