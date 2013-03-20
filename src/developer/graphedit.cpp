/*!
 * \file
 */
#include "graphedit.hpp"
#include "ui_graphedit.h"

namespace Developer {

GraphEdit::GraphEdit(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GraphEdit)
{
    _ui->setupUi(this);
}

GraphEdit::~GraphEdit()
{
    delete _ui;
}

}
