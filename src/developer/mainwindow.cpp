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
#include "graphview/graphwidget.hpp"

// Include spawned dialogs
#include "newprojectwizard.hpp"
#include "newgraphdialog.hpp"
#include "newprogramdialog.hpp"
#include "newruledialog.hpp"
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
    , _mapper(0)
    , _currentGraph(0)
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
    connect(welcome, SIGNAL(openProjectClicked(QString)),
            this, SLOT(openProject(QString)));
    connect(this, SIGNAL(recentProjectsChanged(QStringList)),
            welcome, SLOT(recentProjectsUpdated(QStringList)));
    _ui->tabWidget->addTab(welcome,
                           QIcon(QPixmap(":/icons/application-icon.png")),
                           tr("Welcome")
                           );

    _edit = new Edit(this);
    _ui->tabWidget->addTab(_edit,
                           QIcon(QPixmap(":/icons/edit.png")),
                           tr("Edit")
                           );
    connect(_edit, SIGNAL(graphHasFocus(GraphWidget*)),
            this, SLOT(graphHasFocus(GraphWidget*)));
    connect(_edit, SIGNAL(graphLostFocus(GraphWidget*)),
            this, SLOT(graphLostFocus(GraphWidget*)));

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
    updateRecentProjects();
}

MainWindow::~MainWindow()
{
    delete _ui;
}

QStringList MainWindow::recentProjects() const
{
    return _recentProjects;
}

void MainWindow::updateRecentProjects()
{
    QSettings settings;
    _recentProjects = settings.value(
                "Projects/RecentProjects",
                QStringList()
                ).toStringList();
    QStringList tmp;

    // Loop across the list, ensure that all of the listed projects exist. If
    // any cannot be located then don't list them.
    for(QStringList::iterator iter = _recentProjects.begin();
        iter != _recentProjects.end(); ++iter)
    {
        QString project = *iter;
        QFileInfo info(project);
        if(info.exists())
            tmp << project;
    }

    //! \todo Decide on the maximum size of this list and pop items from the
    //!     back

    if(tmp.count() < 1)
    {
        // There are no recent projects, disable the widgets which require them
        _ui->menuRecentProjects->setEnabled(false);
    }
    else
    {
        // There are recent projects, enable the widgets which require them and
        // populate them with the list contents
        if(_mapper != 0)
            delete _mapper;
        _mapper = new QSignalMapper(this);
        _ui->menuRecentProjects->setEnabled(true);

        _ui->menuRecentProjects->clear();
        for(QStringList::iterator iter = tmp.begin(); iter != tmp.end(); ++iter)
        {
            Project *proj = new Project(*iter, false);
            if(proj->name().isEmpty())
                continue;

            QAction *action = new QAction(proj->absolutePath(), _ui->menuRecentProjects);
            connect(action, SIGNAL(triggered()), _mapper, SLOT(map()));
            _mapper->setMapping(action, proj->absolutePath());

            delete proj;
            _ui->menuRecentProjects->addAction(action);
        }

        connect(_mapper, SIGNAL(mapped(QString)),
                this, SLOT(openProject(QString)));
    }

    _recentProjects = tmp;

    emit recentProjectsChanged(_recentProjects);
}

