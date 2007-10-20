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

#include <QtGui>
#include "mainwindow.h"

// TODO: Saving window state on close

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), randMin(1), randMax(10)
{
	setupUi(this);
	connect(actionSettingsSettings,SIGNAL(triggered()),this,SLOT(ChangeSettings()));
	connect(buttonSolve,SIGNAL(clicked()),this,SLOT(Solve()));
	connect(buttonRandom,SIGNAL(clicked()),this,SLOT(Random()));
	connect(spinCities,SIGNAL(valueChanged(int)),this,SLOT(CitiesNumberChanged(int)));
	// Centering MainWindow
	// TODO: Loading of saved window state
QRect rect = geometry();
    rect.moveCenter(QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen()).center());
	setGeometry(rect);
	qsrand(QDateTime().currentDateTime().toTime_t());
	PrepareTable();
}

int MainWindow::rand(int min, int max)
{
	return min + (int)(((float)qrand() / RAND_MAX) * max);
}

void MainWindow::PrepareTable()
{
QTableWidgetItem *item;
	for (int y = 0; y < spinCities->value(); y++) {
		item = new QTableWidgetItem(trUtf8("Город ") + QVariant(y + 1).toString());
		tableTask->setVerticalHeaderItem(y,item);
		item = new QTableWidgetItem(trUtf8("Город ") + QVariant(y + 1).toString());
		tableTask->setHorizontalHeaderItem(y,item);
		for (int x = 0; x < spinCities->value(); x++) {
			if (y == x) {
				item = new QTableWidgetItem("...");
				item->setFlags(item->flags() ^ Qt::ItemIsEditable);
			} else {
				item = new QTableWidgetItem(QVariant(rand(randMin,randMax)).toString());
QFont font = item->font();
				font.setBold(true);
				item->setFont(font);
			}
			item->setTextAlignment(Qt::AlignCenter);
			tableTask->setItem(x,y,item);
		}
	}
	tableTask->resizeRowsToContents();
	tableTask->resizeColumnsToContents();
}

void MainWindow::CitiesNumberChanged(int n)
{
bool increased = tableTask->rowCount() < n ? true : false;
	tableTask->setRowCount(n);
	tableTask->setColumnCount(n);
	for (int k = 0; k < n; k++)
//		tableTask->setColumnWidth(k,tableTask->viewport()->width() / n);
	// If number of cities increased we need to reset headers and set
	// appropriate sizes for new column and row
	if (increased) {
QTableWidgetItem *item = new QTableWidgetItem(trUtf8("Город ") + QVariant(n).toString());
		tableTask->setVerticalHeaderItem(n - 1,item);
		item = new QTableWidgetItem(trUtf8("Город ") + QVariant(n).toString());
		tableTask->setHorizontalHeaderItem(n - 1,item);
		tableTask->resizeRowToContents(n - 1);
		tableTask->resizeColumnToContents(n - 1);
		item = new QTableWidgetItem("...");
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		tableTask->setItem(spinCities->value() - 1,spinCities->value() - 1,item);
		for (int k = 0; k < spinCities->value() - 1; k++) {
			item = new QTableWidgetItem(QVariant(rand(randMin, randMax)).toString());
QFont font = item->font();
			font.setBold(true);
			item->setFont(font);
			tableTask->setItem(k,spinCities->value() - 1,item);
			item = new QTableWidgetItem(QVariant(rand(randMin, randMax)).toString());
			font = item->font();
			font.setBold(true);
			item->setFont(font);
			tableTask->setItem(spinCities->value() - 1,k,item);
		}
	}
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

void MainWindow::Random()
{
	for (int y = 0; y < spinCities->value(); y++)
		for (int x = 0; x < spinCities->value(); x++)
			if (x != y)
				tableTask->item(x,y)->setText(QVariant(rand(randMin, randMax)).toString());
}

void MainWindow::Solve()
{
	// tabWidget->setCurrentIndex(1);
CTSPSolver solver;
	solver.solve(spinCities->value(),NULL);
	// TODO: Task solving goes here :-)
}

