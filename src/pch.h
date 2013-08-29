/*!
 * \file pch.h
 * \author Copyright (C) 2007-2013 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief Precompiled Header (PCH) file
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

#ifdef __cplusplus
#   include "globals.h"
#   include <QtCore>
#   include <QtGui>
#   if defined(QT_NO_SVG) && !defined(NOSVG)
#       define NOSVG
#   endif
#   if !defined(NOSVG)
#       include <QtSvg>
#   endif // NOSVG
#   if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#       include <QtConcurrent>
#       include <QtPrintSupport>
#   endif
#   ifndef HANDHELD
#       include "qttoolbardialog.h"
#   endif
#endif // __cplusplus
