#-------------------------------------------------
#
# Project created by QtCreator 2012-10-09T11:27:57
#
#-------------------------------------------------

QT       += core gui

TARGET = GPDeveloper
TEMPLATE = app

INCLUDEPATH += src

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/prettytabwidget.cpp \
    src/prettytabmenu.cpp \
    src/prettytabheader.cpp \
    src/prettytabbar.cpp \
    src/prettytab.cpp \
    src/welcome.cpp \
    src/edit.cpp \
    src/run.cpp \
    src/helpdialog.cpp \
    src/styledbutton.cpp

HEADERS  += src/mainwindow.hpp \
    src/prettytabwidget.hpp \
    src/prettytabmenu.hpp \
    src/prettytabheader.hpp \
    src/prettytabbar.hpp \
    src/prettytab.hpp \
    src/welcome.hpp \
    src/edit.hpp \
    src/run.hpp \
    src/helpdialog.hpp \
    src/styledbutton.hpp

FORMS    += src/mainwindow.ui \
    src/prettytabwidget.ui \
    src/prettytabheader.ui \
    src/welcome.ui \
    src/edit.ui \
    src/run.ui \
    src/helpdialog.ui \
    src/styledbutton.ui

RESOURCES += \
    src/icons.qrc \
    src/stylesheets.qrc \
    src/images.qrc

OTHER_FILES += \
    src/stylesheets/tabwidget.css \
    src/stylesheets/main.css \
    README.md \
    .gitignore \
    src/stylesheets/welcome.css
