/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
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

#include "tspsolver.h"

#include <QCoreApplication>

#ifdef DEBUG
#   include <QDebug>
#endif

//! \internal \brief A short for maximum double, used internally in the solution algorithm.
#define MAX_DOUBLE std::numeric_limits<double>::max()

namespace TSPSolver {

/*!
 * \brief Returns CTSPSolver's version ID.
 * \return A string: <b>\$Id: $Format:%h %ai %an$ $</b>.
 */
QString CTSPSolver::getVersionId()
{
    return QString("$Id: $Format:%h %ai %an$ $");
}

/*!
 * \brief Constructs CTSPSolver object.
 * \param parent A parent object.
 */
CTSPSolver::CTSPSolver(QObject *parent)
    : QObject(parent), cc(true), nCities(0), total(0), root(NULL) {}

/*!
 * \brief Cleans up the object and frees up memory used by the solution tree.
 * \param processEvents If set to \c true then \link QCoreApplication::processEvents() QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents)\endlink will be called from time to time while cleaning up.
 * \warning After call to this function a solution tree returned by the solve() function is no longer valid.
 * \note It is not required to call this function manually. This function is always called by solve() at the beginning of the solution process.
 *
 * \sa solve(), setCleanupOnCancel()
 */
void CTSPSolver::cleanup(bool processEvents)
{
    route.clear();
    mayNotBeOptimal = false;
    if (root != NULL)
        deleteTree(root, processEvents);
}

/*!
 * \brief Returns the sorted optimal path, starting from City 1.
 * \param city A string that represents city elements in the path.
 * \param separator A string that represents separators between cities in the path.
 * \return A string, containing sorted optimal path.
 *
 *  The resulting path will be in the form \a city+\a separator+\a city+...+\a separator+\a city.
 *  \c \%1 in \a city will be replaced by the city number.
 */
QString CTSPSolver::getSortedPath(const QString &city, const QString &separator) const
{
    if (!root || route.isEmpty() || (route.size() != nCities))
        return QString();

int i = 0; // We start from City 1
QStringList path;
    path << city.arg(1);
    while ((i = route[i]) != 0) {
        path << city.arg(i + 1);
    }
    // And finish in City 1, too
    path << city.arg(1);

    return path.join(separator);
}

/*!
 * \brief Returns a total number of steps in the current solution.
 * \return A total number of steps or \c 0 if no solution.
 * \note This is not always the same as the number of cities.
 */
int CTSPSolver::getTotalSteps() const
{
    return total;
}

/*!
 * \brief Indicates whether or not the solution is definitely optimal.
 * \return \c true if the solution is definitely optimal, otherwise \c false.
 *
 *  The solution may need some further iterations to determine whether or not it is optimal.
 *  In such cases this function returns \c false.
 */
bool CTSPSolver::isOptimal() const
{
    return !mayNotBeOptimal;
}

/*!
 * \brief Sets whether or not to call cleanup() on solution cancel.
 * \param enable Set to \c true to enable clenup (default).
 *
 *  This may be useful if you want to make cleanup yourself or provide indication of clenup to user.
 *
 * \note Please, note that cleanup() is explicitly called at the start of each solution.
 *       Disabling cleanup and forgetting to do it manually may considerably increase the solution time for large tasks (with more than 15 cities).
 * \sa cleanup()
 */
void CTSPSolver::setCleanupOnCancel(bool enable)
{
    cc = enable;
}

/*!
 * \brief Solves the given task.
 * \param numCities Number of cities in the task.
 * \param task The matrix of city-to-city travel costs.
 * \return Pointer to the root of the solution tree.
 *
 * \todo TODO: Comment the algorithm.
 */
SStep *CTSPSolver::solve(int numCities, const TMatrix &task)
{
    if (numCities < 3)
        return NULL;

QMutexLocker locker(&mutex);
    cleanup();
    canceled = false;
    locker.unlock();

    nCities = numCities;

SStep *step = new SStep();
    step->matrix = task;
    // We need to distinguish the values forbidden by the user
    // from the values forbidden by the algorithm.
    // So we replace user's infinities by the maximum available double value.
    normalize(step->matrix);
#ifdef DEBUG
    qDebug() << step->matrix;
#endif // DEBUG
    step->price = align(step->matrix);
    root = step;

SStep *left, *right;
int nRow, nCol;
bool firstStep = true;
double check = INFINITY;
    total = 0;
    while (route.size() < nCities) {
        step->alts = findCandidate(step->matrix,nRow,nCol);

        while (hasSubCycles(nRow,nCol)) {
#ifdef DEBUG
            qDebug() << "Forbidden: (" << nRow << ";" << nCol << ")";
#endif // DEBUG
            step->matrix[nRow][nCol] = INFINITY;
            step->price += align(step->matrix);
            step->alts = findCandidate(step->matrix,nRow,nCol);
        }

#ifdef DEBUG
        qDebug() /*<< step->matrix*/ << "Selected: (" << nRow << ";" << nCol << ")";
        qDebug() << "Alternate:" << step->alts;
        qDebug() << "Step price:" << step->price << endl;
#endif // DEBUG

        locker.relock();
        if ((nRow == -1) || (nCol == -1) || canceled) {
            if (canceled && cc)
                cleanup();
            return NULL;
        }
        locker.unlock();

        // Route with (nRow,nCol) path
        right = new SStep();
        right->pNode = step;
        right->matrix = step->matrix;
        for (int k = 0; k < nCities; k++) {
            if (k != nCol)
                right->matrix[nRow][k] = INFINITY;
            if (k != nRow)
                right->matrix[k][nCol] = INFINITY;
        }
        right->price = step->price + align(right->matrix);
        // Forbid the selected route to exclude its reuse in next steps.
        right->matrix[nCol][nRow] = INFINITY;
        right->matrix[nRow][nCol] = INFINITY;

        // Route without (nRow,nCol) path
        left = new SStep();
        left->pNode = step;
        left->matrix = step->matrix;
        left->matrix[nRow][nCol] = INFINITY;
        left->price = step->price + align(left->matrix);

        step->candidate.nRow = nRow;
        step->candidate.nCol = nCol;
        step->plNode = left;
        step->prNode = right;

        // This matrix is not used anymore. Restoring infinities back.
        denormalize(step->matrix);

        if (right->price <= left->price) {
            // Route with (nRow,nCol) path is cheaper
            step->next = SStep::RightBranch;
            step = right;
            route[nRow] = nCol;
            emit routePartFound(route.size());
            if (firstStep) {
                check = left->price;
                firstStep = false;
            }
        } else {
            // Route without (nRow,nCol) path is cheaper
            step->next = SStep::LeftBranch;
            step = left;
            QCoreApplication::processEvents();
            if (firstStep) {
                check = right->price;
                firstStep = false;
            }
        }
        total++;
    }

    mayNotBeOptimal = (check < step->price);

    return root;
}

/*!
 * \brief Indicates whether or not the solution process was canceled.
 * \return \c true if the solution process was canceled, otherwise \c false.
 */
bool CTSPSolver::wasCanceled() const
{
QMutexLocker locker(&mutex);
    return canceled;
}

/*!
 * \brief Cancels the solution process.
 */
void CTSPSolver::cancel()
{
QMutexLocker locker(&mutex);
    canceled = true;
}

CTSPSolver::~CTSPSolver()
{
    if (root != NULL)
        deleteTree(root);
}

/* Privates **********************************************************/

double CTSPSolver::align(TMatrix &matrix)
{
double r = 0;
double min;
    for (int k = 0; k < nCities; k++) {
        min = findMinInRow(k,matrix);
        if (min > 0) {
            r += min;
            if (min < MAX_DOUBLE)
                subRow(matrix,k,min);
        }
    }
    for (int k = 0; k < nCities; k++) {
        min = findMinInCol(k,matrix);
        if (min > 0) {
            r += min;
            if (min < MAX_DOUBLE)
                subCol(matrix,k,min);
        }
    }
    return (r != MAX_DOUBLE) ? r : INFINITY;
}

void CTSPSolver::deleteTree(SStep *&root, bool processEvents)
{
    if (root == NULL)
        return;
SStep *step = root;
SStep *parent;
    forever {
        if (processEvents)
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if (step->plNode != NULL) {
            // We have left child node - going inside it
            step = step->plNode;
            step->pNode->plNode = NULL;
            continue;
        } else if (step->prNode != NULL) {
            // We have right child node - going inside it
            step = step->prNode;
            step->pNode->prNode = NULL;
            continue;
        } else {
            // We have no child nodes. Deleting the current one.
            parent = step->pNode;
            delete step;
            if (parent != NULL) {
                // Going back to the parent node.
                step = parent;
            } else {
                // We came back to the root node. Finishing.
                root = NULL;
                break;
            }
        }
    }
}

void CTSPSolver::denormalize(TMatrix &matrix) const
{
    for (int r = 0; r < nCities; r++)
        for (int c = 0; c < nCities; c++)
            if ((r != c) && (matrix.at(r).at(c) == MAX_DOUBLE))
                matrix[r][c] = INFINITY;
}

QList<SStep::SCandidate> CTSPSolver::findCandidate(const TMatrix &matrix, int &nRow, int &nCol) const
{
    nRow = -1;
    nCol = -1;
QList<SStep::SCandidate> alts;
SStep::SCandidate cand;
double h = -1;
double sum;
    for (int r = 0; r < nCities; r++)
        for (int c = 0; c < nCities; c++)
            if (matrix.at(r).at(c) == 0) {
                sum = findMinInRow(r,matrix,c) + findMinInCol(c,matrix,r);
                if (sum > h) {
                    h = sum;
                    nRow = r;
                    nCol = c;
                    alts.clear();
                } else if ((sum == h) && !hasSubCycles(r,c)) {
                    cand.nRow = r;
                    cand.nCol = c;
                    alts.append(cand);
                }
            }
    return alts;
}

double CTSPSolver::findMinInCol(int nCol, const TMatrix &matrix, int exr) const
{
double min = INFINITY;
    for (int k = 0; k < nCities; k++)
        if ((k != exr) && (min > matrix.at(k).at(nCol)))
            min = matrix.at(k).at(nCol);
    return (min == INFINITY) ? 0 : min;
}

double CTSPSolver::findMinInRow(int nRow, const TMatrix &matrix, int exc) const
{
double min = INFINITY;
    for (int k = 0; k < nCities; k++) {
        if (((k != exc)) && (min > matrix.at(nRow).at(k)))
            min = matrix.at(nRow).at(k);
    }
    return (min == INFINITY) ? 0 : min;
}

bool CTSPSolver::hasSubCycles(int nRow, int nCol) const
{
    if ((nRow < 0) || (nCol < 0) || route.isEmpty() || !(route.size() < nCities - 1) || !route.contains(nCol))
        return false;
int i = nCol;
    forever {
        if ((i = route.value(i)) == nRow)
            return true;
        if (!route.contains(i))
            return false;
    }
    return false;
}

void CTSPSolver::normalize(TMatrix &matrix) const
{
    for (int r = 0; r < nCities; r++)
        for (int c = 0; c < nCities; c++)
            if ((r != c) && (matrix.at(r).at(c) == INFINITY))
                matrix[r][c] = MAX_DOUBLE;
}

void CTSPSolver::subCol(TMatrix &matrix, int nCol, double val)
{
    for (int k = 0; k < nCities; k++)
        if (k != nCol)
            matrix[k][nCol] -= val;
}

void CTSPSolver::subRow(TMatrix &matrix, int nRow, double val)
{
    for (int k = 0; k < nCities; k++)
        if (k != nRow)
            matrix[nRow][k] -= val;
}

}

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const TSPSolver::TMatrix &matrix)
{
    for (int r = 0; r < matrix.count(); r++) {
        for (int c = 0; c < matrix.at(r).count(); c++)
            dbg.space() << QString::number(matrix.at(r).at(c)).leftJustified(5);
        dbg << endl;
    }
    return dbg;
}

QDebug operator<<(QDebug dbg, const TSPSolver::SStep::SCandidate &cand)
{
    dbg.nospace() << "(" << cand.nRow << ";" << cand.nCol << ")";
    return dbg;
}
#endif // DEBUG
