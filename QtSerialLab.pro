QT += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/app_controller.cpp \
    src/serial_com.cpp

HEADERS += \
    include/app_controller.h \
    include/serial_com.h

FORMS += \
    ui/interface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
