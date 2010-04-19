/*!
 * \file mainwindow.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Defines MainWindow class.
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "globals.h"

#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include "tspsolver.h"
#include "tspmodel.h"

/*!
 * \brief Class for handling Main Window UI and logic.
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 */
class MainWindow: public QMainWindow, Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
// Actions
	void actionFileNewTriggered();
	void actionFileOpenTriggered();
	bool actionFileSaveTriggered();
	void actionFileSaveAsTaskTriggered();
	void actionFileSaveAsSolutionTriggered();
#ifndef QT_NO_PRINTER
	void actionFilePrintPreviewTriggered();
	void actionFilePrintTriggered();
#endif // QT_NO_PRINTER
	void actionSettingsPreferencesTriggered();
	void actionSettingsLanguageAutodetectTriggered(bool checked);
	void groupSettingsLanguageListTriggered(QAction *action);
#ifdef Q_OS_WIN32
	void actionHelpCheck4UpdatesTriggered();
#endif // Q_OS_WIN32
	void actionHelpAboutTriggered();
// Buttons
	void buttonBackToTaskClicked();
	void buttonRandomClicked();
	void buttonSolveClicked();

	void dataChanged();
	void dataChanged(const QModelIndex &tl, const QModelIndex &br);
#ifdef Q_OS_WINCE_WM
	void changeEvent(QEvent *ev);
	void desktopResized(int screen);
#endif // Q_OS_WINCE_WM
	void numCitiesChanged(int nCities);
#ifndef QT_NO_PRINTER
	void printPreview(QPrinter *printer);
#endif // QT_NO_PRINTER
	void spinCitiesValueChanged(int nCities);

private:
	QString fileName;
	QActionGroup *groupSettingsLanguageList;
#ifndef QT_NO_PRINTER
	QPrinter *printer;
	QAction *actionFilePrintPreview;
	QAction *actionFilePrint;
#endif // QT_NO_PRINTER
#ifdef Q_OS_WIN32
	QAction *actionHelpCheck4Updates;
#endif // Q_OS_WIN32
	QSettings *settings;
	CTSPModel *tspmodel;
#ifdef Q_OS_WINCE_WM
	QRect currentGeometry;
#endif // Q_OS_WINCE_WM

	// Formats
	QTextTableFormat fmt_table;
	QTextBlockFormat fmt_paragraph,
		fmt_center;
	QTextCharFormat fmt_default,
		fmt_selected,
		fmt_alternate,
		fmt_altlist;

	void closeEvent(QCloseEvent *ev);
	bool hasUpdater() const;
	void initDocStyleSheet();
	void loadLangList();
	bool loadLanguage(const QString &lang = QString());
	bool maybeSave();
	void outputMatrix(QTextCursor &cur, const TMatrix &matrix);
	void outputMatrix(QTextCursor &cur, const SStep &step);
	void retranslateUi(bool all = true);
	bool saveTask();
	void setFileName(const QString &fileName = tr("Untitled") + ".tspt");
	void setupUi();
	void toggleSolutionActions(bool enable = true);
	void toggleTranclucency(bool enable);
};

#endif // MAINWINDOW_H
