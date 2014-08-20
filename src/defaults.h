/*!
 * \file defaults.h
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief Contains TSPSG defaults.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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
//! Default for "Remember last used directories"
#define DEF_SAVE_LAST_USED true
/*!
 * \def DEF_USE_NATIVE_DIALOGS
 * \brief Default for "Use native file dialog".
 */
#ifdef Q_OS_WINCE_WM
#   define DEF_USE_NATIVE_DIALOGS false
#else
#   define DEF_USE_NATIVE_DIALOGS true
#endif // Q_OS_WINCE_WM
//! Default for "Save main window state and position"
#define DEF_SAVEPOS true
/*!
 * \def DEF_USE_TRANSLUCENCY
 * \brief Default for "Use translucency effects"
 */
#ifdef Q_OS_WIN32
#   define DEF_USE_TRANSLUCENCY true
#else
#   define DEF_USE_TRANSLUCENCY false
#endif
//! Default for "Check for updates"
#define DEF_CHECK_FOR_UPDATES false
//! Default update check interval (in days)
#define DEF_UPDATE_CHECK_INTERVAL 30

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

//! Default for "Generate solution graph"
#define DEF_GENERATE_GRAPH true
//! Default for "Save solution graph as"
#define DEF_GRAPH_IMAGE_FORMAT "png"
//! Default for "Draw solution graph in higher quality"
#define DEF_HQ_GRAPH false
//! Default for "Embed solution graph into HTML"
#define DEF_EMBED_GRAPH_INTO_HTML false
/*!
 * \def DEF_GRAPH_WIDTH
 * \brief Default solution graph size in centimeters (HQ size is this size * HQ_FACTOR)
 */
#ifdef Q_OS_BLACKBERRY
#   define DEF_GRAPH_WIDTH 2.5
#else
#   define DEF_GRAPH_WIDTH 4.5
#endif
//! Default for "Show solution steps' matrices for every solution step"
#define DEF_SHOW_MATRIX true
//! Default for "Show or hide solution steps' matrices based on number of cities in the task"
#define DEF_USE_SHOW_MATRIX_LIMIT true
//! Default for "Maximum number of cities to show solution steps' matrices"
#define DEF_SHOW_MATRIX_LIMIT 15
//! Default for "Scroll to the end of output after solving"
#define DEF_SCROLL_TO_END false
//! Default font face
#define DEF_FONT_FACE "DejaVu LGC Sans Mono"
/*!
 * \def DEF_FONT_SIZE
 * \brief Default font size
 */
#ifdef Q_OS_SYMBIAN
#   define DEF_FONT_SIZE 8
#elif defined(Q_OS_BLACKBERRY)
#   define DEF_FONT_SIZE 7
#else
#   define DEF_FONT_SIZE 10
#endif // Q_OS_SYMBIAN
/*!
 * \def DEF_TEXT_COLOR
 * \brief Default solution text color
 */
#ifdef Q_OS_BLACKBERRY
#   define DEF_TEXT_COLOR Qt::black
#else
#   define DEF_TEXT_COLOR QApplication::palette().color(QPalette::Text)
#endif
//! Default selected candidate color
#define DEF_SELECTED_COLOR "#008000"
//! Default alternate candidate color
#define DEF_ALTERNATE_COLOR "#A00000"
//! Default table border color
#define DEF_TABLE_COLOR QApplication::palette().color(QPalette::AlternateBase)
////! Default solution background color
//#define DEF_BACKGROUND_COLOR QApplication::palette().color(QPalette::Base)

#ifndef QT_NO_PRINTER
//! Default page size for printing
#   define DEF_PAGE_SIZE QPrinter::A4
//! Default page orientation for printing
#   define DEF_PAGE_ORIENTATION QPrinter::Portrait
//! Default left page margin for printing
#   define DEF_MARGIN_LEFT 25
//! Default top page margin for printing
#   define DEF_MARGIN_TOP 15
//! Default right page margin for printing
#   define DEF_MARGIN_RIGHT DEF_MARGIN_TOP
//! Default bottom page margin for printing
#   define DEF_MARGIN_BOTTOM DEF_MARGIN_TOP
#endif

#endif // DEFAULTS_H
