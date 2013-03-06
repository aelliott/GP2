#ifndef PRETTYTABBAR_HPP
#define PRETTYTABBAR_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QMap>

#include "prettytabmenu.hpp"

/*!
 * \brief The PrettyTabBar class is a subclass of PrettyTabWidget which follows
 *        the established pattern of QTabWidget -> QTabBar
 *
 * This class provides a "pretty" button-based tab bar for use as a vertically
 * aligned bar connected to a QStackedWidget. It is not fully generalised and as
 * such will not work without modification if taken out of context.
 *
 * The tab bar is organised into sub-menus, there is a default menu which will
 * appear first, followed by any user-defined sub-menus. For example:
 *
 * (default):
 *  - Installed
 *  - Mirror
 *  - ChangeLog
 * Plugins:
 *  - SlackBuilds.org
 */
class PrettyTabBar : public QWidget
{
    Q_OBJECT
public:
    explicit PrettyTabBar(QWidget *parent = 0);

    PrettyTab *tab(const QString &name, int index) const;

    void addMenu(const QString &name);
    QPair<QString, int> addTab(const QString &name, const QIcon &icon, const QString &menu = QString());

    bool containsMenu(QString menu) const;
    
signals:
    void currentChanged(QString menu, int index);
    
public slots:
    void setCurrentIndex(QString menu, int index);

protected slots:
    void tabTriggered(QString menu, int index);

private:
    QVBoxLayout *_layout;
    QMap<QString, PrettyTabMenu *> _menus;
    int _menuCount;
};

#endif // PRETTYTABBAR_HPP
