#ifndef RUNCONFIGURATION_HPP
#define RUNCONFIGURATION_HPP

#include <QWidget>

namespace Ui {
    class RunConfiguration;
}

class RunConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit RunConfiguration(QWidget *parent = 0);
    ~RunConfiguration();

public slots:
    void toggleDetails();
    
private:
    Ui::RunConfiguration *_ui;
};

#endif // RUNCONFIGURATION_HPP
