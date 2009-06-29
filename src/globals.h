/*
 *  TSPSG - TSP Solver and Generator
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

// OS detection
#include "os.h"

// DEFINES
// Default values
#define DEF_RAND_MIN 1
#define DEF_RAND_MAX 10
#define DEF_OFFSET 100
#define DEF_FONT_FAMILY "Courier New"
#define DEF_FONT_SIZE 12
#define DEF_FONT_COLOR Qt::black

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

// Decided, that static array with 100 of cities maximum hard limit
// will be enough for most cases, but the code will be simplier than
// when using dynamic lists. If you need more, just change this value
// and recompile the program ;-)
#define MAX_CITIES 100
// This value means infinity :-)
#ifndef INFINITY
	#define INFINITY 1.7E+308
#endif
// This is string, which represents infinite value in table
#define INFSTR "-----"

#endif // GLOBALS_H
