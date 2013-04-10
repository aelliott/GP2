/*!
 * \file
 */
#ifndef OPENPROJECTPROGRESSDIALOG_HPP
#define OPENPROJECTPROGRESSDIALOG_HPP

#include <QDialog>

namespace Ui {
class OpenProjectProgressDialog;
}

namespace Developer {

class Project;

class OpenProjectProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OpenProjectProgressDialog(Project *project = 0, QWidget *parent = 0);
    ~OpenProjectProgressDialog();

public slots:
    void setGraphFiles();
    void setRuleFiles();
    void setProgramFiles();

    void setNodes(int count);
    void setEdges(int count);
    
private:
    Ui::OpenProjectProgressDialog *_ui;
    Project *_project;
};

}

#endif // OPENPROJECTPROGRESSDIALOG_HPP
