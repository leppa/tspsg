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

#include "tspsolver.h"
#include "tspmodel.h"

CTSPSolver::CTSPSolver()
	: nCities(0)
{
}

void CTSPSolver::cleanup()
{
	route.clear();
	mayNotBeOptimal = false;
}

double CTSPSolver::findMinInRow(int nRow, tMatrix matrix, int exc)
{
double min = INFINITY;
	for (int k = 0; k < nCities; k++)
		if (((k != exc)) && (min > matrix.at(nRow).at(k)))
			min = matrix.at(nRow).at(k);
	return min == INFINITY ? 0 : min;
}

double CTSPSolver::findMinInCol(int nCol, tMatrix matrix, int exr)
{
double min = INFINITY;
	for (int k = 0; k < nCities; k++)
		if ((k != exr) && (min > matrix.at(k).at(nCol)))
			min = matrix.at(k).at(nCol);
	return min == INFINITY ? 0 : min;
}

void CTSPSolver::subRow(tMatrix &matrix, int nRow, double val)
{
	for (int k = 0; k < nCities; k++)
		if (k != nRow)
			matrix[nRow][k] -= val;
}

void CTSPSolver::subCol(tMatrix &matrix, int nCol, double val)
{
	for (int k = 0; k < nCities; k++)
		if (k != nCol)
			matrix[k][nCol] -= val;
}

double CTSPSolver::align(tMatrix &matrix)
{
double r = 0;
double min;
	for (int k = 0; k < nCities; k++) {
		min = findMinInRow(k,matrix);
		if (min > 0) {
			r += min;
			subRow(matrix,k,min);
		}
	}
	for (int k = 0; k < nCities; k++) {
		min = findMinInCol(k,matrix);
		if (min > 0) {
			r += min;
			subCol(matrix,k,min);
		}
	}
	return r;
}

bool CTSPSolver::findCandidate(tMatrix matrix, int &nRow, int &nCol)
{
	nRow = -1;
	nCol = -1;
bool alts = false;
double h = -1;
double sum;
	for (int r = 0; r < nCities; r++)
		for (int c = 0; c < nCities; c++)
//			if ((matrix.at(r).at(c) == 0) && !forbidden.values(r).contains(c)) {
			if (matrix.at(r).at(c) == 0) {
				sum = findMinInRow(r,matrix,c) + findMinInCol(c,matrix,r);
				if (sum > h) {
					h = sum;
					nRow = r;
					nCol = c;
					alts = false;
				} else if ((sum == h) && !hasSubCycles(r,c))
					alts = true;
			}
	return alts;
}

bool CTSPSolver::hasSubCycles(int nRow, int nCol)
{
	if ((nRow < 0) || (nCol < 0) || route.isEmpty() || !(route.size() < nCities - 1) || !route.contains(nCol))
		return false;
int i = nCol;
	while (true) {
		if ((i = route[i]) == nRow)
			return true;
		if (!route.contains(i))
			return false;
	}
	return false;
}

// TODO: Comment the algorithm
sStep *CTSPSolver::solve(int numCities, tMatrix task, QWidget *parent)
{
	if (numCities <= 1)
		return NULL;
	cleanup();
	nCities = numCities;
QProgressDialog pd(parent);
QProgressBar *pb = new QProgressBar(&pd);
	pb->setAlignment(Qt::AlignCenter);
	pb->setFormat(trUtf8("%v of %m parts found"));
	pd.setBar(pb);
	pd.setMaximum(nCities);
	pd.setMinimumDuration(1000);
	pd.setLabelText(trUtf8("Calculating optimal route..."));
	pd.setWindowTitle(trUtf8("Solution Progress"));
	pd.setWindowModality(Qt::ApplicationModal);
	pd.setValue(0);

sStep *step = new sStep();
	step->matrix = task;
	step->price = align(step->matrix);
	root = step;

sStep *left, *right;
int nRow, nCol;
bool firstStep = true;
double check;
	while (this->route.size() < nCities) {
//		forbidden.clear();
		step->alts = findCandidate(step->matrix,nRow,nCol);
		while (hasSubCycles(nRow,nCol)) {
//			forbidden[nRow] = nCol;
			step->matrix[nRow][nCol] = INFINITY;
			step->price += align(step->matrix);
			step->alts = findCandidate(step->matrix,nRow,nCol);
		}
		if ((nRow == -1) || (nCol == -1) || pd.wasCanceled()) {
			root = NULL;
			break;
		}

		// Route with (nRow,nCol) path
		right = new sStep();
		right->matrix = step->matrix;
		for (int k = 0; k < nCities; k++) {
			if (k != nCol)
				right->matrix[nRow][k] = INFINITY;
			if (k != nRow)
				right->matrix[k][nCol] = INFINITY;
		}
		right->price = step->price + align(right->matrix);
		// Forbid selected route to exclude its reuse in next steps.
		right->matrix[nCol][nRow] = INFINITY;
		right->matrix[nRow][nCol] = INFINITY;

		// Route without (nRow,nCol) path
		left = new sStep();
		left->matrix = step->matrix;
		left->matrix[nRow][nCol] = INFINITY;
 		left->price = step->price + align(left->matrix);

		step->candidate.nRow = nRow;
		step->candidate.nCol = nCol;
		step->plNode = left;
		step->prNode = right;

		if (right->price <= left->price) {
			// Route with (nRow,nCol) path is cheaper
			step = right;
			this->route[nRow] = nCol;
			pd.setValue(this->route.size());
			if (firstStep) {
				check = left->price;
				firstStep = false;
			}
		} else {
			// Route without (nRow,nCol) path is cheaper
			step = left;
			qApp->processEvents();
			if (firstStep) {
				check = right->price;
				firstStep = false;
			}
		}
	}

	if (!root && !pd.wasCanceled()) {
		pd.reset();
		QMessageBox(QMessageBox::Warning,trUtf8("Solution Result"),trUtf8("Unable to find solution.\nMaybe, this task has no solutions."),QMessageBox::Ok,parent).exec();
	}

	qApp->processEvents();

	if (root) {
		route = this->route;
		mayNotBeOptimal = (check < step->price);
	}
	return root;
}

QString CTSPSolver::getSortedPath() const
{
	if (!root || route.isEmpty() || (route.size() != nCities))
		return QString();

int i = 0; // We start from City 1
QString path = trUtf8("City %1").arg(1) + " -> ";
	while ((i = route[i]) != 0) {
		path += trUtf8("City %1").arg(i + 1) + " -> ";
	}
	// And finish in City 1, too
	path += trUtf8("City %1").arg(1);

	return path;
}

bool CTSPSolver::isOptimal() const
{
	return !mayNotBeOptimal;
}
