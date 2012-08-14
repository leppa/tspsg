######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2012 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
#
#  $Id: $Format:%h %ai %an$ $
#  $URL: http://tspsg.info/ $
#
#  This file is part of TSPSG.
#
######################################################################

OTHER_FILES += \
    doc/Doxyfile \
    doc/dox/*.dox

#dox.depends = $(SOURCES) doc/dox/*.dox
dox.target = doc
dox.commands = cd $$_PRO_FILE_PWD_ && doxygen Doxyfile
dox.depends = FORCE

QMAKE_EXTRA_TARGETS += dox
