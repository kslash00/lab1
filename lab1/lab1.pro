QT       += core gui
QT += widgets
RESOURCES += resources.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

SOURCES += \
    acclist.cpp \
    addacc.cpp \
    crypt.cpp \
    customwidget.cpp \
    dbmodel.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    acclist.h \
    addacc.h \
    crypt.h \
    customwidget.h \
    dbmodel.h \
    mainwindow.h

FORMS += \
    acclist.ui \
    addacc.ui \
    customwidget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
*.debug=true
include($$PWD/../Qt-Secret/src/Qt-Secret.pri)

DISTFILES += \
    icons/padlock.ico \
    icons/pin-code.png \
    icons/poly.png
