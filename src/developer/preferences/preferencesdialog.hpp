/*!
 * \file
 * \author Alex Elliott
 *
 * \section LICENSE
 * This file is part of GP Developer.
 *
 * GP Developer is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * GP Developer is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * GP Developer.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

class QAbstractButton;

namespace Ui {
    class PreferencesDialog;
}

namespace Developer {

class PreferencesPage;

/*!
 * \brief The PreferencesDialog class provides an interface to change any of the
 *  settings in the application
 *
 * Preferences are grouped into "pages" such as build options or visualisation
 * appearance. These pages are presented in a PrettyTabWidget in the same way
 * that MainWindow uses it to display the main interface widgets.
 *
 * Below the preferences pages the dialog has a set of buttons which allow the
 * user to interact with settings as a whole.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT
    
public:
    /*!
     * \brief Construct a new PreferencesDialog
     * \param parent This dialog's parent widget (the MainWindow)
     */
    explicit PreferencesDialog(QWidget *parent = 0);
    /*!
     * \brief Destroy the PreferencesDialog and free memory
     */
    ~PreferencesDialog();

public slots:
    /*!
     * \brief Handle the signal sent by the button bar when one of the buttons
     *  is pressed
     * \param button The button which has been pressed
     */
    void buttonClicked(QAbstractButton *button);

    /*!
     * \brief Reset the settings to the state they were in before any changes
     *  were made
     *
     * This does not imply that the settings have been set to their default
     * values. This is a less widely affecting option which will revert any
     * alterations which have not been applied.
     *
     * \todo It might be of use to provide a "restore defaults" option in the
     *  future
     */
    void reset();
    /*!
     * \brief Applies the current settings, saving them via QSettings
     *
     * After this has occurred the settings cannot be reversed with reset().
     */
    void apply();
    /*!
     * \brief Reimplementation of QDialog::accept() - accept changes and close
     *
     * This calls apply() before using the parent class' implementation of
     * accept.
     *
     * \sa apply
     */
    void accept();
    /*!
     * \brief Reimplementation of QDialog::reject() - reject changes and close
     *
     * This simply closes the window without saving anything.
     *
     * \todo Consider whether a prompt before discarding changes which are
     *  pending is prudent
     * \sa reset
     */
    void reject();
    
private:
    Ui::PreferencesDialog *_ui;
    QVector<PreferencesPage *> _pages;

    typedef QVector<PreferencesPage *>::iterator pageIter;
};

}

#endif // PREFERENCESDIALOG_HPP
