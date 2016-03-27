######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2016 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
#
#  $Id: $Format:%h %ai %an$ $
#  $URL: http://tspsg.info/ $
#
#  This file is part of TSPSG.
#
######################################################################

OTHER_FILES += \
    help/tspsg.qhcp \
    help/*.txt \
    help/html/*.* \
    help/html/en/*.html \
    help/html/en/images/*

help.target = help
help.commands = cd $$_PRO_FILE_PWD_/help && $$[QT_INSTALL_BINS]/qcollectiongenerator tspsg.qhcp -o tspsg.qhc
help.depends = FORCE

QMAKE_EXTRA_TARGETS += help
