#-------------------------------------------------
#
# Project created by QtCreator 2012-10-09T11:27:57
#
#-------------------------------------------------

QT += core gui xml widgets

TARGET = GPDeveloper
TEMPLATE = app

INCLUDEPATH += src/developer

HEADERS += \
    welcome.hpp \
    styledbutton.hpp \
    runconfiguration.hpp \
    run.hpp \
    rule.hpp \
    results.hpp \
    project.hpp \
    program.hpp \
    prettytabwidget.hpp \
    prettytabmenu.hpp \
    prettytabheader.hpp \
    prettytabbar.hpp \
    prettytab.hpp \
    node.hpp \
    newprojectwizard.hpp \
    mainwindow.hpp \
    helpdialog.hpp \
    graph.hpp \
    gpfile.hpp \
    global.hpp \
    edit.hpp \
    edge.hpp \
    aboutdialog.hpp \
    graphview/nodeitem.hpp \
    graphview/graphwidget.hpp \
    graphview/edgeitem.hpp \
    preferences/projectpreferences.hpp \
    preferences/preferencespage.hpp \
    preferences/preferencesdialog.hpp \
    quickrunwidget.hpp \
    ruleparser.hpp

FORMS += \
    welcome.ui \
    styledbutton.ui \
    runconfiguration.ui \
    run.ui \
    results.ui \
    prettytabwidget.ui \
    prettytabheader.ui \
    newprojectwizard.ui \
    mainwindow.ui \
    helpdialog.ui \
    edit.ui \
    aboutdialog.ui \
    preferences/projectpreferences.ui \
    preferences/preferencesdialog.ui \
    quickrunwidget.ui

RESOURCES += \
    icons.qrc \
    templates.qrc \
    stylesheets.qrc \
    images.qrc

SOURCES += \
    welcome.cpp \
    styledbutton.cpp \
    runconfiguration.cpp \
    run.cpp \
    rule.cpp \
    results.cpp \
    project.cpp \
    program.cpp \
    prettytabwidget.cpp \
    prettytabmenu.cpp \
    prettytabheader.cpp \
    prettytabbar.cpp \
    prettytab.cpp \
    node.cpp \
    newprojectwizard.cpp \
    mainwindow.cpp \
    main.cpp \
    helpdialog.cpp \
    graph.cpp \
    gpfile.cpp \
    global.cpp \
    edit.cpp \
    edge.cpp \
    aboutdialog.cpp \
    graphview/nodeitem.cpp \
    graphview/graphwidget.cpp \
    graphview/edgeitem.cpp \
    preferences/projectpreferences.cpp \
    preferences/preferencespage.cpp \
    preferences/preferencesdialog.cpp \
    quickrunwidget.cpp \
    ruleparser.cpp

OTHER_FILES += \
    templates/newproject.gpp \
    CMakeLists.txt \
    stylesheets/welcome.css \
    stylesheets/tabwidget.css \
    stylesheets/runconfiguration.css \
    stylesheets/preferences.css \
    stylesheets/main.css \
    stylesheets/helpdialog.css \
    documentation/namespace_developer.dox \
    documentation/developer_main.dox \
    tests/CMakeLists.txt \
    templates/newrule_alternative.gpr

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libgp/release/ -llibgp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libgp/debug/ -llibgp
else:unix: LIBS += -L$$OUT_PWD/../libgp/ -llibgp

INCLUDEPATH += $$PWD/../libgp
DEPENDPATH += $$PWD/../libgp
