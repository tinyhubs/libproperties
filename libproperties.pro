TEMPLATE = lib
CONFIG  -= qt
CONFIG  -= gui
CONFIG  += console
CONFIG  += c11

TARGET  = properties
DESTDIR = $$PWD/lib
QMAKE_CFLAGS += -fsigned-char

VERSION = $$cat("$$PWD/VERSION")

include($$PWD/libproperties.pri)

