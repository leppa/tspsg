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
#   - translations go to /usr/share/TSPSG/l10n
#   - COPYING, ChangeLog.txt, README.txt and INSTALL.txt
#     go to /usr/share/doc/TSPSG-x.x.x
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
	desktop.files = resources/tspsg.desktop
	desktop.path = $$PREFIX/share/applications
	icon.files = resources/tspsg.png
	icon.path = $$PREFIX/share/pixmaps
	INSTALLS += desktop icon
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
	TARGET = TSPSG$${D}

	# qmake for Symbian (as of Qt 4.6.2) has a bug: file masks doesn't work, so we need to specify all files manually
	share.sources = $$share.files
#	l10n.sources = $$[QT_INSTALL_TRANSLATIONS]/qt_ru.qm \
#		$$[QT_INSTALL_TRANSLATIONS]/qt_uk.qm \
#		l10n/tspsg_en.qm l10n/tspsg_ru.qm l10n/tspsg_uk.qm
#	l10n.path = l10n
	docs.sources = $$docs.files
#	docs.pkg_prerules = \
#		"\"README.txt\" - \"\", FILETEXT, TEXTCONTINUE" \
#		"\"COPYING\" - \"\", FILETEXT, TEXTEXIT"
	DEPLOYMENT += share docs # l10n
#	DEPLOYMENT_PLUGIN += qjpeg qtiff # qsvgicon

	ICON = resources/tspsg.svg
	TARGET.EPOCHEAPSIZE = 0x20000 0x1100000
	# OVI Publish - 0x2003AEFB, Self-signed - 0xA89FD7A3
	TARGET.UID3 = 0xA89FD7A3
#	TARGET.UID3 = 0x2003AEFB

	languages="&EN,RU,UK"
	package_header = "$$LITERAL_HASH{" \
		" \"$$QMAKE_TARGET_PRODUCT\"," \
		" \"$$QMAKE_TARGET_PRODUCT\"," \
		" \"$$QMAKE_TARGET_PRODUCT\"" \
		"},($$TARGET.UID3),$$BUILD_VERSION_MAJOR,$${BUILD_VERSION_MINOR}$${BUILD_RELEASE},$$REVISION"
	vendor = \
		"%{\"Oleksii Serdiuk\",\"Алексей Сердюк\",\"Олексій Сердюк\"}" \
		":\"Oleksii Serdiuk\""
#	logo = \
#		"=\"resources/tspsg.png\",\"image/png\",\"\""
	dependencies = \
		"; Depend on Qt $${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}.$${QT_PATCH_VERSION}" \
		"(0x2001E61C),$${QT_MAJOR_VERSION},$${QT_MINOR_VERSION},$${QT_PATCH_VERSION},{\"Qt\",\"Qt\",\"Qt\"}" \
		"; Declare the supported platforms" \
		"; Symbian^1" \
		"[0x1028315F],0,0,0,{\"S60ProductID\",\"S60ProductID\",\"S60ProductID\"}" \
		"; Symbian^3" \
		"[0x20022E6D],0,0,0,{\"S60ProductID\",\"S60ProductID\",\"S60ProductID\"}"

	default_deployment.pkg_prerules -= pkg_platform_dependencies pkg_depends_qt
	default_deployment.pkg_prerules += languages package_header vendor dependencies
#	default_deployment.pkg_prerules += languages package_header vendor logo dependencies
	# OVI Publish - 0x2002CCCF, Self-signed - 0xA000D7CE
	DEPLOYMENT.installer_header = "$${LITERAL_HASH}{" \
		"\"$$QMAKE_TARGET_PRODUCT Installer\"," \
		"\"Установщик $$QMAKE_TARGET_PRODUCT\"," \
		"\"Встановлювач $$QMAKE_TARGET_PRODUCT\"" \
		"},(0xA000D7CE),$$BUILD_VERSION_MAJOR,$${BUILD_VERSION_MINOR}$${BUILD_RELEASE},$$REVISION"
#		"},(0x2002CCCF),$$BUILD_VERSION_MAJOR,$${BUILD_VERSION_MINOR}$${BUILD_RELEASE},$$REVISION"
}
