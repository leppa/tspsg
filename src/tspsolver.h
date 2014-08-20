/*!
 * \file tspsolver.h
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief Defines TSPSolver namespace and everything needed for solving TSP tasks.
 *
 *  <b>TSPSG: TSP Solver and Generator</b>
 *
 *  This file is part of TSPSG.
 *
 *  TSPSG is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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

#include <limits>

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QStringList>

/*!
 * \def INFINITY
 * \brief This value means infinity :-)
 *
 *  Define \c INFINITY if it's not already defined.
 */
#ifndef INFINITY
#   define INFINITY std::numeric_limits<double>::infinity()
#endif

/*!
 * \brief A TSP Solver namespace.
 *
 *  This namespace contains all the stuff required for solving TSP tasks.
 */
namespace TSPSolver {

//! A matrix of city-to-city travel costs
typedef QList<QList<double> > TMatrix;

/*!
 * \brief This structure represents one step of solving.
 *
 *  A tree of such elements will represent the solving process.
 */
struct SStep {
    //! A structure that represents a candidate for branching
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

    //! An enum that describes possible selection of the next step
    enum NextStep {
        NoNextStep, //!< No next step (end of solution)
        LeftBranch, //!< Left branch was selected for the next step
        RightBranch //!< Right branch was selected for the next step
    };

    TMatrix matrix; //!< This step's matrix
    double price; //!< The price of travel to this step

    SCandidate candidate; //!< A candiadate for branching in the current step
    QList<SCandidate> alts; //!< A list of alternative branching candidates
    SStep *pNode; //!< Pointer to the parent step
    SStep *plNode; //!< Pointer to the left branch step
    SStep *prNode; //!< Pointer to the right branch step
    NextStep next; //!< Indicates what branch was selected for the next step

    //! Assigns default values
    SStep() {
        price = -1;
        pNode = plNode = prNode = NULL;
        next = NoNextStep;
    }
};

/*!
 * \brief This class solves Travelling Salesman Problem task.
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 */
class CTSPSolver: public QObject
{
    Q_OBJECT

public:
    static QString getVersionId();

    CTSPSolver(QObject *parent = NULL);
    void cleanup(bool processEvents = false);
    QString getSortedPath(const QString &city, const QString &separator = QString(" -> ")) const;
    int getTotalSteps() const;
    bool isOptimal() const;
    void setCleanupOnCancel(bool enable = true);
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
    bool mayNotBeOptimal, canceled, cc;
    int nCities, total;
    SStep *root;
    QHash<int,int> route;
    mutable QMutex mutex;

    double align(TMatrix &matrix);
    void deleteTree(SStep *&root, bool processEvents = false);
    void denormalize(TMatrix &matrix) const;
    QList<SStep::SCandidate> findCandidate(const TMatrix &matrix, int &nRow, int &nCol) const;
    double findMinInCol(int nCol, const TMatrix &matrix, int exr = -1) const;
    double findMinInRow(int nRow, const TMatrix &matrix, int exc = -1) const;
    void finishRoute();
    bool hasSubCycles(int nRow, int nCol) const;
    void normalize(TMatrix &matrix) const;
    void subCol(TMatrix &matrix, int nCol, double val);
    void subRow(TMatrix &matrix, int nRow, double val);
};

}

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const TSPSolver::TMatrix &matrix);
QDebug operator<<(QDebug dbg, const TSPSolver::SStep::SCandidate &candidate);
#endif // DEBUG

#endif // TSPSOLVER_H
