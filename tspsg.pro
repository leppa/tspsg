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

win32 {
    TEMPLATE = vcapp
} else {
    TEMPLATE = app
}
TARGET = tspsg
DEPENDPATH += .
INCLUDEPATH += .

# Saving all intermediate files to tmp directory.
MOC_DIR = ./tmp
RCC_DIR = ./tmp
UI_DIR = ./tmp

#Include file(s)
include(tspsg.pri)

# For wince: we are deploying to storage card because Qt libraries
# (especially debug) are big enough for internal memory.
deploy.path = \Storage Card\tspsg
DEPLOYMENT += deploy

#Windows resource file
win32:RC_FILE = resources/tspsg.rc
