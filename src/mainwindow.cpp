/*
 *	TSPSG - TSP Solver and Generator
 *	Copyright (C) 2007 Lёppa <lacontacts[at]gmail[dot]com>
 *
 *	$Id$
 *	$URL$
 *
 *	This file is part of TSPSG.
 *
 *	TSPSG is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	TSPSG is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with TSPSG.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui>
#include "mainwindow.h"

// TODO: Saving window state on close

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	connect(actionSettingsSettings,SIGNAL(triggered()),this,SLOT(ChangeSettings()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(CitiesNumberChanged(int)));
	// Centering MainWindow
	// TODO: Loading of saved window state
QRect rect = geometry();
    rect.moveCenter(QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).center());
	setGeometry(rect);
	for (int k = 0; k < tableTask->rowCount(); k++) {
		QTableWidgetItem *item = new QTableWidgetItem(trUtf8("Город ")+QVariant(k + 1).toString());
		tableTask->setVerticalHeaderItem(k,item);
	}
	for (int k = 0; k < tableTask->columnCount(); k++) {
		QTableWidgetItem *item = new QTableWidgetItem(trUtf8("Город ")+QVariant(k + 1).toString());
		tableTask->setHorizontalHeaderItem(k,item);
	}
	tableTask->resizeRowsToContents();
	tableTask->resizeColumnsToContents();
}

void MainWindow::CitiesNumberChanged(int n)
{
	// Adding row, setting header and adjusting its size
bool increased = tableTask->rowCount() < n ? true : false;
	tableTask->setRowCount(n);
QTableWidgetItem *item = new QTableWidgetItem(trUtf8("Город ")+QVariant(n).toString());
	tableTask->setVerticalHeaderItem(n - 1,item);
	if (increased)
		tableTask->resizeRowToContents(n - 1);
	// Adding column, setting header and adjusting its size
	increased = tableTask->columnCount() < n ? true : false;
	tableTask->setColumnCount(n);
	item = new QTableWidgetItem(trUtf8("Город ")+QVariant(n).toString());
	tableTask->setHorizontalHeaderItem(n - 1,item);
	if (increased)
	tableTask->resizeColumnToContents(n - 1);
	tableTask->setMinimumSize(tableTask->sizeHint());
}

void MainWindow::ChangeSettings()
{
SettingsDialog sd(this);
	sd.exec();
}
