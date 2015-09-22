
TEMPLATE = lib
TARGET = soccerwindow2_model
DESTDIR = ../../lib

DEPENDPATH += .
win32 {
  DEPENDPATH += . ../../rcsc
}

INCLUDEPATH += . .. ../..
win32 {
  INCLUDEPATH += ../../boost
}
unix {
  INCLUDEPATH += ${HOME}/local/include
  INCLUDEPATH += /opt/local/include
}
macx {
  INCLUDEPATH += /opt/local/include
}

LIBS +=

DEFINES += HAVE_LIBRCSC_GZ
win32 {
  DEFINES += HAVE_WINDOWS_H NO_TIMER
}
unix {
  DEFINES += HAVE_NETINET_IN_H
}

CONFIG += staticlib warn_on release
#QMAKE_LFLAGS_DEBUG += -static
#QMAKE_LFLAGS_DEBUG += -pg
#QMAKE_CFLAGS_DEBUG += -pg -static -DQABSTRACTSOCKET_DEBUG
#QMAKE_CXXFLAGS_DEBUG += -pg -static -DQABSTRACTSOCKET_DEBUG

# Input
HEADERS += \
ball.h \
debug_client_parser.h \
debug_log_data.h \
debug_log_holder.h \
debug_view_data.h \
main_data.h \
monitor_view_data.h \
options.h \
player.h \
playmode.h \
point.h \
team.h \
trainer_data.h \
view_holder.h

SOURCES += \
ball.cpp \
debug_client_parser.cpp \
debug_log_data.cpp \
debug_log_holder.cpp \
debug_view_data.cpp \
main_data.cpp \
monitor_view_data.cpp \
options.cpp \
player.cpp \
playmode.cpp \
team.cpp \
trainer_data.cpp \
view_holder.cpp
