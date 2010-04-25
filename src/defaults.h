/*!
 * \file defaults.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Contains TSPSG defaults.
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

#ifndef DEFAULTS_H
#define DEFAULTS_H

//! Default number of cities
#define DEF_NUM_CITIES 5

//////// GENERAL

//! Default for "Automatically resize rows and columns to their contents"
#define DEF_AUTOSIZE true
/*!
 * \def DEF_USE_NATIVE_DIALOGS
 * \brief Default for "Use native file dialog".
 */
#ifdef Q_OS_WINCE
	#define DEF_USE_NATIVE_DIALOGS false
#else
	#define DEF_USE_NATIVE_DIALOGS true
#endif // Q_OS_WINCE
//! Default for "Save main window state and position"
#define DEF_SAVEPOS true
//! Default for "Use translucency effects in Vista/7"
#define DEF_USE_TRANSLUCENCY true

//////// TASK

//! Default for "Accuracy of the fractional values"
#define DEF_FRACTIONAL_ACCURACY 2
//! Default minimum for random numbers generation
#define DEF_RAND_MIN 1
//! Default maximum for random numbers generation
#define DEF_RAND_MAX 10
//! Default for "Fractional random values"
#define DEF_FRACTIONAL_RANDOM false
//! Default for "Symmetric tasks mode"
#define DEF_SYMMETRIC_MODE false

//////// OUTPUT

//! Default for "Show solution graph"
#define DEF_SHOW_GRAPH true
//! Default for "Show solution steps' matrices for every solution step"
#define DEF_SHOW_MATRIX true
//! Default for "Show or hide solution steps' matrices based on number of cities in the task"
#define DEF_USE_SHOW_MATRIX_LIMIT true
//! Default for "Maximum number of cities to show solution steps' matrices"
#define DEF_SHOW_MATRIX_LIMIT 15
//! Default for "Scroll to the end of output after solving"
#define DEF_SCROLL_TO_END false
/*!
 * \def DEF_FONT_FAMILY
 * \brief Default font name.
 */
#ifdef Q_OS_WINCE_WM
	#define DEF_FONT_FAMILY "Tahoma"
#else
	#define DEF_FONT_FAMILY "Courier New"
#endif // Q_OS_WINCE_WM
/*!
 * \def DEF_FONT_SIZE
 * \brief Default font size
 */
#ifdef Q_OS_SYMBIAN
	#define DEF_FONT_SIZE 8
#else
	#define DEF_FONT_SIZE 10
#endif // Q_OS_SYMBIAN
//! Default text color
#define DEF_TEXT_COLOR QPalette().color(QPalette::Text)
//! Default selected candidate color
#define DEF_SELECTED_COLOR QColor(0x008000)
//! Default alternate candidate color
#define DEF_ALTERNATE_COLOR QColor(0xA00000)

#endif // DEFAULTS_H
