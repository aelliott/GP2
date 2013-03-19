/*!
 * \file
 */
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// Include main page elements
#include "welcome.hpp"
#include "edit.hpp"
#include "run.hpp"
#include "results.hpp"

// Include spawned dialogs
#include "newprojectwizard.hpp"
#include "preferences/preferencesdialog.hpp"
#include "helpdialog.hpp"
#include "aboutdialog.hpp"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

namespace Developer {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _activeProject(0)
{
    _ui->setupUi(this);

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

    _edit = new Edit(this);
    _ui->tabWidget->addTab(_edit,
                           QIcon(QPixmap(":/icons/edit.png")),
                           tr("Edit")
                           );

    _run = new Run(this);
    _ui->tabWidget->addTab(_run,
                           QIcon(QPixmap(":/icons/run.png")),
                           tr("Run")
                           );

    _results = new Results(this);
    _ui->tabWidget->addTab(_results,
                           QIcon(QPixmap(":/icons/results.png")),
                           tr("Results")
                           );

    // No open project by default, so set that state
    setProjectActive(false);

    statusBar()->showMessage(tr("Ready."));

    restoreWindowDimensions();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::restoreWindowDimensions()
{
    QSettings settings;

    settings.beginGroup("MainWindow");

    // If the window is maximised, then the position and size are already known
    // and we can safely ignore them
    if(settings.value("isMaximised", QVariant(false)).toBool())
        showMaximized();
    else
    {
        QSize size = settings.value("size", QSize()).toSize();
        if(!size.isNull() && size.width() > 0 && size.height() > 0)
            resize(size);
        QPoint point = settings.value("pos", QPoint()).toPoint();
        if(!point.isNull())
            move(point);
    }

    settings.endGroup();
}

void MainWindow::setProject(Project *project)
{
    _ui->title->setText(QString("GP Developer - ") + project->name());
    _ui->quickRunWidget->setProject(project);
    _edit->setProject(project);
}

void MainWindow::setProjectActive(bool state)
{
    if(state)
    {
        // Show the quick run widget
        _ui->quickRunWidget->setVisible(true);

        // Enable all tabs
        _ui->tabWidget->setTabEnabled("default", 1, true);
        _ui->tabWidget->setTabEnabled("default", 2, true);
        _ui->tabWidget->setTabEnabled("default", 3, true);
    }
    else
    {
        // Hide the quick run widget until a project is open
        _ui->quickRunWidget->setVisible(false);

        // All tabs apart from "Welcome" should only become active once a project is
        // created or opened.
        _ui->tabWidget->setTabEnabled("default", 1, false);
        _ui->tabWidget->setTabEnabled("default", 2, false);
        _ui->tabWidget->setTabEnabled("default", 3, false);
    }
}

void MainWindow::newProject()
{
    NewProjectWizard *wizard = new NewProjectWizard(this);
    wizard->exec();

    if(wizard->project() != 0)
    {
        setProject(wizard->project());

        if(_activeProject != 0)
        {
            // This might cause some grief later on
            //! \todo Review whether this still makes sense or if it will leave too
            //!     many dangling pointers to be worth it
            delete _activeProject;
        }

        _activeProject = wizard->project();
        setProjectActive(true);
    }
}

void MainWindow::openProject(QString path)
{
    QSettings settings;
    QString defaultPath = settings.value(
                "Projects/DefaultProjectLocation",
                QVariant(QDir::toNativeSeparators(
                             QDir::homePath()
                             ))
                ).toString();

    if(path.isEmpty())
    {
        path = QFileDialog::getOpenFileName(
                    this,
                    tr("Open GP Project"),
                    defaultPath,
                    "GP Project Files (*.gpp)"
                    );
        if(path.isEmpty())
            return;
    }

    QFile f(path);
    if(!f.exists())
        return;

    Project *newProject = new Project(path, this);
    if(newProject->isNull())
    {
        QMessageBox::warning(
                    this,
                    tr("Failed to Open Project"),
                    tr("GP Developer was unable to open the specified project"
                       "file (%1). The error returned from the system was: %2"
                       ).arg(
                        path,
                        newProject->error()
                        ),
                    QMessageBox::Ok
                    );
        // Throw it away, it's useless
        delete newProject;
        return;
    }

    setProject(newProject);

    if(_activeProject != 0)
    {
        // As above ^ (see \todo)
        delete _activeProject;
    }

    _activeProject = newProject;
    setProjectActive(true);
}

void MainWindow::showPreferences()
{
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->exec();
}

void MainWindow::showApplicationHelp()
{
    HelpDialog *dialog = new HelpDialog(HelpDialog::Introduction, this);
    dialog->exec();
}

void MainWindow::showApplicationAbout()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Initially we're going to assume that the user knows what they are doing
    // but this should probably have a dialog asking for confirmation in the
    // case where there are changes which have not been saved.
    event->accept();
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("isMaximised", isMaximized());
    settings.endGroup();
}

}
