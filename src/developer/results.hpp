/*!
 * \file
 */
#ifndef RESULTS_HPP
#define RESULTS_HPP

#include <QWidget>

namespace Ui {
    class Results;
}

namespace Developer {

class Results : public QWidget
{
    Q_OBJECT
    
public:
    explicit Results(QWidget *parent = 0);
    ~Results();
    
private:
    Ui::Results *_ui;
};

}

#endif // RESULTS_HPP
