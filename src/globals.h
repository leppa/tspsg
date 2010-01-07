/*!
 * \file globals.h
 * \author Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Contains TSPSG global defines.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
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
//! Default minimum for random numbers generation
#define DEF_RAND_MIN 1
//! Default maximum for random numbers generation
#define DEF_RAND_MAX 10
//! Default number of cities
#define DEF_NUM_CITIES 5
/*!
 * \def DEF_USE_NATIVE_DIALOGS
 * \brief Default for "Use native file dialog".
 */
#ifdef Q_OS_WINCE
	#define DEF_USE_NATIVE_DIALOGS false
#else
	#define DEF_USE_NATIVE_DIALOGS true
#endif // Q_OS_WINCE
//! Default for "Automatically resize rows and columns to their contents"
#define DEF_AUTOSIZE true
//! Default for "Save main window state and position"
#define DEF_SAVEPOS false
//! Default for "Fractional random values"
#define DEF_FRACTIONAL_RANDOM false
//! Default for "Show solution steps' matrices for every solution step"
#define DEF_SHOW_MATRIX true
//! Default for "Show or hide solution steps' matrices based on number of cities in the task"
#define DEF_USE_SHOW_MATRIX_LIMIT true
//! Default for "Maximum number of cities to show solution steps' matrices"
#define DEF_SHOW_MATRIX_LIMIT 15
//! Default for "Scroll to the end of output after solving"
#define DEF_SCROLL_TO_END true
//! Default font name
#define DEF_FONT_FAMILY "Courier New"
//! Default font size
#define DEF_FONT_SIZE 10
//! Default font color
#define DEF_FONT_COLOR Qt::black

//! Maximum available number of cities
#define MAX_NUM_CITIES 50
//! Maximum allowed value for random generation limits
#define MAX_RAND_VALUE 1000

// Paths
/*!
 * \def PATH_I18N
 * \brief Bath to internationalization files.
 */
/*!
 * \def PATH_DOCS
 * \brief Bath to documentation files.
 */
#if defined(Q_OS_LINUX) || defined(Q_OS_UNIX)
	#define PATH_I18N "/usr/share/tspsg/i18n"
	#define PATH_DOCS "/usr/share/doc/tspsg"
#else
	#define PATH_I18N "i18n"
	#define PATH_DOCS "help"
#endif // Q_OS_LINUX

//! TSPSG Task file signature - letters \c TSPT
#define TSPT quint32(0x54535054)
//! TSPSG Task file version
#define TSPT_VERSION quint8(1)
//! TSPSG Task file metadata version
#define TSPT_META_VERSION quint8(1)
//! TSPSG Task file metadata size in bytes (incl. version)
#define TSPT_META_SIZE 2
//! ZKomModRd Task file signature - letters \c ZK
#define ZKT quint16(0x5A4B)
//! ZKomModRd Task file version
#define ZKT_VERSION quint8(1)

/*!
 * \def INFINITY
 * \brief This value means infinity :-)
 *
 *  Some libraries already have \c INFINITY defined.
 *  We need to redefine it for the \c INFINITY to always have the same value.
 */
#ifdef INFINITY
	#undef INFINITY
#endif
#define INFINITY 1.7E+308
//! This string represents infinite value in the table
#define INFSTR "---"

// FUNCTIONS
/*!
 * \brief Checks whether \a x contains an integer value.
 * \param x A value to check.
 * \return \c true if \a x countains an integer, oherwise \c false.
 */
inline bool isInteger(double x)
{
double i;
	return (modf(x, &i) == 0.0);
}

// Sanity checks
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
