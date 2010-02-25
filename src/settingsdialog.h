/*!
 * \file settingsdialog.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Defines SettingsDialog class.
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "globals.h"

#include "ui_settingsdialog.h"

/*!
 * \brief Class for handling Settings Dialog UI and logic.
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 */
class SettingsDialog: public QDialog, Ui::SettingsDialog
{
	Q_OBJECT
public:
	SettingsDialog(QWidget *parent = 0);
	bool colorChanged() const;
	bool fontChanged() const;
	qint8 translucencyChanged() const;

private:
	bool _newFont;
	bool _newColor;
	qint8 _translucency;

	QColor color;
	QFont font;

	QSettings *settings;
	QHBoxLayout *layoutCitiesLimit;
#ifdef Q_OS_WIN32
	QCheckBox *cbUseTranslucency;
#endif // Q_OS_WIN32
#ifdef Q_OS_WINCE
	QRect currentGeometry;
#elif !defined(Q_OS_SYMBIAN)
	QCheckBox *cbSaveState;
	QLabel *imgIcon;
	QFrame *lineVertical;
	QLabel *labelHint;

	bool event(QEvent *ev);
#endif // Q_OS_WINCE

private slots:
	void accept();
	void buttonColorClicked();
	void buttonFontClicked();
#ifdef Q_OS_WINCE
	void desktopResized(int screen);
#endif // Q_OS_WINCE
	void spinRandMinValueChanged(int val);
};

#endif // SETTINGSDIALOG_H
