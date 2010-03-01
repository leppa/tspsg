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

#Header files
HEADERS += src/globals.h \
	src/version.h \
	src/os.h \
	src/defaults.h \
	src/mainwindow.h \
	src/settingsdialog.h \
	src/tspmodel.h \
	src/tspsolver.h \
	src/resource.h \
	src/qtwin.h

#Source files
SOURCES += src/main.cpp \
	src/mainwindow.cpp \
	src/settingsdialog.cpp \
	src/tspmodel.cpp \
	src/tspsolver.cpp \
	src/qtwin.cpp

#Forms
FORMS += ui/mainwindow.ui \
	ui/settingsdialog.ui

#Resource file(s)
RESOURCES += resources/tspsg.qrc

#Translations
CODECFORTR = UTF-8
TRANSLATIONS += l10n/tspsg_en.ts \
	l10n/tspsg_ru.ts \
	l10n/tspsg_uk.ts
