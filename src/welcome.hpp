#ifndef WELCOME_HPP
#define WELCOME_HPP

#include <QWidget>

namespace Ui {
    class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT
    
public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();
    
private:
    Ui::Welcome *_ui;
};

#endif // WELCOME_HPP
