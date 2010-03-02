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

QT += svg

TEMPLATE = app
TARGET = tspsg
DEPENDPATH += .
INCLUDEPATH += .

# Versioning
BUILD_VERSION_MAJOR = 0
BUILD_VERSION_MINOR = 1
BUILD_RELEASE = 2

# This one is only defined on releases
#DEFINES += TSPSG_RELEASE_BUILD

REVISION = $$system(svnversion)
REVISION = $$replace(REVISION,"M","")
VERSION = $$sprintf("%1.%2.%3",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR,$$BUILD_RELEASE)

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
# We "embed" SVG and JPEG support on static build
	QTPLUGIN += qjpeg qsvg
	DEFINES += STATIC_BUILD
}

CONFIG(release, debug|release) {
	OBJECTS_DIR = release
	DESTDIR = release
} else {
	OBJECTS_DIR = debug
	DESTDIR = debug
	DEFINES += DEBUG
}

# Saving all intermediate files to tmp directory.
MOC_DIR = ./tmp
RCC_DIR = ./tmp
UI_DIR = ./tmp

# Include file(s)
include(tspsg.pri)

# Installation and deployment
# Common rules
l10n.files = l10n/*.qm
#share.files =
docs.files = COPYING README
INSTALLS += target l10n share docs

# For *nix:
#   - executable goes to /usr/bin
#   - COPYING and README go to /usr/share/TSPSG
#   - translations go to /usr/share/TSPSG/l10n
#   - docs go to /usr/share/doc/TSPSG-x.x.x
unix:!symbian {
	PREFIX = /usr
	CONFIG(release, debug|release) {
		DEFINES += PATH_L10N=\\\"$$PREFIX/share/TSPSG/l10n\\\"
		DEFINES += PATH_DOCS=\\\"$$PREFIX/share/TSPSG/docs\\\"
	}

	target.path = $$PREFIX/bin
	share.path = $$PREFIX/share/TSPSG
	l10n.path = $$PREFIX/share/TSPSG/l10n
	docs.path = $$PREFIX/share/doc/TSPSG-$$VERSION
	apps.path = $$PREFIX/share/applications/
	apps.files = resources/tspsg.desktop
	icon.path = $$PREFIX/share/pixmaps
	icon.files = resources/tspsg.png
	INSTALLS += apps icon
}

# TODO: MacOSX

# For win32: everything goes to "C:\Program Files\TSPSG" and subfolders.
win32 {
	PREFIX = "C:\Program Files"

	CONFIG(release, debug|release) {
		imageformats.files = $$[QT_INSTALL_PLUGINS]/imageformats/qsvg4.dll \
			$$[QT_INSTALL_PLUGINS]/imageformats/qjpeg4.dll
	} else {
		imageformats.files = $$[QT_INSTALL_PLUGINS]/imageformats/qsvgd4.dll \
			$$[QT_INSTALL_PLUGINS]/imageformats/qjpegd4.dll
	}
	imageformats.path = $$PREFIX/TSPSG/imageformats
	INSTALLS += imageformats
}

# For wince: we are deploying to \Program Files\TSPSG.
wince {
	PREFIX = "\Program Files"
	DEPLOYMENT += target share l10n docs
	DEPLOYMENT_PLUGIN += qjpeg qsvg
}

# win32 and wince common
win* {
	target.path = $$PREFIX/TSPSG
	share.path = $$PREFIX/TSPSG
	l10n.path = $$PREFIX/TSPSG/l10n
	docs.path = $$PREFIX/TSPSG

	RC_FILE = resources/tspsg.rc
}

# Symbian
symbian {
	l10n.path = l10n
	docs.pkg_prerules = \
		"\"README\" - \"\", FILETEXT, TEXTCONTINUE" \
		"\"COPYING\" - \"\", FILETEXT, TEXTEXIT"
	DEPLOYMENT += share l10n docs
	DEPLOYMENT_PLUGIN += qjpeg qsvg

	ICON = resources/tspsg.svg

	appinfo = \
		"$$LITERAL_HASH{\"TSPSG\"},(0xEb9dce0e),0,1,2"
	vendorinfo = \
		"%{\"l-homes.org\"}" \
		":\"l-homes.org\""
	default_deployment.pkg_prerules = appinfo vendorinfo
	DEPLOYMENT.installer_header = "$${LITERAL_HASH}{\"TSPSG Installer\"},(0xA000D7CE),1,0,0"
}
