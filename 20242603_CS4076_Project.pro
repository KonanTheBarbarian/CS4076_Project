QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Your code may fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Command.cpp \
    CommandWords.cpp \
    Parser.cpp \
    Planet.cpp \
    Stack.cpp \
    WordleEngine.cpp \
    ZorkUL.cpp \

HEADERS += \
    Command.h \
    Parser.h \
    Planet.h \
    Stack.h \
    WordleEngine.h \
    ZorkUL.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    wordle_words.txt

RESOURCES += \
    resources.qrc
