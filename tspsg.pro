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

TEMPLATE = app
TARGET = tspsg
DEPENDPATH += .
INCLUDEPATH += .

CONFIG(release, debug|release) {
	OBJECTS_DIR = release
	DESTDIR = release
	win32 {
		OBJECTS_DIR = release/win32
		DESTDIR = release/win32
	}
	wince* {
		OBJECTS_DIR = release/wince
		DESTDIR = release/wince
	}
	unix {
		OBJECTS_DIR = release/nix
		DESTDIR = release/nix
	}
} else {
	OBJECTS_DIR = debug
	DESTDIR = debug
	win32 {
		OBJECTS_DIR = debug/win32
		DESTDIR = debug/win32
	}
	wince* {
		OBJECTS_DIR = debug/wince
		DESTDIR = debug/wince
	}
	unix {
		OBJECTS_DIR = debug/nix
		DESTDIR = debug/nix
	}
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
