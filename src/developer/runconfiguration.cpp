/*!
 * \file
 */
#include "runconfiguration.hpp"
#include "ui_runconfiguration.h"

#include "project.hpp"

#include <QFile>

namespace Developer {

RunConfiguration::RunConfiguration(Project *proj, QWidget *parent)
    : QWidget(parent)
    , _ui(new Ui::RunConfiguration)
    , _project(proj)
{
    _ui->setupUi(this);

    // Load the main stylesheet and apply it to this widget
    QFile fp(":/stylesheets/runconfiguration.css");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString style = fp.readAll();
    setStyleSheet(style);

    _ui->detailsWidget->setVisible(false);

    updatePrograms();
    updateGraphs();
}

RunConfiguration::~RunConfiguration()
{
    delete _ui;
}

void RunConfiguration::toggleDetails()
{
    _ui->detailsWidget->setVisible(!_ui->detailsWidget->isVisible());

    if(_ui->detailsWidget->isVisible())
        _ui->arrowImage->setPixmap(QPixmap(":/icons/bullet_arrow_down.png"));
    else
        _ui->arrowImage->setPixmap(QPixmap(":/icons/bullet_arrow_right.png"));
}

void RunConfiguration::updatePrograms()
{
    _ui->programCombo->clear();
    QVector<Program *> programs = _project->programs();
    for(QVector<Program *>::iterator iter = programs.begin();
        iter != programs.end(); ++iter)
    {
        Program *prog = *iter;
        if(prog == 0)
        {
            qDebug() << "Null pointer in RunConfiguration::updatePrograms(), "
                        "ignoring.";
        }
        _ui->programCombo->addItem(prog->name());
    }
}

void RunConfiguration::updateGraphs()
{
    _ui->targetGraphCombo->clear();
    QVector<Graph *> graphs = _project->graphs();
    for(QVector<Graph *>::iterator iter = graphs.begin();
        iter != graphs.end(); ++iter)
    {
        Graph *graph = *iter;
        if(graph == 0)
        {
            qDebug() << "Null pointer in RunConfiguration::updateGraphs(), "
                        "ignoring.";
        }
        _ui->targetGraphCombo->addItem(graph->fileName());
    }
}

}
