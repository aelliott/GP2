/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// We need OGDF's exceptions header to catch precondition errors
#include <ogdf/basic/basic.h>

// Include main page elements
#include "welcome.hpp"
#include "edit.hpp"
#include "run.hpp"
#include "results.hpp"
#include "graphview/graphwidget.hpp"
#include "graphview/graphscene.hpp"

// Include spawned dialogs
#include "firstrundialog.hpp"
#include "importprogramdialog.hpp"
#include "importruledialog.hpp"
#include "importgraphdialog.hpp"
#include "newprojectwizard.hpp"
#include "newgraphdialog.hpp"
#include "newprogramdialog.hpp"
#include "newruledialog.hpp"
#include "openprojectprogressdialog.hpp"
#include "preferences/preferencesdialog.hpp"
#include "helpdialog.hpp"
#include "aboutdialog.hpp"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QUndoStack>
#include <QtSvg/QSvgGenerator>

namespace Developer {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _activeProject(0)
    , _mapper(0)
    , _currentGraph(0)
    , _undoStack(0)
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

    QSettings settings;
    if(settings.value("FirstRun", true).toBool())
    {
        FirstRunDialog dialog(this);
        dialog.exec();
        settings.setValue("FirstRun", false);
    }
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
    _run->setProject(project);
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
        _ui->actionOpenProgram->setEnabled(true);
        _ui->actionOpenRule->setEnabled(true);
        _ui->actionOpenGraph->setEnabled(true);
        _ui->actionCloseProject->setEnabled(true);
        _ui->actionSaveAll->setEnabled(true);
        _ui->actionSelectAll->setEnabled(true);
        _ui->menuFindReplace->setEnabled(true);

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
        _ui->actionOpenProgram->setEnabled(false);
        _ui->actionOpenRule->setEnabled(false);
        _ui->actionOpenGraph->setEnabled(false);
        _ui->actionCloseProject->setEnabled(false);
        _ui->actionSaveAll->setEnabled(false);
        _ui->actionSelectAll->setEnabled(false);
        _ui->menuFindReplace->setEnabled(false);

        // If we're not in the welcome tab then move us there now
        _ui->tabWidget->setCurrentIndex("default", 0);
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

    Project *newProject = new Project(path);

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

    if(_activeProject != 0)
        delete _activeProject;

    _activeProject = newProject;
    connect(_activeProject, SIGNAL(currentFileChanged(GPFile*)),
            this, SLOT(currentFileChanged(GPFile*)));

    setProject(newProject);
    addRecentProject(_activeProject->absolutePath());
    setProjectActive(true);
}

void MainWindow::openProgram()
{
    ImportProgramDialog dialog(_activeProject, this);
    dialog.exec();
}

void MainWindow::openRule()
{
    ImportRuleDialog dialog(_activeProject, this);
    dialog.exec();
}

void MainWindow::openGraph()
{
    ImportGraphDialog dialog(_activeProject, this);
    dialog.exec();
}

