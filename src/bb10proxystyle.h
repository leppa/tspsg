/*!
 * \file bb10proxystyle.h
 * \author Copyright &copy; 2007-2013 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief Defines BB10ProxyStyle class.
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

#ifndef BB10PROXYSTYLE_H
#define BB10PROXYSTYLE_H

#include <QProxyStyle>

class BB10ProxyStyle : public QProxyStyle
{
public:
    BB10ProxyStyle(QStyle *style = 0);

    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;
};

#define CSS_FIX \
"QMenuBar {"\
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #444 stop:0.65 #333, stop:1 #444);"\
"    border-bottom: 1px solid darkgray;"\
"    spacing: 1px;"\
"}"\
"QMenuBar::item {"\
"    padding: 0.3em 0.5em;"\
"}"\
"QMenuBar::item:selected {"\
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00a8df stop:0.65 #00889f, stop:1 #00a8df);"\
"}"\
"QMenu {"\
"    border: 1px solid darkgray;"\
"}"\
"QMenu::item {"\
"    padding: 0.4em 0.5em 0.4em 1.4em;"\
"    color: white;"\
"    font-size: 9pt;"\
"    background-color: #121212;"\
"}"\
"QMenu::item:selected {"\
"    background-color: #00a8df;"\
"}"\
"QMenu::item:disabled {"\
"    color: darkgray;"\
"    background-color: #121212;"\
"}"\
"QMenu::icon {"\
"    margin-left: 0.85em;"\
"}"\
"QTabBar {"\
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #444 stop:0.65 #333, stop:1 #444);"\
"    border-bottom: 1px solid darkgray;"\
"    spacing: 1px;"\
"}"\
"QTabBar::tab {"\
"    padding: 0.3em 0.5em;"\
"}"\
"QTabBar::tab:selected {"\
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #00a8df stop:0.65 #00889f, stop:1 #00a8df);"\
"    padding: 0.3em 0.5em;"\
"}"\
"QTableView {"\
"    gridline-color: darkgray;"\
"}"\
"QTableView::item {"\
"    padding: 0 0.5em;"\
"}"\
"QComboBox {"\
"    padding: 0.5em 1.3em 0.5em 0.5em;"\
"}"\
"QTextBrowser {"\
"    background-color: lightgray;"\
"    color: black;"\
"}"

#endif // BB10PROXYSTYLE_H
