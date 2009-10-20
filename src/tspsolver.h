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

#ifndef TSPSOLVER_H
#define TSPSOLVER_H

#include "globals.h"

#include "tspmodel.h"

typedef QList<QList<double> > tMatrix;

// This structure represents one step of solving
// The tree of such elements will represent the solving process
struct sStep {
	tMatrix matrix; // Steps's matrix
	double price; // Price of travel to this step
	struct {int nRow; int nCol;} candidate; // Candiadate for branching in current matrix
	bool alts; // This matrix has alternative candidates
	sStep *plNode, *prNode; // Pointers to left and right branch steps
	sStep() { price = candidate.nRow = candidate.nCol = -1; alts = false; plNode = prNode = NULL; } // Default values
};

// TSP Solver class
class CTSPSolver
{
	Q_DECLARE_TR_FUNCTIONS(CTSPSolver)

public:
	CTSPSolver();
	QString getSortedPath() const;
	bool isOptimal() const;
	sStep *solve(int, tMatrix, QWidget *parent = 0);
	static QString getVersionId();

private:
	bool mayNotBeOptimal;
	int nCities;
	sStep *root;
	QHash<int,int> route;
//	QHash<int,int> forbidden;
	double align(tMatrix &);
	void cleanup();
	bool findCandidate(tMatrix, int &, int &);
	double findMinInRow(int, tMatrix, int exc = -1);
	double findMinInCol(int, tMatrix, int exr = -1);
	bool hasSubCycles(int, int);
	void subCol(tMatrix &, int, double);
	void subRow(tMatrix &, int, double);
};

#endif // TSPSOLVER_H
