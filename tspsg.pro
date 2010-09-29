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

# QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.5.sdk

# Version information
QMAKE_TARGET_COMPANY = Oleksii "Lёppa" Serdiuk
QMAKE_TARGET_PRODUCT = TSP Solver and Generator
QMAKE_TARGET_DESCRIPTION = TSPSG: TSP Solver and Generator
QMAKE_TARGET_COPYRIGHT = Copyright © 2007-2010 Oleksii "Lёppa" Serdiuk <contacts[at]oleksii[dot]name>

# Versioning
BUILD_VERSION_MAJOR = 0
BUILD_VERSION_MINOR = 1
BUILD_RELEASE = 3

# These are only defined on releases
#DEFINES += TSPSG_RELEASE_BUILD
#!symbian {
#	DEFINES += BUILD_STATUS="\"(alpha 2)\""
#} else {
#	# Symbian doesn't handle spaces in defines well
#	DEFINES += BUILD_STATUS="(alpha2)"
#}

#REVISION = 100
REVISION = $$system(svnversion)
REVISION = $$replace(REVISION,":","")
REVISION = $$replace(REVISION,"M","")
win32-msvc*|wincewm* {
	VERSION = $$sprintf("%1.%2",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR)
} else {
	VERSION = $$sprintf("%1.%2.%3.%4",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR,$$BUILD_RELEASE,$$REVISION)
}

DEFINES += BUILD_VERSION_MAJOR=$$BUILD_VERSION_MAJOR \
	BUILD_VERSION_MINOR=$$BUILD_VERSION_MINOR \
	BUILD_RELEASE=$$BUILD_RELEASE \
	BUILD_NUMBER=$$REVISION

# A hack to determine whether we have static or dynamic Qt build
macx {
	PRL = $$[QT_INSTALL_LIBS] QtCore.framework QtCore.prl
} else:symbian {
	PRL = $$[QT_INSTALL_LIBS] QtCore.prl
} else:unix {
	PRL = $$[QT_INSTALL_LIBS] libQtCore.prl
} else {
	PRL = $$[QT_INSTALL_LIBS] QtCore.prl
}
include($$join(PRL, "/"))
contains(QMAKE_PRL_CONFIG, static) {
	# We "embed" SVG icon, JPEG and TIFF support on static build
	DEFINES += STATIC_BUILD
#	!nosvg:QTPLUGIN += qsvgicon
}

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
!wincewm*:!symbian {
	include(src/3rdparty/qttoolbardialog-2.2_1-opensource/src/qttoolbardialog.pri)
}

win32:LIBS += -lole32

# Include file(s)
include(tspsg.pri)

# Installation and deployment
include(install.pri)
