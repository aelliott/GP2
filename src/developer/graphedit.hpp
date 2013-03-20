/*!
 * \file
 */
#ifndef GRAPHEDIT_HPP
#define GRAPHEDIT_HPP

#include <QWidget>

namespace Ui {
class GraphEdit;
}

namespace Developer {

class GraphEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit GraphEdit(QWidget *parent = 0);
    ~GraphEdit();
    
private:
    Ui::GraphEdit *_ui;
};

}

#endif // GRAPHEDIT_HPP
