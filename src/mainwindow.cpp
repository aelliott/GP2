#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// Include main page elements
#include "welcome.hpp"
#include "edit.hpp"
#include "run.hpp"
#include "results.hpp"

// Include spawned dialogs
#include "newprojectwizard.hpp"
#include "preferencesdialog.hpp"
#include "helpdialog.hpp"
#include "aboutdialog.hpp"

#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    // Hide the quick run dialog until a project is open
    _ui->quickRunWidget->setVisible(false);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    Welcome *welcome = new Welcome(this);
    connect(welcome, SIGNAL(newProjectClicked()), this, SLOT(newProject()));
    connect(welcome, SIGNAL(openProjectClicked()), this, SLOT(openProject()));
    connect(welcome, SIGNAL(openProjectClicked(QString)), this, SLOT(openProject(QString)));
    _ui->tabWidget->addTab(welcome,
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

    _ui->tabWidget->addTab(new Results(this),
                           QIcon(QPixmap(":/icons/results.png")),
                           tr("Results")
                           );

    // All tabs apart from "Welcome" should only become active one a project is
    // created or opened.
    _ui->tabWidget->setTabEnabled("default", 1, false);
    _ui->tabWidget->setTabEnabled("default", 2, false);
    _ui->tabWidget->setTabEnabled("default", 3, false);

    statusBar()->showMessage(tr("Ready."));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::newProject()
{
    NewProjectWizard *wizard = new NewProjectWizard(this);
    wizard->exec();
}

void MainWindow::openProject(QString path)
{

}

void MainWindow::showPreferences()
{
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->exec();
}

void MainWindow::showApplicationHelp()
{
    HelpDialog *dialog = new HelpDialog(this);
    dialog->exec();
}

void MainWindow::showApplicationAbout()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->exec();
}