void MainWindow::addRecentProject(QString project)
{
    QSettings settings;

    if(_recentProjects.contains(project))
        _recentProjects.removeOne(project);

    _recentProjects.push_front(project);
    _recentProjects.removeDuplicates();

    while(_recentProjects.count() > MAX_RECENT_PROJECTS)
        _recentProjects.pop_back();

    settings.setValue("Projects/RecentProjects", _recentProjects);
    // The below is very dangerous memory-wise, it causes the set of buttons
    // and menu actions relating to recent projects to be destroyed, but those
    // items are usually what triggered this in the first place and deleting
    // themselves while they're working causes a segmentation fault (bad access)
    //updateRecentProjects();
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
    if(project == 0)
        return;

    connect(project, SIGNAL(fileChanged(QString)),
            this, SLOT(projectChanged()));
    connect(project, SIGNAL(fileListChanged()),
            this, SLOT(projectChanged()));
    connect(project, SIGNAL(fileStatusChanged(QString,int)),
            this, SLOT(projectChanged()));

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

        // Enable elements of the drop-down menus
        _ui->actionNewGraph->setEnabled(true);
        _ui->actionNewProgram->setEnabled(true);
        _ui->actionNewRule->setEnabled(true);
        _ui->actionOpenGraph->setEnabled(true);
        _ui->actionSaveAll->setEnabled(true);

        // Move us into the edit tab if we're in the welcome tab.
        if(_ui->tabWidget->currentTab().second == 0)
            _ui->tabWidget->setCurrentIndex("default", 1);
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

        // Disable elements of the drop-down menus
        _ui->actionNewGraph->setEnabled(false);
        _ui->actionNewProgram->setEnabled(false);
        _ui->actionNewRule->setEnabled(false);
        _ui->actionOpenGraph->setEnabled(false);
        _ui->actionSaveAll->setEnabled(false);
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
        connect(_activeProject, SIGNAL(currentFileChanged(GPFile*)),
                this, SLOT(currentFileChanged(GPFile*)));
        setProjectActive(true);
        addRecentProject(_activeProject->absolutePath());
    }
}

void MainWindow::newGraph()
{
    if(_activeProject == 0)
        return;

    NewGraphDialog dialog(_activeProject, this);
    dialog.exec();
}

void MainWindow::newProgram()
{
    if(_activeProject == 0)
        return;

    NewProgramDialog dialog(_activeProject, this);
    dialog.exec();
}

void MainWindow::newRule()
{
    if(_activeProject == 0)
        return;

    NewRuleDialog dialog(_activeProject, this);
    dialog.exec();
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
    connect(_activeProject, SIGNAL(currentFileChanged(GPFile*)),
            this, SLOT(currentFileChanged(GPFile*)));
    addRecentProject(_activeProject->absolutePath());
    setProjectActive(true);
}

void MainWindow::save()
{
    _activeProject->saveCurrentFile();
}

void MainWindow::saveAs()
{
    _activeProject->saveCurrentFileAs();
}

void MainWindow::saveAll()
{
    _activeProject->saveAll();
}

void MainWindow::layoutSugiyama()
{
    if(_currentGraph == 0)
        return;

    _currentGraph->layoutSugiyama();
}

void MainWindow::layoutCircular()
{
    if(_currentGraph == 0)
        return;

    _currentGraph->layoutCircular();
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

void MainWindow::projectChanged()
{
    // Stub for handling a project alteration if such is required
}

void MainWindow::currentFileChanged(GPFile *f)
{
    _ui->actionSaveCurrentFile->setEnabled(true);
    _ui->actionSaveCurrentFileAs->setEnabled(true);
    _ui->actionSaveCurrentFile->setText(tr("Save '%1'").arg(f->fileName()));
    _ui->actionSaveCurrentFileAs->setText(tr("Save '%1' As...").arg(
                                              f->fileName()));
}

void MainWindow::graphHasFocus(GraphWidget *graphWidget)
{
    _currentGraph = graphWidget;
    _ui->menuLayout->setEnabled(true);
}

void MainWindow::graphLostFocus(GraphWidget *graphWidget)
{
    if(_currentGraph == graphWidget)
    {
        _currentGraph = 0;
        _ui->menuLayout->setEnabled(false);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // If there are unsaved changes, force the user to confirm that they wish
    // to leave.
    if(_activeProject != 0 && _activeProject->hasUnsavedChanges())
    {
        QMessageBox::StandardButton response;
        response = QMessageBox::warning(
                    0,
                    tr("Unsaved Changes"),
                    tr("The currently open project has unsaved changes. Are you "
                       "sure you want to discard them?"),
                    QMessageBox::Yes | QMessageBox::Cancel
                    );
        // The user hit cancel, bail without changing anything
        if(response != QMessageBox::Yes)
        {
            event->ignore();
            return;
        }
    }

    event->accept();
    QSettings settings;

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("isMaximised", isMaximized());
    settings.endGroup();
}

}
