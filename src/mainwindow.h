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
#include <QSettings>
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
	void closeEvent(QCloseEvent *event);
private slots:
	void actionFileNewTriggered();
	void actionSettingsPreferencesTriggered();
	void actionSettingsLanguageAutodetectTriggered(bool checked);
	void groupSettingsLanguageListTriggered(QAction *action);
	void actionHelpAboutTriggered();
#ifndef Q_OS_WINCE
	void actionFilePrintSetupTriggered();
#endif // Q_OS_WINCE
	void buttonSolveClicked();
	void buttonRandomClicked();
	void spinCitiesValueChanged(int n);
private:
	QSettings *settings;
	QPrinter *printer;
	CTSPModel *tspmodel;
	QActionGroup *groupSettingsLanguageList;
	bool loadLanguage(QString lang = "");
	void loadLangList();
};

#endif // MAINWINDOW_H
