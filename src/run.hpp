#ifndef RUN_HPP
#define RUN_HPP

#include <QWidget>

namespace Ui {
    class Run;
}

class Run : public QWidget
{
    Q_OBJECT
    
public:
    explicit Run(QWidget *parent = 0);
    ~Run();
    
private:
    Ui::Run *_ui;
};

#endif // RUN_HPP
