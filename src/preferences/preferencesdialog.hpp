#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

class PreferencesPage;
class QAbstractButton;

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

public slots:
    void buttonClicked(QAbstractButton *button);

    void reset();
    void apply();
    void accept();
    void reject();
    
private:
    Ui::PreferencesDialog *_ui;
    QVector<PreferencesPage *> _pages;

    typedef QVector<PreferencesPage *>::iterator pageIter;
};

#endif // PREFERENCESDIALOG_HPP
