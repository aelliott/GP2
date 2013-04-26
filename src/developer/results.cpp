/*!
 * \file
 */
#include "results.hpp"
#include "ui_results.h"

namespace Developer {

Results::Results(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Results)
{
    _ui->setupUi(this);


    QStringList items;
    items << "Results";
    QTreeWidgetItem *root = new QTreeWidgetItem(items);
    root->setIcon(0, QIcon(QPixmap(":/icons/application-icon.png")));
    _ui->resultsTreeWidget->addTopLevelItem(root);
}

Results::~Results()
{
    delete _ui;
}

}
