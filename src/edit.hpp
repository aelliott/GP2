#ifndef EDIT_HPP
#define EDIT_HPP

#include <QWidget>

namespace Ui {
    class Edit;
}

class Edit : public QWidget
{
    Q_OBJECT
    
public:
    explicit Edit(QWidget *parent = 0);
    ~Edit();
    
private:
    Ui::Edit *_ui;
};

#endif // EDIT_HPP
