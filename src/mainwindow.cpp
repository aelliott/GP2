#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// Include main page elements
#include "welcome.hpp"
#include "edit.hpp"
#include "run.hpp"

// Include spawned dialogs
#include "helpdialog.hpp"

#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->tabWidget->addTab(new Welcome(this),
                           QIcon(QPixmap(":/icons/application-icon.png")),
                           tr("Welcome")
                           );

    _ui->tabWidget->addTab(new Edit(this),
                           QIcon(QPixmap(":/icons/edit.png")),
                           tr("Edit")
                           );

    _ui->tabWidget->addTab(new Run(this),
                           QIcon(QPixmap(":/icons/run.png")),
                           tr("Run")
                           );

    statusBar()->showMessage(tr("Ready."));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::showApplicationHelp()
{
    HelpDialog *dialog = new HelpDialog(this);
    dialog->exec();
}
