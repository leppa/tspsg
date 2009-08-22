/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TSPSG is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with TSPSG.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

// INCLUDES
#include <QtCore>
#include <QtGui>

// Version info
#include "version.h"
// OS and ARCH detection
#include "os.h"

// DEFINES
// Default values
#define DEF_RAND_MIN 1
#define DEF_RAND_MAX 10
#define DEF_NUM_CITIES 5
#define DEF_FONT_FAMILY "Courier New"
#define DEF_FONT_SIZE 10
#define DEF_FONT_COLOR Qt::black

// Maximum available number of cities
#define MAX_NUM_CITIES 30
// Maximum for random generation limit settings
#define MAX_RAND_VALUE 1000

// Paths
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
	#define PATH_I18N "/usr/share/tspsg/i18n"
	#define PATH_DOCS "/usr/share/doc/tspsg"
#else
	#define PATH_I18N "i18n"
	#define PATH_DOCS "help"
#endif // Q_OS_LINUX

// TSPSG Task file signature - letters TSPT
#define TSPT quint32(0x54535054)
// TSPSG Task file version
#define TSPT_VERSION quint8(1)
// TSPSG Task file metadata version
#define TSPT_META_VERSION quint8(1)
// TSPSG Task file metadata size in bytes (incl. version)
#define TSPT_META_SIZE 2
// ZKomModRd Task file signature - letters ZK
#define ZKT quint16(0x5A4B)
// ZKomModRd Task file version
#define ZKT_VERSION quint8(1)

// This value means infinity :-)
#ifndef INFINITY
	#define INFINITY 1.7E+308
#endif
// This is string, which represents infinite value in table
#define INFSTR "---"

// Check that default number of cities is sane (<= MAX_NUM_CITIES)
#if DEF_NUM_CITIES > MAX_NUM_CITIES
	#undef DEF_NUM_CITIES
	#define DEF_NUM_CITIES MAX_NUM_CITIES
#endif
// Check that maximum for random generation is sane (<= MAX_RAND_VALUE)
#if DEF_RAND_MAX > MAX_RAND_VALUE
	#undef DEF_RAND_MAX
	#define DEF_RAND_MAX MAX_RAND_VALUE
#endif
// Check that DEF_RAND_MIN <= DEF_RAND_MAX
#if DEF_RAND_MIN > DEF_RAND_MAX
	#undef DEF_RAND_MIN
	#define DEF_RAND_MIN DEF_RAND_MAX
#endif

#endif // GLOBALS_H
