######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
#
#  $Id$
#  $URL$
#
#  This file is part of TSPSG.
#
######################################################################

!nosvg {
	QT += svg
} else {
	DEFINES += NOSVG
}

TEMPLATE = app

# Versioning
BUILD_VERSION_MAJOR = 0
BUILD_VERSION_MINOR = 1
BUILD_RELEASE = 3

# These are only defined on releases
#DEFINES += TSPSG_RELEASE_BUILD
#!symbian {
#      DEFINES += BUILD_STATUS="\"(alpha 2)\""
#} else {
#      # Symbian doesn't handle spaces in defines well
#      DEFINES += BUILD_STATUS="(alpha2)"
#}

#REVISION = 100
REVISION = $$system(svnversion)
REVISION = $$replace(REVISION,":","")
REVISION = $$replace(REVISION,"M","")
#VERSION = $$sprintf("%1.%2.%3.%4",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR,$$BUILD_RELEASE,$$REVISION)
win32-msvc* {
	VERSION = $$sprintf("%1.%2",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR)
} else {
	VERSION = $$sprintf("%1.%2.%3.%4",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR,$$BUILD_RELEASE,$$REVISION)
}

DEFINES += BUILD_VERSION_MAJOR=$$BUILD_VERSION_MAJOR \
	BUILD_VERSION_MINOR=$$BUILD_VERSION_MINOR \
	BUILD_RELEASE=$$BUILD_RELEASE \
	BUILD_NUMBER=$$REVISION

# A hack to determine whether we have static or dynamic Qt build
unix:!symbian {
	PRL = $$[QT_INSTALL_LIBS] libQtCore.prl
} else:unix {
	PRL = $$[QT_INSTALL_LIBS] QtCore.prl
} else {
	PRL = $$[QT_INSTALL_LIBS] QtCore.prl
}
include($$join(PRL, "/"))
contains(QMAKE_PRL_CONFIG, static) {
# We "embed" JPEG and TIFF support on static build
	DEFINES += STATIC_BUILD
#	QTPLUGIN += qjpeg qtiff
}

CONFIG(release, debug|release) {
	OBJECTS_DIR = release
	DESTDIR = release
	D =
} else {
	OBJECTS_DIR = debug
	DESTDIR = debug
	DEFINES += DEBUG
#	CONFIG += console
	D = d
}
TARGET = tspsg$${D}

# Saving all intermediate files to tmp directory.
MOC_DIR = ./tmp
RCC_DIR = ./tmp
UI_DIR = ./tmp

# Include 3rd party libraries
include(src/3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)

# Include file(s)
include(tspsg.pri)

# Installation and deployment
include(install.pri)
