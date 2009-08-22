/*
 *  TSPSG: TSP Solver and Generator
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

#include "globals.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "tspsolver.h"
#include "tspmodel.h"

class MainWindow: public QMainWindow, Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	void closeEvent(QCloseEvent *);

private slots:
	void actionFileNewTriggered();
	void actionFileOpenTriggered();
	void actionFileSaveTriggered();
	void actionFileSaveAsTaskTriggered();
	void actionFileSaveAsSolutionTriggered();
	void actionSettingsPreferencesTriggered();
	void actionSettingsLanguageAutodetectTriggered(bool);
	void groupSettingsLanguageListTriggered(QAction *);
	void actionHelpAboutTriggered();
	void dataChanged();
	void dataChanged(const QModelIndex &, const QModelIndex &);
#ifndef QT_NO_PRINTER
	void printPreview(QPrinter *);
	void actionFilePrintPreviewTriggered();
	void actionFilePrintTriggered();
#endif // QT_NO_PRINTER
	void buttonSolveClicked();
	void buttonRandomClicked();
	void buttonBackToTaskClicked();
	void spinCitiesValueChanged(int);
	void numCitiesChanged(int);

private:
	QSettings *settings;
#ifndef QT_NO_PRINTER
	QPrinter *printer;
#endif // QT_NO_PRINTER
	CTSPModel *tspmodel;
	QString fileName;
	QActionGroup *groupSettingsLanguageList;
	QStringList output;
	void enableSolutionActions(bool enable = true);
	void initDocStyleSheet();
	bool loadLanguage(QString lang = "");
	void loadLangList();
	bool maybeSave();
	void outputMatrix(tMatrix, QStringList &, int nRow = -1, int nCol = -1);
	void setFileName(QString fileName = trUtf8("Untitled") + ".tspt");
	bool saveTask();
};

#endif // MAINWINDOW_H
