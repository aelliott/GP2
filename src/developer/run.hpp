/*!
 * \file
 */
#ifndef RUN_HPP
#define RUN_HPP

#include <QWidget>
#include "../libgp/errors.h"

namespace Ui {
    class Run;
}

namespace Developer {

class Project;
class RunConfiguration;

class Run : public QWidget
{
    Q_OBJECT
    
public:
    explicit Run(QWidget *parent = 0);
    ~Run();

    Project *project() const;

    void setProject(Project *proj);

public slots:
    RunConfiguration *addRunConfiguration(bool addToProject = true);
    
private:
    Ui::Run *_ui;
    Project *_project;
    bool _initial;
};

}

#endif // RUN_HPP
