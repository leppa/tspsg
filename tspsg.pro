######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2011 Lёppa <contacts[at]oleksii[dot]name>
#
#  $Id: $Format:%h %ai %an$ $
#  $URL: http://tspsg.info/ $
#
#  This file is part of TSPSG.
#
######################################################################

!nosvg {
    QT += svg
} else {
    DEFINES += NOSVG
}

wincewm*|symbian|maemo*|simulator {
    CONFIG += handheld
}

TEMPLATE = app

# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk

# Version information
QMAKE_TARGET_COMPANY = Oleksii "Lёppa" Serdiuk
QMAKE_TARGET_PRODUCT = TSP Solver and Generator
QMAKE_TARGET_DESCRIPTION = TSPSG: TSP Solver and Generator
QMAKE_TARGET_COPYRIGHT = Copyright © 2007-2011 Oleksii "Lёppa" Serdiuk <contacts[at]oleksii[dot]name>

# Version detection
include(version.pri)

DEFINES += BUILD_VERSION_MAJOR=$$BUILD_VERSION_MAJOR \
    BUILD_VERSION_MINOR=$$BUILD_VERSION_MINOR \
    BUILD_RELEASE=$$BUILD_RELEASE \
    BUILD_NUMBER=$$BUILD_NUMBER

CONFIG(release, debug|release) {
    OBJECTS_DIR = release
    D =
} else {
    OBJECTS_DIR = debug
    DEFINES += DEBUG
#	CONFIG += console
    D = d
}
DESTDIR = bin
TARGET = tspsg$${D}

# Saving all intermediate files to tmp directory.
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp

# Include 3rd party libraries
!handheld {
    include(src/3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)
}

win32:LIBS += -lole32

# Include file(s)
include(tspsg.pri)

# Installation and deployment rules
include(install.pri)
