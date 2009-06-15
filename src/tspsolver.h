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

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <QtCore>

typedef QList<double *> tMatrix;

// This structure represents one step of solving
// The tree of such elements will represent the solving process
struct sStep {
	tMatrix matrix;
	double price;
	struct {unsigned int x; unsigned int y;} pos;
	sStep *plNode, *prNode;
	sStep() { price = pos.x = pos.y = 0; plNode = prNode = NULL; }
};

// TSP Solver class
class CTSPSolver
{
public:
	CTSPSolver();
	sStep *solve(int, tMatrix);
private:
	int nCities;
	sStep *root;
	double findMinInRow(int, tMatrix);
	double findMinInCol(int, tMatrix);
};

#endif // TSPSOLVER_H
