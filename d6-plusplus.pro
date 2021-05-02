QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Core/Converter.cpp \
    Core/Evaluator.cpp \
    Core/MassCalc.cpp \
    Core/MathFunc.cpp \
    Core/RPNTool.cpp \
    Dice/Die.cpp \
    Dice/Roller.cpp \
    Errors/Error.cpp \
    Errors/ErrorClass.cpp \
    Errors/FuncError.cpp \
    Errors/InputError.cpp \
    Utility/CommHist.cpp \
    Utility/History.cpp \
    Utility/UtilityFunctions.cpp \
    filewindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Core/Converter.h \
    Core/Evaluator.h \
    Core/MassCalc.h \
    Core/MathFunc.h \
    Core/RPNTool.h \
    Dice/Die.h \
    Dice/Roller.h \
    Errors/Error.h \
    Errors/ErrorClass.h \
    Errors/FuncError.h \
    Errors/InputError.h \
    Utility/CircBuff.h \
    Utility/CommHist.h \
    Utility/History.h \
    Utility/HistoryQueue.h \
    Utility/UtilityFunctions.h \
    filewindow.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    filewindow.ui \
    mainwindow.ui
