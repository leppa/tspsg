######################################################################
#
#  TSPSG: TSP Solver and Generator
#  Copyright (C) 2007-2013 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
#
#  $Id: $Format:%h %ai %an$ $
#  $URL: http://tspsg.info/ $
#
#  This file is part of TSPSG.
#
######################################################################

exists(".git/") {
    DESC = $$system("git describe --abbrev=40")
} else:exists(".tag") {
    DESC = $$cat(".tag")
}
!isEmpty(DESC) {
    DESC = $$split(DESC, "-")
    VER = $$member(DESC, 0)
    VER = $$split(VER, ".")

    BUILD_VERSION_MAJOR = $$member(VER, 0)
    BUILD_VERSION_MINOR = $$member(VER, 1)
    BUILD_RELEASE = $$member(VER, 2)
    count(DESC, 2) {
        # We're exactly at a tag - use whatever is provided by it
        BUILD_NUMBER = $$member(VER, 3)
        isEmpty(BUILD_NUMBER) {
            BUILD_NUMBER = 0
        }

        DEFINES += TSPSG_RELEASE_BUILD
        DEFINES += BUILD_STATUS=$$member(DESC, 1)
    } else {
        TMP = $$member(VER, 3)
        TMP = $$replace(TMP, "0$", "")
        BUILD_NUMBER = $$TMP$$member(DESC, 2)
        REVISION = $$member(DESC, 3)
        REVISION = $$replace(REVISION, "^g", "")

        DEFINES += REVISION=$$REVISION
    }

    SHORT_VERSION = $$sprintf("%1.%2",$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR)
    win32-msvc*|wincewm* {
        VERSION = $$SHORT_VERSION
    } else {
        VERSION = $$sprintf("%1.%2",$$SHORT_VERSION,$$BUILD_RELEASE)
    }
} else {
    # We can't deduce in case we don't have git or repo was exported.
    # For this case defaults are provided.
    BUILD_VERSION_MAJOR = 0
    BUILD_VERSION_MINOR = 1
    BUILD_RELEASE = 4
    BUILD_NUMBER = 0
}
