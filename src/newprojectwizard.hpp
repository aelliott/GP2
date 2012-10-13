#ifndef NEWPROJECTWIZARD_HPP
#define NEWPROJECTWIZARD_HPP

#include <QWizard>
#include "project.hpp"

namespace Ui {
    class NewProjectWizard;
}

class NewProjectWizard : public QWizard
{
    Q_OBJECT
    
public:
    explicit NewProjectWizard(QWidget *parent = 0);
    ~NewProjectWizard();

    Project *project() const;

public slots:
    void updateProjectLocation();
    void accept();
    
private:
    Ui::NewProjectWizard *_ui;
    Project *_project;
};

#endif // NEWPROJECTWIZARD_HPP
