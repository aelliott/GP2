#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setProjectActive(bool state);

    void newProject();
    void openProject(QString path = QString());

private slots:
    void showPreferences();
    void showApplicationHelp();
    void showApplicationAbout();

private:
    Ui::MainWindow *_ui;
};

#endif // MAINWINDOW_HPP
