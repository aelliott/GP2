#ifndef EDIT_HPP
#define EDIT_HPP

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class Edit;
}

namespace Developer {

/*!
 * \brief The Edit class provides a container for editing all three file types
 *
 * The files supported by this widget are rules, programs and graphs contained
 * in the currently open project.
 */
class Edit : public QWidget
{
    Q_OBJECT
    
public:
    explicit Edit(QWidget *parent = 0);
    ~Edit();

public slots:
    void fileClicked(QTreeWidgetItem *item);

    void showInjectiveHelp();
    
private:
    Ui::Edit *_ui;
};

}

#endif // EDIT_HPP
