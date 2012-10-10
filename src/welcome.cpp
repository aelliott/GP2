#include "welcome.hpp"
#include "ui_welcome.h"

Welcome::Welcome(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Welcome)
{
    _ui->setupUi(this);
}

Welcome::~Welcome()
{
    delete _ui;
}
