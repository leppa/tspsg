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

#include "tspsolver.h"
#include "tspmodel.h"

CTSPSolver::CTSPSolver()
{
}

double CTSPSolver::findMinInRow(int nRow, tMatrix matrix)
{
double min = INFINITY;
	for (int k = 0; k < nCities; k++)
		if (min > matrix[nRow][k])
			min = matrix[nRow][k];
	return min == INFINITY ? 0 : min;
}

double CTSPSolver::findMinInCol(int nCol, tMatrix matrix)
{
double min = INFINITY;
	for (int k = 0; k < nCities; k++)
		if (min > matrix[k][nCol])
			min = matrix[k][nCol];
	return min == INFINITY ? 0 : min;
}

sStep *CTSPSolver::solve(int numCities, tMatrix task)
{
	if (numCities <= 1)
		return NULL;
	nCities = numCities;
sStep *step = new sStep();
	step->matrix = task;
	root = step;

	return step;
}
