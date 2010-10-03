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

# Common rules
#l10n.files = l10n/*.qm
#share.files =
docs.files = COPYING ChangeLog.txt README.txt INSTALL.txt
INSTALLS += target share docs # l10n

# For *nix:
#   - executable goes to /usr/bin
#   - COPYING and README go to /usr/share/TSPSG
#   - translations go to /usr/share/TSPSG/l10n
#   - docs go to /usr/share/doc/TSPSG-x.x.x
unix:!macx:!symbian {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	CONFIG(release, debug|release) {
		DEFINES += PATH_L10N=\\\"$$PREFIX/share/TSPSG/l10n\\\"
		DEFINES += PATH_DOCS=\\\"$$PREFIX/share/TSPSG/docs\\\"
	}

	target.path = $$PREFIX/bin
	share.path = $$PREFIX/share/TSPSG
#	l10n.path = $$PREFIX/share/TSPSG/l10n
	docs.path = $$PREFIX/share/doc/TSPSG-$$VERSION
	apps.files = resources/tspsg.desktop
	apps.path = $$PREFIX/share/applications/
	icon.files = resources/tspsg.png
	icon.path = $$PREFIX/share/pixmaps
	INSTALLS += apps icon
}

# TODO: MacOSX
macx {
}

# For win32: everything goes to "%PROGRAMFILES%\TSPSG" and subfolders.
win32 {
	isEmpty(PREFIX) {
		PREFIX = "$$(PROGRAMFILES)"
	}

	share.files = $$[QT_INSTALL_BINS]/QtCore$${D}4.dll \
		$$[QT_INSTALL_BINS]/QtGui$${D}4.dll
	!nosvg {
		share.files += $$[QT_INSTALL_BINS]/QtSvg$${D}4.dll
	}
#	l10n.files += $$[QT_INSTALL_TRANSLATIONS]/*.qm
	win32-g++ {
		share.files += $$[QT_INSTALL_BINS]/mingwm10.dll \
			$$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll
	}
#	iconengines.files = $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicon$${D}4.dll
#	iconengines.path = $$PREFIX/TSPSG/iconengines
	imageformats.files = $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg$${D}4.dll \
		$$[QT_INSTALL_PLUGINS]/imageformats/qtiff$${D}4.dll
	imageformats.path = $$PREFIX/TSPSG/imageformats
	INSTALLS += imageformats # iconengines
}

# For wince: we are deploying to \Program Files\TSPSG.
wince* {
	isEmpty(PREFIX) {
		PREFIX = "\\Program Files"
	}
	share.sources = $$share.files
#	l10n.sources = $$l10n.files \
#		$$[QT_INSTALL_TRANSLATIONS]/*.qm
	docs.sources = $$docs.files

	DEPLOYMENT += target share docs # l10n
#	DEPLOYMENT_PLUGIN += qjpeg qtiff qsvgicon
}

# win32 and wince common
win* {
	target.path = $$PREFIX/TSPSG
	share.path = $$PREFIX/TSPSG
#	l10n.path = $$PREFIX/TSPSG/l10n
	docs.path = $$PREFIX/TSPSG
}

# Symbian
symbian {
	# qmake for Symbian (as of Qt 4.6.2) has a bug: file masks doesn't work, so we need to specify all files manually
	share.sources = $$share.files
#	l10n.sources = $$[QT_INSTALL_TRANSLATIONS]/qt_ru.qm \
#		$$[QT_INSTALL_TRANSLATIONS]/qt_uk.qm \
#		l10n/tspsg_en.qm l10n/tspsg_ru.qm l10n/tspsg_uk.qm
#	l10n.path = l10n
	docs.sources = $$docs.files
	docs.pkg_prerules = \
		"\"README.txt\" - \"\", FILETEXT, TEXTCONTINUE" \
		"\"COPYING\" - \"\", FILETEXT, TEXTEXIT"
	DEPLOYMENT += share docs # l10n
#	DEPLOYMENT_PLUGIN += qjpeg qtiff qsvgicon

	ICON = resources/tspsg.svg

	appinfo = \
		"$$LITERAL_HASH{\"TSPSG\"},(0xEb9dce0e),$$BUILD_VERSION_MAJOR,$$BUILD_VERSION_MINOR,$$BUILD_RELEASE"
	vendorinfo = \
		"%{\"l-homes.org\"}" \
		":\"l-homes.org\""
	default_deployment.pkg_prerules = appinfo vendorinfo
	DEPLOYMENT.installer_header = "$${LITERAL_HASH}{\"TSPSG Installer\"},(0xA000D7CE),1,0,0"
}
