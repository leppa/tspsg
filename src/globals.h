/*!
 * \file globals.h
 * \author Copyright &copy; 2007-2012 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
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
#if QT_VERSION < QT_VERSION_CHECK(4,5,0)
#   error You are using Qt version < 4.5 but minimum required version is 4.5.0. Compilation aborted.
#endif
#if defined(Q_WS_WINCE_WM) || defined(Q_WS_S60) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
    //! This is defined on handheld devices (e.g., Windows Mobile, Symbian).
#   define HANDHELD
#   define QT_NO_STATUSTIP
#endif
#include <QtGui>
#if defined(QT_NO_SVG) && !defined(NOSVG)
#   define NOSVG
#endif
#if !defined(NOSVG)
#   include <QtSvg>
#endif // NOSVG
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QtConcurrent>
#   include <QtPrintSupport>
#endif

#ifndef HANDHELD
#   include "qttoolbardialog.h"
#endif

// Version info
#include "version.h"
// OS and ARCH detection
#include "os.h"
// TSPSG Defaults
#include "defaults.h"
// TSPSolver
#include "tspsolver.h"
#ifndef HANDHELD
    // Eyecandy
#   include "qtwin.h"
#endif // HANDHELD

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
#   define PATH_L10N "l10n"
#endif // PATH_L10N
/*!
 * \def PATH_DOCS
 * \brief Bath to documentation files.
 */
#ifndef PATH_DOCS
#   define PATH_DOCS "help"
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

//! Number of centimeters in 1 inch
#define CM_IN_INCH 2.54
//! Factor for high quality graph generation
#define HQ_FACTOR 2

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

/*!
 * \brief Checks whether the updater app is installed/available.
 * \return \c true if updater app is available, oherwise \c false.
 * \note The updater app is only available under Windows at this moment.
 *  On other systems this function always returns \c false.
 */
inline bool hasUpdater()
{
#ifdef Q_WS_WIN32
    return QFile::exists("updater/Update.exe");
#else // Q_WS_WIN32
    return false;
#endif // Q_WS_WIN32
}

/*!
 * \brief Converts \a in into Base64 format with lines wrapped at 64 characters.
 * \param in A byte array to be converted.
 * \return Converted byte array.
 */
inline QByteArray toWrappedBase64(const QByteArray &in)
{
    QByteArray out, base64(in.toBase64());
    for (int i = 0; i <= base64.size() - 64; i += 64) {
        out.append(QByteArray::fromRawData(base64.data() + i, 64)).append('\n');
    }
    if (int rest = base64.size() % 64)
        out.append(QByteArray::fromRawData(base64.data() + base64.size() - rest, rest));
    return out;
}

#ifndef HANDHELD
/*!
 * \brief Enables or disables a mask for the \a widget.
 * \param widget A widget to toggle mask on.
 * \param enable Set to \c true to enable mask or \c false to disable it.
 *
 *  This function is used to enable an outlined font effect for \c QLabel with a static text.
 */
void toggleStyle(QWidget *widget, bool enable);
#endif // HANDHELD

#ifndef DOXYGEN_EXCLUDE

#ifndef QT_NO_PRINTER
    Q_DECLARE_METATYPE(QPrinter::PageSize)
    Q_DECLARE_METATYPE(QPrinter::Orientation)
#endif

#ifdef HANDHELD
#   define ICON_SIZE "48x48"
#   define ICON_FORMAT "png"
#else
#   define ICON_SIZE "128x128"
#   define ICON_FORMAT "png"
#endif

#if QT_VERSION >= QT_VERSION_CHECK(4,6,0)
#   define GET_ICON(x) QIcon::fromTheme(x, QIcon(":/images/icons/"ICON_SIZE"/"x"."ICON_FORMAT))
#else
#   define GET_ICON(x) QIcon(":/images/icons/"ICON_SIZE"/"x"."ICON_FORMAT)
// No QVariant::toReal() member in Qt < 4.6
// A quick hack to maintain compatibility with Qt 4.5.x
#   define toReal toDouble
#endif

// Sanity checks
// Check that default number of cities is sane (<= MAX_NUM_CITIES)
#if DEF_NUM_CITIES > MAX_NUM_CITIES
#   undef DEF_NUM_CITIES
#   define DEF_NUM_CITIES MAX_NUM_CITIES
#endif
// Check that maximum for random generation is sane (<= MAX_RAND_VALUE)
#if DEF_RAND_MAX > MAX_RAND_VALUE
#   undef DEF_RAND_MAX
#   define DEF_RAND_MAX MAX_RAND_VALUE
#endif
// Check that DEF_RAND_MIN <= DEF_RAND_MAX
#if DEF_RAND_MIN > DEF_RAND_MAX
#   undef DEF_RAND_MIN
#   define DEF_RAND_MIN DEF_RAND_MAX
#endif

#endif // DOXYGEN_EXCLUDE

#endif // GLOBALS_H
