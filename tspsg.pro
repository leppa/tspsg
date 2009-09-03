######################################################################
#
# TSPSG: TSP Solver and Generator
# Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
#
# $Id$
# $URL$
#
# This file is part of TSPSG.
#
######################################################################

TEMPLATE = app
TARGET = tspsg
DEPENDPATH += .
INCLUDEPATH += .

CONFIG(release, debug|release) {
	OBJECTS_DIR = release
	DESTDIR = release
} else {
	OBJECTS_DIR = debug
	DESTDIR = debug
}

# Saving all intermediate files to tmp directory.
MOC_DIR = ./tmp
RCC_DIR = ./tmp
UI_DIR = ./tmp

#Include file(s)
include(tspsg.pri)

# For *nix:
#   - executable goes to $(INSTALL_ROOT)/bin
#   - COPYING and README go to $(INSTALL_ROOT)/share/tspsg
#   - translations go to $(INSTALL_ROOT)/share/tspsg/i18n
#   - docs (none, yet) go to $(INSTALL_ROOT)/share/doc/tspsg
# Usually, $(INSTALL_ROOT) is /usr or /usr/local
unix {
	target.path = /bin
	share.path = /share/tspsg
	share.files = COPYING README
	i18n.path = /share/tspsg/i18n
	i18n.files = i18n/languages.ini i18n/*.qm
	docs.path = /share/doc/tspsg
#	docs.files = docs/*
	apps.path = /share/applications/
	apps.files = resources/tspsg.desktop
	icon.path = /share/pixmaps
	icon.files = resources/tspsg.png
	INSTALLS += target i18n docs share icon apps
}

# For win32: everything goes to $(INSTALL_ROOT)\tspsg and subfolders.
# Usually, $(INSTALL_ROOT) is "C:\Program Files"
win32 {
	target.path = "\tspsg"
	share.path = "\tspsg"
	share.files = COPYING README
	i18n.path = "\tspsg\i18n"
	i18n.files = i18n\languages.ini i18n\*.qm
	docs.path = "\tspsg\help"
#	docs.files = docs\*
	INSTALLS += target i18n docs share
}

# TODO: MacOSX

# For wince: we are deploying to \Storage Card\Program Files\tspsg.
wince {
	deploy.path = "\Storage Card\Program Files\tspsg"
	share.sources = COPYING README
	share.path = "\Storage Card\Program Files\tspsg"
	i18n.sources = i18n\languages.ini i18n\*.qm
	i18n.path = "\Storage Card\Program Files\tspsg\i18n"
#	docs.sources = docs\*
#	docs.path = "\Storage Card\Program Files\tspsg\help"
	DEPLOYMENT += deploy share i18n # docs
}

#Windows resource file
win32:RC_FILE = resources/tspsg.rc
