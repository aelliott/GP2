#ifndef EDIT_HPP
#define EDIT_HPP

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
    class Edit;
}

namespace Developer {

// Forward declaration
class Project;

/*!
 * \brief The Edit class provides a container for editing all three file types
 *
 * The files supported by this widget are rules, programs and graphs contained
 * in the currently open project.
 *
 * \todo Investigate extending this to include separate Macro definitions
 */
class Edit : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     * \brief Create a new Edit widget
     *
     * This does not completely set up the widget for use, setProject() should
     * be called before this widget is made active.
     *
     * \param parent This widget's parent widget - the MainWindow
     */
    explicit Edit(QWidget *parent = 0);

    /*!
     * \brief Destroy this widget and free memory
     */
    ~Edit();

    /*!
     * \brief Set this widget's project to the provided one and update child
     *  widgets
     * \param project   The project to use
     */
    void setProject(Project *project);

public slots:
    /*!
     * \brief Slot which handles one of the files being clicked in the tree
     *  widget
     *
     * The first thing this member function does is to determine what kind of
     * element has been clicked. The interesting nodes in the tree have as their
     * parent one of the file types (rule, program, graph). Each of these three
     * is handled separately and any without any of these conditions are
     * discarded.
     *
     * \param item  The tree item which has been clicked
     */
    void fileClicked(QTreeWidgetItem *item);

    /*!
     * \brief Slot to handle displaying the "injective matching" help page from
     *  the HelpDialog
     */
    void showInjectiveHelp();

    /*!
     * \brief Slot to handle the project's file list changing
     *
     * The tree is constructed with root nodes for each file type, then the list
     * of files under each type are appended to those nodes. The status of each
     * file affects how it should be displayed.
     */
    void fileListChanged();
    
private:
    Ui::Edit *_ui;
    Project *_project;
};

}

#endif // EDIT_HPP
