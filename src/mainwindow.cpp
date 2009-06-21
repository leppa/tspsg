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

#include <QtGui>
#ifndef Q_OS_WINCE
	#include <QPrintDialog>
#endif // Q_OS_WINCE
#include "defines.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
QSettings settings(INI_FILE,QSettings::IniFormat);
	spinCities->setValue(settings.value("NumCities",5).toInt());
	connect(actionSettingsSettings,SIGNAL(triggered()),this,SLOT(ChangeSettings()));
	connect(actionHelpAbout,SIGNAL(triggered()),this,SLOT(showAbout()));
#ifndef Q_OS_WINCE
	connect(actionFilePrintSetup,SIGNAL(triggered()),this,SLOT(PrintSetup()));
#endif // Q_OS_WINCE
	connect(buttonSolve,SIGNAL(clicked()),this,SLOT(Solve()));
	connect(buttonRandom,SIGNAL(clicked()),this,SLOT(Random()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(CitiesNumberChanged(int)));
QRect rect = geometry();
#ifdef Q_OS_WINCE
	// HACK: Fix for all tabWidget elements becoming "unclickable" if making it central widget.
	rect.setSize(QApplication::desktop()->availableGeometry().size());
	rect.setHeight(rect.height() - (QApplication::desktop()->screenGeometry().height() - QApplication::desktop()->availableGeometry().height()));
	tabWidget->resize(rect.width(),rect.height() - toolBar->size().height());
#else
	if (settings.value("SavePos",false).toBool()) {
		// Loading of saved window state
		settings.beginGroup("MainWindow");
		resize(settings.value("Size",size()).toSize());
		move(settings.value("Position",pos()).toPoint());
		if (settings.value("Maximized",false).toBool())
			setWindowState(windowState() | Qt::WindowMaximized);
		settings.endGroup();
	} else {
		// Centering main window
		rect.moveCenter(QApplication::desktop()->availableGeometry(this).center());
		setGeometry(rect);
	}
#endif // Q_OS_WINCE
	qsrand(QDateTime().currentDateTime().toTime_t());
	tspmodel = new CTSPModel();
	tspmodel->setNumCities(spinCities->value());
	taskView->setModel(tspmodel);
#ifdef Q_OS_WINCE
	taskView->resizeColumnsToContents();
	taskView->resizeRowsToContents();
#endif // Q_OS_WINCE
}

void MainWindow::CitiesNumberChanged(int n)
{
#ifdef Q_OS_WINCE
int count = tspmodel->numCities();
#endif // Q_OS_WINCE
	tspmodel->setNumCities(n);
#ifdef Q_OS_WINCE
	if (n > count)
		for (int k = count; k < n; k++) {
			taskView->resizeColumnToContents(k);
			taskView->resizeRowToContents(k);
		}
#endif // Q_OS_WINCE
}

void MainWindow::ChangeSettings()
{
SettingsDialog sd(this);
	sd.exec();
}

#ifndef Q_OS_WINCE
void MainWindow::PrintSetup()
{
QPrintDialog pd;
	pd.exec();
}
#endif // Q_OS_WINCE

void MainWindow::Random()
{
	tspmodel->randomize();
#ifdef Q_OS_WINCE
	taskView->resizeColumnsToContents();
	taskView->resizeRowsToContents();
#endif // Q_OS_WINCE
}

void MainWindow::Solve()
{
	// TODO: Task solving goes here :-)
tMatrix matrix;
double *row;
int n = spinCities->value();
bool ok;
	for (int r = 0; r < n; r++) {
		row = new double[n];
		for (int c = 0; c < n; c++) {
			row[c] = tspmodel->index(r,c).data(Qt::UserRole).toDouble(&ok);
			if (!ok) {
				QMessageBox(QMessageBox::Critical,trUtf8("Ошибка в данных"),QString(trUtf8("Ошибка в ячейке [Строка %1; Колонка %2]: Неверный формат данных.")).arg(r + 1).arg(c + 1),QMessageBox::Ok,this).exec();
				return;
			}
		}
		matrix.append(row);
	}
CTSPSolver solver;
sStep *root = solver.solve(spinCities->value(),matrix);
	if (!root)
		QMessageBox(QMessageBox::Critical,trUtf8("Ошибка при решении"),trUtf8("Во время решения задачи возникла ошибка"),QMessageBox::Ok,this).exec();
	// tabWidget->setCurrentIndex(1);
}

void MainWindow::showAbout()
{
	// TODO: Normal about window :-)
QString about = QString::fromUtf8("TSPSG - TSP Solver and Generator\n\
    Copyright (C) 2007-%1 Lёppa <contacts[at]oleksii[dot]name>\n\
Qt library versions:\n\
    Compile time: %2\n\
    Runtime: %3\n\
\n\
TSPSG is licensed under the terms of the GNU General Public License. You should have received a copy of the GNU General Public License along with TSPSG.").arg(QDate().toString("%Y"),QT_VERSION_STR,qVersion());
	QMessageBox(QMessageBox::Information,"About",about).exec();
}

#ifndef Q_OS_WINCE
void MainWindow::closeEvent(QCloseEvent *event)
{
	// Saving windows state
QSettings settings(INI_FILE,QSettings::IniFormat);
	settings.setValue("NumCities",spinCities->value());
	if (settings.value("SavePos",false).toBool()) {
		settings.beginGroup("MainWindow");
		settings.setValue("Maximized",isMaximized());
		if (!isMaximized()) {
			settings.setValue("Size",size());
			settings.setValue("Position",pos());
		}
		settings.endGroup();
	}
	QMainWindow::closeEvent(event);
}
#endif // Q_OS_WINCE