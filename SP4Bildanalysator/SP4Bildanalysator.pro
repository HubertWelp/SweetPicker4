QT -= gui
QT += core
QT += network
QT += widgets


CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp\
        kamera.cpp \
        orientierungsermittler.cpp \
        ressourcen.cpp \
        verwalter.cpp \
        udpnode.cpp \
        textauswerter.cpp

HEADERS += \
        kamera.h \
        orientierungsermittler.h \
        verwalter.h \
        ressourcen.h \
        udpnode.hpp \
        textauswerter.h


INCLUDEPATH += /usr/include/opencv4/


LIBS += -Lopencv2 \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_videoio


LIBS += /home/student/git/SP4/SweetPicker4/build-SP4Admin-Desktop_Qt_6_2_3_GCC_64bit-Debug/konfig.o

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
