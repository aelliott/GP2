/*!
 * \file
 */
#include "edit.hpp"
#include "ui_edit.h"

#include "helpdialog.hpp"
#include "project.hpp"

namespace Developer {

Edit::Edit(QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::Edit)
    , _project(0)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/main.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    connect(_ui->graphEdit, SIGNAL(graphHasFocus(GraphWidget*)),
            this, SLOT(handleGraphHasFocus(GraphWidget*)));
    connect(_ui->graphEdit, SIGNAL(graphLostFocus(GraphWidget*)),
            this, SLOT(handleGraphLostFocus(GraphWidget*)));
}

Edit::~Edit()
{
    delete _ui;
}

void Edit::setProject(Project *project)
{
    if(project == 0)
    {
        qDebug() << "Edit::setProject() handed a null pointer... not pleased";
        return;
    }

    // Handle this!
    _project = project;
    //_ui->ruleEdit->setRule();
    if(_project->programs().count() > 0)
    {
        _ui->programEdit->setEnabled(true);
        _ui->programEdit->setProgram(_project->programs().at(0));
        _ui->graphEdit->setGraph(_project->graphs().at(0));
    }
    else
        _ui->programEdit->setEnabled(false);
    connect(_project, SIGNAL(fileListChanged()), this, SLOT(fileListChanged()));
    connect(_project, SIGNAL(fileStatusChanged(QString,int)),
            this, SLOT(fileStatusChanged(QString,int))
            );
    fileListChanged();
}

void Edit::fileClicked(QTreeWidgetItem *item)
{
    QTreeWidgetItem *parent = item->parent();
    if(parent == 0)
        return;

    // Handle a clicked rule
    if(parent->text(0) == tr("Rules"))
    {
        _ui->stackedWidget->setCurrentIndex(0);
        _project->setCurrentFile(item->text(0), Project::RuleFile);
    }

    // Handle a clicked program
    if(parent->text(0) == tr("Programs"))
    {
        _ui->stackedWidget->setCurrentIndex(1);
        _ui->programEdit->setProgram(_project->program(item->text(0)));
        _project->setCurrentFile(item->text(0), Project::ProgramFile);
    }

    // Handle a clicked graph
    if(parent->text(0) == tr("Graphs"))
    {
        _ui->stackedWidget->setCurrentIndex(2);
        _ui->graphEdit->setGraph(_project->graph(item->text(0)));
        _project->setCurrentFile(item->text(0), Project::GraphFile);
    }
}

void Edit::fileListChanged()
{
    // Clear the tree of existing items
    _ui->projectTreeWidget->clear();
    _treeMap.clear();

    // Add the root node with the project name
    QStringList items;
    items << _project->name();
    QTreeWidgetItem *root = new QTreeWidgetItem(items);
    root->setIcon(0, QIcon(QPixmap(":/icons/application-icon.png")));
    _ui->projectTreeWidget->addTopLevelItem(root);

    // Loop over rules adding each to the list
    items.clear(); items << tr("Rules");
    QTreeWidgetItem *rules = new QTreeWidgetItem(items);
    rules->setIcon(0, QIcon(QPixmap(":/icons/small_folder_brick.png")));
    root->addChild(rules);

    QVector<Rule *> projectRules = _project->rules();
    for(QVector<Rule *>::iterator iter = projectRules.begin();
        iter != projectRules.end(); ++iter)
    {
        Rule *r = *iter;
        items.clear(); items << r->name();
        QTreeWidgetItem *item = new QTreeWidgetItem(items);
        item->setToolTip(0, r->absolutePath());
        switch(r->status())
        {
        case GPFile::Modified:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_save.png")));
            break;
        case GPFile::ExternallyModified:
        case GPFile::Error:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_error.png")));
            break;
        case GPFile::Deleted:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_delete.png")));
            break;
        case GPFile::Normal:
            item->setIcon(0, QIcon());
            break;
        default:
            // Do nothing
            break;
        }
        rules->addChild(item);
        _treeMap.insert(r, item);
    }

    // Loop over programs adding each to the list
    items.clear(); items << tr("Programs");
    QTreeWidgetItem *programs = new QTreeWidgetItem(items);
    programs->setIcon(0, QIcon(QPixmap(":/icons/small_folder_page.png")));
    root->addChild(programs);

    QVector<Program *> projectPrograms = _project->programs();
    for(QVector<Program *>::iterator iter = projectPrograms.begin();
        iter != projectPrograms.end(); ++iter)
    {
        Program *p = *iter;
        items.clear(); items << p->name();
        QTreeWidgetItem *item = new QTreeWidgetItem(items);
        item->setToolTip(0, p->absolutePath());
        switch(p->status())
        {
        case GPFile::Modified:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_save.png")));
            break;
        case GPFile::ExternallyModified:
        case GPFile::Error:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_error.png")));
            break;
        case GPFile::Deleted:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_delete.png")));
            break;
        case GPFile::Normal:
            item->setIcon(0, QIcon());
            break;
        default:
            // Do nothing
            break;
        }
        programs->addChild(item);
        _treeMap.insert(p, item);
    }

    // Loop over graphs adding each to the list
    items.clear(); items << tr("Graphs");
    QTreeWidgetItem *graphs = new QTreeWidgetItem(items);
    graphs->setIcon(0, QIcon(QPixmap(":/icons/small_folder.png")));
    root->addChild(graphs);

    QVector<Graph *> projectGraphs = _project->graphs();
    for(QVector<Graph *>::iterator iter = projectGraphs.begin();
        iter != projectGraphs.end(); ++iter)
    {
        Graph *g = *iter;
        items.clear(); items << g->fileName();
        QTreeWidgetItem *item = new QTreeWidgetItem(items);
        item->setToolTip(0, g->absolutePath());
        switch(g->status())
        {
        case GPFile::Modified:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_save.png")));
            break;
        case GPFile::ExternallyModified:
        case GPFile::Error:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_error.png")));
            break;
        case GPFile::Deleted:
            item->setIcon(0, QIcon(QPixmap(":/icons/small_page_delete.png")));
            break;
        case GPFile::Normal:
            item->setIcon(0, QIcon());
            break;
        default:
            // Do nothing
            break;
        }
        graphs->addChild(item);
        _treeMap.insert(g, item);
    }

    _ui->projectTreeWidget->expandAll();
}

void Edit::fileStatusChanged(QString path, int status)
{
    if(!_treeMap.contains(_project->file(path)))
    {
        qDebug() << "Edit::fileStatusChanged called but could not locate file.";
        qDebug() << "path = " << path;
        qDebug() << "status = " << status;
        return;
    }

    QTreeWidgetItem *item = _treeMap[_project->file(path)];
    if(item == 0)
        return;

    switch(status)
    {
    case GPFile::Modified:
        item->setIcon(0, QIcon(QPixmap(":/icons/small_page_save.png")));
        break;
    case GPFile::ExternallyModified:
    case GPFile::Error:
        item->setIcon(0, QIcon(QPixmap(":/icons/small_page_error.png")));
        break;
    case GPFile::Deleted:
        item->setIcon(0, QIcon(QPixmap(":/icons/small_page_delete.png")));
        break;
    case GPFile::Normal:
        item->setIcon(0, QIcon());
        break;
    default:
        // Do nothing
        break;
    }
}

void Edit::handleGraphHasFocus(GraphWidget *graphWidget)
{
    emit graphHasFocus(graphWidget);
}

void Edit::handleGraphLostFocus(GraphWidget *graphWidget)
{
    emit graphLostFocus(graphWidget);
}

}
