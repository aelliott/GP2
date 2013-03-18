#ifndef QUICKRUNWIDGET_HPP
#define QUICKRUNWIDGET_HPP

#include <QWidget>

namespace Ui {
class QuickRunWidget;
}

namespace Developer {

class Project;

class QuickRunWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QuickRunWidget(QWidget *parent = 0);
    ~QuickRunWidget();

    void setProject(Project *project);

public slots:
    void run();
    
private:
    Ui::QuickRunWidget *_ui;
    Project *_project;
};

}

#endif // QUICKRUNWIDGET_HPP
