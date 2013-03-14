#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "project.hpp"

namespace Ui {
    class MainWindow;
}

namespace Developer {

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void restoreWindowDimensions();

public slots:
    void setProjectActive(bool state);

    void newProject();
    void openProject(QString path = QString());

    void closeEvent(QCloseEvent *event);

private slots:
    void showPreferences();
    void showApplicationHelp();
    void showApplicationAbout();

private:
    Ui::MainWindow *_ui;
    Project *_activeProject;
};

}

#endif // MAINWINDOW_HPP
