/*!
 * \file tspsolver.h
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 *
 *  $Id$
 *  $URL$
 *
 * \brief Defines #TMatrix typedef, SCandidate and SStep structs and CTSPSolver class.
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
typedef QList<QList<double> > TMatrix;

/*!
 * \brief A structure that represents a candidate for branching.
 */
struct SCandidate {
	int nRow; //!< A zero-based row number of the candidate
	int nCol; //!< A zero-based column number of the candidate

	//! Assigns default values
	SCandidate() {
		nCol = nRow = -1;
	}
	//! An operator == implementation
	bool operator ==(const SCandidate &cand) const {
		return ((cand.nRow == nRow) && (cand.nCol == nCol));
	}
};

/*!
 * \brief This structure represents one step of solving.
 *
 *  A tree of such elements will represent the solving process.
 */
struct SStep {
	TMatrix matrix; //!< This step's matrix
	double price; //!< The price of travel to this step
	SCandidate candidate; //!< A candiadate for branching in the current matrix
	QList<SCandidate> alts; //!< A list of alternative branching candidates
	SStep *pNode; //!< Pointer to the parent step
	SStep *plNode; //!< Pointer to the left branch step
	SStep *prNode; //!< Pointer to the right branch step

	//! Assigns default values
	SStep() {
		price = -1;
		pNode = plNode = prNode = NULL;
	}
};

/*!
 * \brief This class solves Travelling Salesman Problem task.
 * \author Copyright &copy; 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
 */
class CTSPSolver: public QObject
{
	Q_OBJECT

public:
	static QString getVersionId();

	CTSPSolver(QObject *parent = NULL);
	void cleanup(bool processEvents = false);
	QString getSortedPath() const;
	bool isOptimal() const;
	SStep *solve(int numCities, const TMatrix &task);
	bool wasCanceled() const;
	~CTSPSolver();

public slots:
	void cancel();

signals:
	/*!
	 * \brief This signal is emitted once every time a part of the route is found.
	 * \param n Indicates the number of the route parts found.
	 */
	void routePartFound(int n);

private:
	bool mayNotBeOptimal, canceled;
	int nCities;
	SStep *root;
	QHash<int,int> route;
	mutable QMutex mutex;

	double align(TMatrix &matrix);
	void deleteTree(SStep *&root, bool processEvents = false);
	void denormalize(TMatrix &matrix) const;
	QList<SCandidate> findCandidate(const TMatrix &matrix, int &nRow, int &nCol) const;
	double findMinInCol(int nCol, const TMatrix &matrix, int exr = -1) const;
	double findMinInRow(int nRow, const TMatrix &matrix, int exc = -1) const;
	bool hasSubCycles(int nRow, int nCol) const;
	void normalize(TMatrix &matrix) const;
	void subCol(TMatrix &matrix, int nCol, double val);
	void subRow(TMatrix &matrix, int nRow, double val);
};

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const TMatrix &matrix);
QDebug operator<<(QDebug dbg, const SCandidate &candidate);
#endif // DEBUG

#endif // TSPSOLVER_H
