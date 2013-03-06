#ifndef PRETTYTAB_HPP
#define PRETTYTAB_HPP

#include <QWidget>
#include <QString>
#include <QIcon>

/*!
 * \brief The PrettyTab class represents one tab in the PrettyTabBar
 *
 * This is a UI element which is primarily involved with drawing itself onto
 * the screen within its custom paintEvent(). It also handles user interaction
 * such as mouse-over, mouse-out and mouse clicks.
 *
 * The tab is either in a "selected" or "unselected" state, it becomes
 * "selected" when it is clicked and emits a pressed() signal. It becomes
 * "unselected" when informed that another tab has focus via its
 * clearSelection() method.
 *
 * The setTop() method is a hack to prevent a double-border situation which
 * occurs because the top item is adjacent to the title bar which has the
 * responsibility for its own bottom border. The top tab should not add a second
 * 1px border, and should skip that part of its paint event.
 */
class PrettyTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrettyTab(const QString &label, const QIcon &icon, QWidget *parent = 0);
    ~PrettyTab();

    void setEnabled(bool enabled);
    void setSelected(bool selected);
    void setTop(bool top);
    void clearSelection();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

    QSize sizeHint() const;

signals:
    void pressed();
    
private:
    QString _label;
    QIcon _icon;

    int _iconHeight;
    int _iconWidth;

    int _topMargin;
    int _rightMargin;
    int _bottomMargin;
    int _leftMargin;
    int _itemSpacing;

    bool _enabled;
    bool _mouseOver;
    bool _active;
    bool _top;
};

#endif // PRETTYTAB_HPP
