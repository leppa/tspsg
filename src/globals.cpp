/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2012 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
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

#include "globals.h"

#ifdef Q_OS_WINCE_WM
#   include <shellapi.h>
#endif

QSettings *initSettings(QObject *parent)
{
#ifdef Q_OS_WINCE_WM
    /*!
     * \hack HACK: On Windows Mobile the way Qt tries to get path for saving
     *  settings doesn't always work. This workaround tries to fix it.
     */
    if (!QDesktopServices::storageLocation(QDesktopServices::DataLocation).isEmpty()) {
#endif // Q_OS_WINCE_WM
    return new QSettings(QSettings::IniFormat, QSettings::UserScope, "TSPSG", "tspsg", parent);
#ifdef Q_OS_WINCE_WM
    } else {
        wchar_t path[MAX_PATH];
        SHGetSpecialFolderPath(0, path, 0x001a, FALSE);
        QString fileName = QString::fromWCharArray(path);
        fileName.append("\\TSPSG\\tspsg.ini");
        return new QSettings(fileName, QSettings::IniFormat, parent);
    }
#endif // Q_OS_WINCE_WM
}

#ifndef HANDHELD
void toggleStyle(QWidget *widget, bool enable)
{
    if (enable) {
        widget->setStyleSheet(QString("%1 {background-color: %2; border-color: %3; border-width: 1px; border-style: solid; border-radius: 3px;}").arg(widget->metaObject()->className(), widget->palette().window().color().name(), widget->palette().shadow().color().name()));
    } else {
        widget->setStyleSheet(QString());
    }
}
#endif // HANDHELD
