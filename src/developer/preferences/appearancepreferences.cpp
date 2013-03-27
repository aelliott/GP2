/*!
 * \file
 */
#include "appearancepreferences.hpp"
#include "ui_appearancepreferences.h"

#include <QSettings>
#include <QFile>

namespace Developer {

AppearancePreferences::AppearancePreferences(QWidget *parent)
    : PreferencesPage(parent)
    , _ui(new Ui::AppearancePreferences)
{
    _ui->setupUi(this);

    // Set up a simple program containing each of the token types
    QFile fp(":/templates/example_program.gpx");
    fp.open(QIODevice::ReadOnly | QIODevice::Text);
    QString example = fp.readAll();

    _ui->plainTextEdit->setPlainText(example);
    _ui->plainTextEdit->parse();
}

AppearancePreferences::~AppearancePreferences()
{
    delete _ui;
}

void AppearancePreferences::reset()
{
    QSettings settings;
}

void AppearancePreferences::apply()
{
    QSettings settings;
}

}
