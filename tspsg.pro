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

#Include file(s)
include(tspsg.pri)

#Windows resource file
win32:RC_FILE = resources/tspsg.rc
