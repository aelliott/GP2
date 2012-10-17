#include "preferencesdialog.hpp"
#include "ui_preferencesdialog.h"

#include <QFile>

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::PreferencesDialog)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/preferences.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->mainWidget->addTab(new QWidget(this),
                            QIcon(QPixmap(":/icons/folder.png")),
                            tr("Projects")
                            );

    _ui->mainWidget->addTab(new QWidget(this),
                            QIcon(QPixmap(":/icons/palette.png")),
                            tr("Appearance")
                            );

    _ui->mainWidget->addTab(new QWidget(this),
                            QIcon(QPixmap(":/icons/cog.png")),
                            tr("YAM")
                            );
}

PreferencesDialog::~PreferencesDialog()
{
    delete _ui;
}
