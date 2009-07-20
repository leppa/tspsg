######################################################################
#
# TSPSG - TSP Solver and Generator
# Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
#
# $Id$
# $URL$
#
# This file is part of TSPSG.
#
######################################################################

QT += svg
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

# For wince: we are deploying to storage card because Qt libraries
# (especially debug) are big enough for internal memory.
deploy.path = "\Storage Card\tspsg"
i18n.sources = i18n\languages.ini i18n\*.qm
i18n.path = "\Storage Card\tspsg\i18n"
DEPLOYMENT += deploy i18n

#Windows resource file
win32:RC_FILE = resources/tspsg.rc
