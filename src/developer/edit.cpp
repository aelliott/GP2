#include "edit.hpp"
#include "ui_edit.h"

#include <QFile>
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
}

Edit::~Edit()
{
    delete _ui;
}

void Edit::setProject(Project *project)
{
    // Handle this!
    _project = project;
    connect(_project, SIGNAL(fileListChanged()), this, SLOT(fileListChanged()));
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
    }

    // Handle a clicked program
    if(parent->text(0) == tr("Programs"))
    {
        _ui->stackedWidget->setCurrentIndex(1);
    }

    // Handle a clicked graph
    if(parent->text(0) == tr("Graphs"))
    {
        _ui->stackedWidget->setCurrentIndex(2);
    }
}

void Edit::showInjectiveHelp()
{
    HelpDialog *dialog = new HelpDialog(HelpDialog::Injective, this);
    dialog->exec();
}

void Edit::fileListChanged()
{
    // Clear the tree of existing items
    _ui->projectTreeWidget->clear();

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
        default:
            // Do nothing
            break;
        }
        rules->addChild(item);
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
        default:
            // Do nothing
            break;
        }
        programs->addChild(item);
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
        default:
            // Do nothing
            break;
        }
        graphs->addChild(item);
    }
}

}