void MainWindow::closeProject()
{
    setProject(0);
    _activeProject = 0;
    setProjectActive(false);
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

void MainWindow::setUndoAvailable(bool available)
{
    _ui->actionUndo->setEnabled(available);
}

void MainWindow::undo()
{
    // undo
}

void MainWindow::setRedoAvailable(bool available)
{
    _ui->actionRedo->setEnabled(available);
}

void MainWindow::redo()
{
    // redo
}

void MainWindow::setTextEditing(bool editing)
{
    _ui->actionPaste->setEnabled(editing);
}

void MainWindow::setTextSelected(bool selected)
{
    _ui->actionCut->setEnabled(selected);
    _ui->actionCopy->setEnabled(selected);
}

void MainWindow::cut()
{
    // cut
}

void MainWindow::copy()
{
    // copy
}

void MainWindow::paste()
{
    // paste
}

void MainWindow::selectAll()
{
    // select all
}

void MainWindow::findReplaceCurrentFile()
{
    // find replace in current file
}

void MainWindow::findReplaceProject()
{
    // find replace in this project
}

void MainWindow::layoutTreeTopToBottom()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutTree(Layout_TopToBottom);
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutTreeRightToLeft()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutTree(Layout_RightToLeft);
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutTreeBottomToTop()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutTree(Layout_BottomToTop);
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutTreeLeftToRight()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutTree(Layout_LeftToRight);
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutRadialTree()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutRadialTree();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutSugiyama()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutSugiyama();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutFPP()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutFPP();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutPlanarDraw()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutPlanarDraw();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutPlanarStraight()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutPlanarStraight();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutSchnyder()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutSchnyder();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutPlanarizationGrid()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutPlanarizationGrid();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutCircular()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutCircular();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutSpring()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutSpring();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutDavidsonHarel()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutDavidsonHarel();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutFMMM()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutFMMM();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::layoutGEM()
{
    if(_currentGraph == 0)
        return;

    try
    {
        _currentGraph->layoutGEM();
    }
    catch(ogdf::Exception e)
    {
        Q_UNUSED(e)
        QMessageBox::information(
                    this,
                    tr("Layout Failed"),
                    tr("The layout mechanism failed with a precondition error "
                       "error. Ensure that the layout mechanism selected is "
                       "appropriate for the provided graph."));
    }
}

void MainWindow::exportGraphToPng()
{
    if(_currentGraph == 0)
    {
        qDebug() << "Export graph requested with no active graph";
        return;
    }

    reinterpret_cast<GraphScene *>(_currentGraph->scene())->resizeToContents();
    QRect rect = _currentGraph->scene()->sceneRect().toRect();
    QPixmap image(rect.width(), rect.height());

    if(image.isNull())
    {
        QMessageBox::information(
                    this,
                    tr("Failed to Export Graph"),
                    tr("GP Developer could not export this graph as an image. "
                       "It may be too large for Qt's QPixmap to handle. The "
                       "graph file is %1x%2 pixels in size.").arg(
                        QVariant(rect.width()).toString(),
                        QVariant(rect.height()).toString()
                        ));
        return;
    }

    QPainter painter;
    painter.begin(&image);

    if(!painter.isActive())
    {
        QMessageBox::information(
                    this,
                    tr("Failed to Export Graph"),
                    tr("GP Developer could not export this graph as an image. "
                       "It may be too large for Qt's QPainter to handle. The "
                       "graph file is %1x%2 pixels in size.").arg(
                        QVariant(rect.width()).toString(),
                        QVariant(rect.height()).toString()
                        ));
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(Qt::white));
    painter.drawRect(rect);
    _currentGraph->scene()->render(&painter);
    painter.end();

    QString savePath = QFileDialog::getSaveFileName(
                this,
                tr("Save Graph as Image"),
                QDir::homePath(),
                tr("PNG Files (*.png)"));

    if(savePath.isEmpty())
        return;

    image.save(savePath);
}

void MainWindow::exportGraphToSvg()
{
    if(_currentGraph == 0)
    {
        qDebug() << "Export graph requested with no active graph";
        return;
    }

    reinterpret_cast<GraphScene *>(_currentGraph->scene())->resizeToContents();
    QRect rect = _currentGraph->scene()->sceneRect().toRect();

    QSvgGenerator generator;
    QString savePath = QFileDialog::getSaveFileName(
                this,
                tr("Save Graph as Image"),
                QDir::homePath(),
                tr("SVG Files (*.svg)"));

    if(savePath.isEmpty())
        return;
    generator.setFileName(savePath);
    generator.setSize(QSize(rect.width(), rect.height()));
    generator.setViewBox(QRect(0, 0, rect.width(), rect.height()));
    generator.setTitle("Graph Export - GP Developer");
    generator.setDescription(tr("Export from GP Developer's graph view"));

    QPainter painter;
    painter.begin(&generator);

    if(!painter.isActive())
    {
        QMessageBox::information(
                    this,
                    tr("Failed to Export Graph"),
                    tr("GP Developer could not export this graph as an image. "
                       "It may be too large for Qt's QPainter to handle. The "
                       "graph file is %1x%2 pixels in size.").arg(
                        QVariant(rect.width()).toString(),
                        QVariant(rect.height()).toString()
                        ));
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(Qt::white));
    painter.drawRect(rect);
    _currentGraph->scene()->render(&painter);
    painter.end();
}

void MainWindow::exportGraphToDot()
{
    if(_currentGraph == 0)
    {
        qDebug() << "Export graph requested with no active graph";
        return;
    }

    reinterpret_cast<GraphScene *>(
                _currentGraph->scene()
                )->graph()->exportTo(QString(), DotGraph);
}

void MainWindow::exportGraphToGxl()
{
    if(_currentGraph == 0)
    {
        qDebug() << "Export graph requested with no active graph";
        return;
    }

    reinterpret_cast<GraphScene *>(
                _currentGraph->scene()
                )->graph()->exportTo(QString(), GxlGraph);
}

void MainWindow::exportGraphToLaTeX()
{
    if(_currentGraph == 0)
    {
        qDebug() << "Export graph requested with no active graph";
        return;
    }

    reinterpret_cast<GraphScene *>(
                _currentGraph->scene()
                )->graph()->exportTo(QString(), LaTeXGraph);
}

void MainWindow::showPreferences()
{
    PreferencesDialog *dialog = new PreferencesDialog(this);
    dialog->exec();
}

void MainWindow::showFirstRunDialog()
{
    FirstRunDialog dialog(this);
    dialog.exec();
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
    _ui->actionReplaceInCurrentFile->setEnabled(true);
}

void MainWindow::graphHasFocus(GraphWidget *graphWidget)
{
    _currentGraph = graphWidget;
    _ui->menuLayout->setEnabled(true);
    _ui->menuExport->setEnabled(true);
}

void MainWindow::graphLostFocus(GraphWidget *graphWidget)
{
    if(_currentGraph == graphWidget)
    {
        // Commented for now, keep the last one we have received
        //_currentGraph = 0;
        //_ui->menuLayout->setEnabled(false);
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
