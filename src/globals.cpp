/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2011 Lёppa <contacts[at]oleksii[dot]name>
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
