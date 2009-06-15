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
#include "mainwindow.h"

// TODO: Saving window state on close

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), randMin(1), randMax(10)
{
	setupUi(this);
	connect(actionSettingsSettings,SIGNAL(triggered()),this,SLOT(ChangeSettings()));
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
	// Centering MainWindow
	// TODO: Loading of saved window state
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
#endif // Q_OS_WINCE
	setGeometry(rect);
	qsrand(QDateTime().currentDateTime().toTime_t());
	tspmodel = new CTSPModel();
	tspmodel->randMin = randMin;
	tspmodel->randMax = randMax;
	tspmodel->setNumCities(spinCities->value());
	taskView->setModel(tspmodel);
}

void MainWindow::CitiesNumberChanged(int n)
{
	tspmodel->setNumCities(n);
}

void MainWindow::ChangeSettings()
{
SettingsDialog sd(this);
	sd.spinRandMin->setValue(randMin);
	sd.spinRandMax->setValue(randMax);
	if (sd.exec() == QDialog::Accepted) {
		randMin = sd.spinRandMin->value();
		randMax = sd.spinRandMax->value();
	}
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
