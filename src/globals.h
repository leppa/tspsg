/*!
 * \file globals.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
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
#if !defined(NOSVG) && (QT_VERSION >= 0x040500)
	#include <QtSvg>
#endif // NOSVG && QT_VERSION >= 0x040500

// Version info
#include "version.h"
// OS and ARCH detection
#include "os.h"
// TSPSG Defaults
#include "defaults.h"
// TSPSolver
#include "tspsolver.h"
#ifdef Q_OS_WIN32
	// Vista/7 Eyecandy
	#include "qtwin.h"
#endif // Q_OS_WIN32

// DEFINES
//! Maximum available number of cities
#define MAX_NUM_CITIES 50
//! Maximum allowed value for random generation limits
#define MAX_RAND_VALUE 1000

// Paths
/*!
 * \def PATH_L10N
 * \brief Path to internationalization files.
 */
#ifndef PATH_L10N
	#define PATH_L10N "l10n"
#endif // PATH_L10N
/*!
 * \def PATH_DOCS
 * \brief Bath to documentation files.
 */
#ifndef PATH_DOCS
	#define PATH_DOCS "help"
#endif // PATH_DOCS

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

#ifdef Q_OS_WIN32
/*!
 * \brief Enables or disables a mask for the \a widget.
 * \param widget A widget to toggle mask on.
 * \param enable Set to \c true to enable mask or \c false to disable it.
 *
 *  This function is used to enable an outlined font effect for \c QLabel with a static text.
 */
void toggleStyle(QWidget *widget, bool enable);
#endif // Q_OS_WIN32

#if defined(Q_OS_WINCE_WM) || defined(Q_OS_SYMBIAN)
	//! This is defined on handheld (e.g., Windows Mobile, Symbian) devices.
	#define HANDHELD
#endif

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
