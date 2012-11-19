#-------------------------------------------------
#
# Project created by QtCreator 2012-10-09T11:27:57
#
#-------------------------------------------------

QT       += core gui xml

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
    src/styledbutton.cpp \
    src/runconfiguration.cpp \
    src/results.cpp \
    src/newprojectwizard.cpp \
    src/project.cpp \
    src/graph.cpp \
    src/node.cpp \
    src/edge.cpp \
    src/preferencesdialog.cpp \
    src/aboutdialog.cpp

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
    src/styledbutton.hpp \
    src/runconfiguration.hpp \
    src/results.hpp \
    src/newprojectwizard.hpp \
    src/project.hpp \
    src/graph.hpp \
    src/node.hpp \
    src/edge.hpp \
    src/preferencesdialog.hpp \
    src/aboutdialog.hpp \
    src/global.hpp

FORMS    += src/mainwindow.ui \
    src/prettytabwidget.ui \
    src/prettytabheader.ui \
    src/welcome.ui \
    src/edit.ui \
    src/run.ui \
    src/helpdialog.ui \
    src/styledbutton.ui \
    src/runconfiguration.ui \
    src/results.ui \
    src/newprojectwizard.ui \
    src/preferencesdialog.ui \
    src/aboutdialog.ui

RESOURCES += \
    src/icons.qrc \
    src/stylesheets.qrc \
    src/images.qrc \
    src/templates.qrc

OTHER_FILES += \
    src/stylesheets/tabwidget.css \
    src/stylesheets/main.css \
    README.md \
    .gitignore \
    src/stylesheets/welcome.css \
    src/stylesheets/runconfiguration.css \
    src/stylesheets/helpdialog.css \
    src/stylesheets/preferences.css \
    src/templates/newproject.gpp
