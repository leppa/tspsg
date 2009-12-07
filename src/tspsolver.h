/*!
 * \file tspsolver.h
 * \author Copyright &copy; 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Defines #tMatrix typedef, sStep struct and CTSPSolver class.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
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

//! A matrix of city-to-city travel costs.
typedef QList<QList<double> > tMatrix;

/*!
 * \brief This structure represents one step of solving.
 *
 *  A tree of such elements will represent the solving process.
 */
//! \todo TODO: List alternative candidates.
struct sStep {
	tMatrix matrix; //!< This step's matrix
	double price; //!< The price of travel to this step
	struct {
		int nRow; //!< A zero-based row number of the candidate
		int nCol; //!< A zero-based column number of the candidate
	} candidate; //!< A candiadate for branching in the current matrix
	bool alts; //!< Indicates whether or not matrix has alternative candidates
	sStep *plNode; //!< Pointer to the left branch step
	sStep *prNode; //!< Pointer to the right branch step

	//! Assigns default values
	sStep() {
		price = candidate.nRow = candidate.nCol = -1;
		alts = false;
		plNode = prNode = NULL;
	}
};

/*!
 * \brief This class solves Travelling Salesman Problem task.
 * \author Copyright &copy; 2007-2009 Lёppa <contacts[at]oleksii[dot]name>
 *
 * \todo TODO: Deletion of solution tree on destroy and cleanup.
 */
class CTSPSolver
{
	Q_DECLARE_TR_FUNCTIONS(CTSPSolver)

public:
	CTSPSolver();
	QString getSortedPath() const;
	static QString getVersionId();
	bool isOptimal() const;
	sStep *solve(int numCities, tMatrix task, QWidget *parent = 0);

private:
	bool mayNotBeOptimal;
	int nCities;
	sStep *root;
	QHash<int,int> route;
//	QHash<int,int> forbidden;

	double align(tMatrix &matrix);
	void cleanup();
	bool findCandidate(const tMatrix &matrix, int &nRow, int &nCol) const;
	double findMinInCol(int nCol, const tMatrix &matrix, int exr = -1) const;
	double findMinInRow(int nRow, const tMatrix &matrix, int exc = -1) const;
	bool hasSubCycles(int nRow, int nCol) const;
	void subCol(tMatrix &matrix, int nCol, double val);
	void subRow(tMatrix &matrix, int nRow, double val);
};

#endif // TSPSOLVER_H
