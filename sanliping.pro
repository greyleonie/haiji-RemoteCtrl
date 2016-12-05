#-------------------------------------------------
#
# Project created by QtCreator 2012-03-18T18:57:53
#
#-------------------------------------------------

QT       += core gui \
            sql \
            network \
            xml

HEADERS += \
    LoginDialog.h \
    UserDialog.h \
    MonitorMainWindow.h \
    MonitorWindow.h \
    Console.h \
    MoteDialog.h \
    UserInfo.h \
    MoteConfig.h \
    Logs.h \
    OperLogs.h \
    ModbusTcpClient.h \
    ModbusTcpFrame.h \
    Mote.h \
    MoteControl.h \
    MoteTableDialog.h \
    LogViewDialog.h \
    Config.h \
    StateViewDialog.h

SOURCES += \
    main.cpp \
    LoginDialog.cpp \
    UserDialog.cpp \
    MonitorWindow.cpp \
    Console.cpp \
    MoteDialog.cpp \
    UserInfo.cpp \
    MoteConfig.cpp \
    Logs.cpp \
    OperLogs.cpp \
    ModbusTcpClient.cpp \
    ModbusTcpFrame.cpp \
    Mote.cpp \
    MoteControl.cpp \
    MoteTableDialog.cpp \
    LogViewDialog.cpp \
    Config.cpp \
    StateViewDialog.cpp

FORMS += \
    monitor.ui \
    MoteDialog.ui \
    MoteConfig.ui \
    UserDialog.ui \
    LoginDialog.ui \
    MoteControl.ui

OTHER_FILES += \
    config.xml \
    myico.rc \
    sanliping.ico \
    view_statelogs.sql \
    view_operlogs.sql

RC_FILE += \
    myico.rc
