/*
 *  TSPSG - TSP Solver and Generator
 *  Copyright (C) 2007 Lёppa <lacontacts[at]gmail[dot]com>
 *
 *  $Id$
 *  $URL$
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

#include <QDialog>
#include "ui_settingsdialog.h"

class SettingsDialog: public QDialog, public Ui::SettingsDialog
{
	Q_OBJECT
public:
	SettingsDialog(QWidget *parent = 0);

private:
	bool event(QEvent *);

private slots:
	void spinRandMinValueChanged(int val) { spinRandMax->setMinimum(val + 1); }
};

#endif // SETTINGSDIALOG_H

