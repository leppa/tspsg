######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2016 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
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

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport concurrent
}

wincewm*|symbian|maemo*|simulator|blackberry {
    CONFIG += handheld
}
blackberry {
#    DEFINES += QT_NO_SHORTCUT
    DEFINES += QT_NO_PRINTDIALOG
    DEFINES += QT_NO_PRINTPREVIEWDIALOG

    QT += xml declarative
    LIBS += -lbb -lbbcascadespickers
}

TEMPLATE = app

# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk

# Version information
QMAKE_TARGET_COMPANY = Oleksii Serdiuk
QMAKE_TARGET_PRODUCT = TSP Solver and Generator
QMAKE_TARGET_DESCRIPTION = TSPSG: TSP Solver and Generator
QMAKE_TARGET_COPYRIGHT = Copyright © 2007-2016 Oleksii Serdiuk <contacts[at]oleksii[dot]name>

# Version detection
include(version.pri)

DEFINES += BUILD_VERSION_MAJOR=$$BUILD_VERSION_MAJOR \
    BUILD_VERSION_MINOR=$$BUILD_VERSION_MINOR \
    BUILD_RELEASE=$$BUILD_RELEASE \
    BUILD_NUMBER=$$BUILD_NUMBER

blackberry {
    D =
} else {
    !debug_and_release|build_pass {
        CONFIG(debug, debug|release) {
            OBJECTS_DIR = debug
            DEFINES += DEBUG
#            CONFIG += console
            D = d
        } else {
            OBJECTS_DIR = release
            D =
        }
    }
    DESTDIR = bin
}
TARGET = tspsg$${D}

# Saving all intermediate files to tmp directory.
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp

# We need to generate translations before building.
# Either way, resource files won't compile.
translations.name = Translations
translations.input = TRANSLATIONS
translations.output = $$_PRO_FILE_PWD_/l10n/${QMAKE_FILE_BASE}.qm
freebsd-* {
    translations.commands = $$[QT_INSTALL_BINS]/lrelease-qt$${QT_MAJOR_VERSION} ${QMAKE_FILE_IN}
} else {
    translations.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN}
}
translations.CONFIG = no_link
QMAKE_EXTRA_COMPILERS += translations
PRE_TARGETDEPS += compiler_translations_make_all

# Include 3rd party libraries
!handheld {
    include(src/3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)

    unix:LIBS += -lX11
}

win32:LIBS += -lole32

# Source
include(tspsg.pri)

# Help
include(help/help.pri)

# Code Documentation
include(doc/doc.pri)

# Installation and deployment rules
include(install.pri)
