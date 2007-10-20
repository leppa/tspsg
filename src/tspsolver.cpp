/*
 *  TSPSG - TSP Solver and Generator
 *  Copyright (C) 2007 L¸ppa <lacontacts[at]gmail[dot]com>
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

// Temoporary matrix for testing algorithm
double testmatrix[] = {
		infinity, 6, 7, 3, 4,
		9, infinity, 6, 9, 10,
		6, 9, infinity, 5, 3,
		3, 10, 4, infinity, 2,
		5, 1, 1, 9, infinity
	};


CTSPSolver::CTSPSolver()
{
}

sStep *CTSPSolver::solve(int numCities, double *task)
{
	if (numCities <= 1)
		return NULL;
// Temporary debug code :-)
	task = &testmatrix[0];
	numCities = 5;
//*/
sStep step;
	step.matrix = new double(numCities * numCities);
	memcpy(step.matrix,task,sizeof(double) * numCities * numCities);

	return NULL;
}

