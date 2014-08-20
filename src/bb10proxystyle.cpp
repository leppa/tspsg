/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
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

#include "bb10proxystyle.h"

BB10ProxyStyle::BB10ProxyStyle(QStyle *style)
    : QProxyStyle(style) {}

int BB10ProxyStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    switch (metric) {
    case QStyle::PM_ButtonIconSize:
        return 42;
    case QStyle::PM_SmallIconSize:
        return 42;
    case QStyle::PM_LargeIconSize:
        return 64;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}

int BB10ProxyStyle::styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    if (hint == QStyle::SH_UnderlineShortcut)
        return 0;
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
