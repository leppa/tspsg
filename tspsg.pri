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

#Header files
HEADERS += src/defines.h \
	src/mainwindow.h \
	src/settingsdialog.h \
	src/tspmodel.h \
	src/tspsolver.h \
	src/resource.h

#Source files
SOURCES += ./src/main.cpp \
	src/mainwindow.cpp \
	src/settingsdialog.cpp \
	src/tspmodel.cpp \
	src/tspsolver.cpp

#Forms
FORMS += ui/mainwindow.ce.ui \
	ui/mainwindow.ui \
	ui/settingsdialog.ui \
	ui/settingsdialog.ce.ui

#Resource file(s)
RESOURCES += resources/tspsg.qrc

#Translations
CODECFORTR = UTF-8
TRANSLATIONS += i18n/English.ts \
	i18n/Russian.ts \
	i18n/Ukrainian.ts
