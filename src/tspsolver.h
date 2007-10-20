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

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include <QtCore>

const double infinity = 1.7E+308;

// Structure represent one step of solving
// The tree of such elements will represent the solving process
struct sStep {
	double *matrix;
	double price;
	struct {unsigned int x; unsigned int y;} pos;
	sStep *pLeft, *pRight;
	sStep() { matrix = NULL; price = pos.x = pos.y = 0; pLeft = pRight = NULL;}
};

// TSP Solver class
class CTSPSolver
{
public:
	CTSPSolver();
	sStep *solve(int, double *);
};

#endif // TSPSOLVER_H

