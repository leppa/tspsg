/*!
 * \file mainwindow.h
 * \author Copyright &copy; 2007-2011 Lёppa <contacts[at]oleksii[dot]name>
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

#include "tspmodel.h"

#ifdef Q_WS_WIN32
    // Forward declaration. A real one is in shobjidl.h
    struct ITaskbarList3;
#endif

using namespace TSPSolver;

/*!
 * \brief Class for handling Main Window UI and logic.
 * \author Copyright &copy; 2007-2011 Lёppa <contacts[at]oleksii[dot]name>
 */
class MainWindow: public QMainWindow, Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

#ifdef Q_OS_BLACKBERRY
public slots:
    void setWindowModified(bool modified);
#endif

private slots:
// Actions
    void actionFileNewTriggered();
    void actionFileOpenTriggered();
    bool actionFileSaveTriggered();
    void actionFileSaveAsTaskTriggered();
    void actionFileSaveAsSolutionTriggered();
#ifndef QT_NO_PRINTDIALOG
    void actionFilePrintPreviewTriggered();
    void actionFilePageSetupTriggered();
    void actionFilePrintTriggered();
#endif // QT_NO_PRINTDIALOG
    void actionSettingsPreferencesTriggered();
    void actionSettingsLanguageAutodetectTriggered(bool checked);
    void groupSettingsLanguageListTriggered(QAction *action);
    void actionSettingsStyleSystemTriggered(bool checked);
    void groupSettingsStyleListTriggered(QAction *action);
#ifndef HANDHELD
    void actionSettingsToolbarsConfigureTriggered();
#endif // HANDHELD
    void actionHelpOnlineSupportTriggered();
    void actionHelpReportBugTriggered();
    void actionHelpCheck4UpdatesTriggered();
    void actionHelpAboutTriggered();
// Buttons
    void buttonBackToTaskClicked();
    void buttonRandomClicked();
    void buttonSolveClicked();

    void dataChanged();
    void dataChanged(const QModelIndex &tl, const QModelIndex &br);
#ifdef Q_WS_WINCE_WM
    void changeEvent(QEvent *ev);
    void desktopResized(int screen);
#endif // Q_WS_WINCE_WM
    void numCitiesChanged(int nCities);
#ifndef QT_NO_PRINTER
    void printPreview(QPrinter *printer);
#endif // QT_NO_PRINTER
#ifdef Q_WS_WIN32
    void solverRoutePartFound(int n);
#endif // Q_WS_WIN32
    void spinCitiesValueChanged(int nCities);

private:
    QString fileName;
    QActionGroup *groupSettingsLanguageList;
    QActionGroup *groupSettingsStyleList;
#ifndef HANDHELD
    QAction *actionSettingsToolbarsConfigure;
    QtToolBarManager *toolBarManager;
#endif // HANDHELD
#ifndef QT_NO_PRINTER
    QPrinter *printer;
#ifndef QT_NO_PRINTDIALOG
    QAction *actionFilePrintPreview;
    QAction *actionFilePageSetup;
    QAction *actionFilePrint;
#endif // QT_NO_PRINTDIALOG
#endif // QT_NO_PRINTER
    QAction *actionHelpCheck4Updates;
#ifdef Q_WS_S60
    QAction *actionRightSoftKey;
#endif
    QSettings *settings;
    CTSPModel *tspmodel;
#ifdef Q_WS_WINCE_WM
    QRect currentGeometry;
#endif // Q_WS_WINCE_WM

#ifdef Q_WS_WIN32
    ITaskbarList3 *tl;
#endif // Q_WS_WIN32

    // The solution graph SVG
    QPicture graph;

    // Formats
    QTextTableFormat fmt_table;
    QTextBlockFormat fmt_paragraph,
        fmt_lastparagraph,
        fmt_cell;
    QTextCharFormat fmt_default,
        fmt_selected,
        fmt_alternate,
        fmt_altlist;

    void check4Updates(bool silent = false);
    void closeEvent(QCloseEvent *ev);
    void dragEnterEvent(QDragEnterEvent *ev);
    void drawNode(QPainter &pic, int nstep, bool left = false, SStep *step = NULL);
    void dropEvent(QDropEvent *ev);
    void initDocStyleSheet();
    void loadLangList();
    bool loadLanguage(const QString &lang = QString());
    void loadStyleList();
    void loadToolbarList();
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

#ifdef Q_WS_S60
// A quickly hacked QMessageBox for Symbian that supports three buttons.
class QSMessageBox: public QMessageBox {
    Q_OBJECT
private slots:
    void cancel();
    void discard();
public:
    QSMessageBox(QWidget *parent = 0);
};
#endif // Q_WS_S60

#endif // MAINWINDOW_H
