/*
 *  TSPSG - TSP Solver and Generator
 *  Copyright (C) 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#ifdef Q_OS_WINCE
	#include "ui_mainwindow.ce.h"
#else
	#include "ui_mainwindow.h"
#endif // Q_OS_WINCE
#include "settingsdialog.h"
#include "tspsolver.h"
#include "tspmodel.h"

class MainWindow: public QMainWindow, Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
#ifndef Q_OS_WINCE
	void closeEvent(QCloseEvent *event);
#endif // Q_OS_WINCE
private slots:
	void ChangeSettings();
	void showAbout();
#ifndef Q_OS_WINCE
	void PrintSetup();
#endif // Q_OS_WINCE
	void Solve();
	void Random();
	void CitiesNumberChanged(int n);
private:
	CTSPModel *tspmodel;
};

#endif // MAINWINDOW_H
