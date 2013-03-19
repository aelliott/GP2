/*!
 * \file
 */
#ifndef RUN_HPP
#define RUN_HPP

#include <QWidget>
#include "../libgp/errors.h"

namespace Ui {
    class Run;
}

namespace Developer {

class Run : public QWidget
{
    Q_OBJECT
    
public:
    explicit Run(QWidget *parent = 0);
    ~Run();
    
private:
    Ui::Run *_ui;
};

}

#endif // RUN_HPP
