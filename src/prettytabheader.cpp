#include "prettytabheader.hpp"
#include "ui_prettytabheader.h"

PrettyTabHeader::PrettyTabHeader(const QString &title, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::PrettyTabHeader)
{
    _ui->setupUi(this);
    _ui->label->setText(title);
}

PrettyTabHeader::~PrettyTabHeader()
{
    delete _ui;
}
