#include "run.hpp"
#include "ui_run.h"

Run::Run(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Run)
{
    _ui->setupUi(this);
}

Run::~Run()
{
    delete _ui;
}
