/*!
 * \file
 */
#ifndef RUNCONFIGURATION_HPP
#define RUNCONFIGURATION_HPP

#include <QWidget>

namespace Ui {
    class RunConfiguration;
}

namespace Developer {

class Project;

class RunConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit RunConfiguration(Project *proj, QWidget *parent = 0);
    ~RunConfiguration();

public slots:
    void toggleDetails();

    void updatePrograms();
    void updateGraphs();
    
private:
    Ui::RunConfiguration *_ui;
    Project *_project;
};

}

#endif // RUNCONFIGURATION_HPP
