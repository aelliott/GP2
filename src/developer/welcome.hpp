#ifndef WELCOME_HPP
#define WELCOME_HPP

#include <QWidget>

namespace Ui {
    class Welcome;
}

namespace Developer {

class Welcome : public QWidget
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

signals:
    void newProjectClicked();
    void openProjectClicked();
    void openProjectClicked(QString);

private slots:
    void newProject();
    void openProject(QString path = QString());
    
private:
    Ui::Welcome *_ui;
};

}

#endif // WELCOME_HPP
