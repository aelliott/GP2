#ifndef PRETTYTABWIDGET_HPP
#define PRETTYTABWIDGET_HPP

#include <QWidget>
#include <QMap>
#include <QPair>

namespace Ui {
class PrettyTabWidget;
}

/*!
 * \brief The PrettyTabWidget class is a custom replacement for QTabWidget which
 *        provides an "East" positioned set of styled tabs with icons.
 *
 * This is not a complete drop-in replacement for QTabWidget and cannot be
 * moved to another context without alteration due to a lack of generalisation.
 * The reason for this is that QTabWidget is not amenable to icon/button type
 * styled tabs, and a custom approach is (in my view) preferable to pressing a
 * different UI element into service as a tab widget (as is the approach taken
 * by Qxt with a QTableView).
 *
 * If the desktop UI elements in Qt Quick were production-ready at this point it
 * would have made for a compelling alternative option as this type of UI is
 * easy to produce in Qt Quick - however since this is the only element which
 * needed special care I felt this was an easier option as opposed to packaging
 * and depending upon the desktop components in Qt's gitorious.
 */
class PrettyTabWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrettyTabWidget(QWidget *parent = 0);
    ~PrettyTabWidget();

    void setTabEnabled(const QString &menu, int index, bool enabled);
    void addMenu(const QString &menu);
    QPair<QString, int> addTab(QWidget *page, const QIcon &icon, const QString &label, const QString &menu = QString());

signals:
    void currentChanged(QString title);

public slots:
    void setCurrentIndex(QString menu, int index);

protected slots:
    void tabTriggered(QString menu, int index);
    
private:
    Ui::PrettyTabWidget *_ui;
    QMap<QPair<QString, int>, int> _pageMapping;
    QMap<QPair<QString, int>, QString> _nameMapping;
};

#endif // PRETTYTABWIDGET_HPP
