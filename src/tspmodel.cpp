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

#include "tspmodel.h"

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QtCore/qmath.h>

#include "tspsolver.h"
#include "os.h"

/*!
 * \brief Class constructor.
 * \param parent The parent of the table model.
 */
CTSPModel::CTSPModel(QObject *parent)
    : QAbstractTableModel(parent), settings(initSettings(this)), nCities(0) {}

/*!
 * \brief Resets the table, setting all its elements to 0.
 *
 * \sa randomize()
 */
void CTSPModel::clear()
{
    for (int r = 0; r < nCities; r++)
        for (int c = 0; c < nCities; c++)
            if (r != c)
                table[r][c] = 0;
    emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
}

/*!
 * \brief Returns the column count in the table.
 * \return Number of columns in the table.
 *
 *  Actually, this function returns the number of cities in the current task.
 *
 * \sa numCities(), rowCount()
 */
int CTSPModel::columnCount(const QModelIndex &) const
{
    return nCities;
}

/*!
 * \brief Returns the data stored under the given \a role for the item referred to by the \a index.
 * \param index An item index to get data from.
 * \param role The role to get data for.
 * \return Corresponding data.
 *
 * \sa setData(), headerData()
 */
QVariant CTSPModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::TextAlignmentRole)
        return int(Qt::AlignCenter);
    else if (role == Qt::FontRole) {
QFont font;
        font.setBold(true);
        return font;
    } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.row() < nCities && index.column() < nCities)
            if (table.at(index.row()).at(index.column()) == INFINITY)
                return tr(INFSTR);
            else
                //! \hack HACK: Converting to string to prevent spinbox in edit mode
                return QVariant(table.at(index.row()).at(index.column())).toString();
        else
            return QVariant();
    } else if (role == Qt::UserRole)
        return table[index.row()][index.column()];
    return QVariant();
}

/*!
 * \brief Returns the item flags for the given \a index.
 * \param index An item index to get flags from.
 * \return Corresponding item flags.
 */
Qt::ItemFlags CTSPModel::flags(const QModelIndex &index) const
{
Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.row() != index.column())
        flags |= Qt::ItemIsEditable;
    return flags;
}

/*!
 * \brief Returns the data for the given \a role and \a section in the header with the specified \a orientation.
 * \param section The section to get header data for.
 * \param orientation The orientation to get header data for.
 * \param role The role to get header data for.
 * \return Corresponding header data.
 *
 *  For horizontal headers, the section number corresponds to the column number of items shown beneath it. For vertical headers, the section number typically to the row number of items shown alongside it.
 */
QVariant CTSPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Vertical)
            return tr("City %1").arg(section + 1);
        else
            return tr("%1").arg(section + 1);
    }
    return QVariant();
}

/*!
 * \brief Loads a task from \a fname.
 * \param fname The name of the file to be loaded.
 * \return \c true on success, otherwise \c false.
 *
 * \sa saveTask()
 */
bool CTSPModel::loadTask(const QString &fname)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
QFile f(fname);
    if (!f.open(QIODevice::ReadOnly)) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), QString(tr("Unable to open task file.\nError: %1")).arg(f.errorString()));
        QApplication::restoreOverrideCursor();
        return false;
    }
QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_4);
quint32 sig;
    ds >> sig;
    if (loadError(ds.status())) {
        return false;
    }
    ds.device()->reset();
    if (sig == TSPT) {
        if (!loadTSPT(&ds)) {
            f.close();
            return false;
        }
    } else if ((sig >> 16) == ZKT) {
        if (!loadZKT(&ds)) {
            f.close();
            return false;
        }
    } else {
        f.close();
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n" + tr("Unknown file format or file is corrupted."));
        QApplication::restoreOverrideCursor();
        return false;
    }
    f.close();
    QApplication::restoreOverrideCursor();
    return true;
}

/*!
 * \brief Returns the number of cities.
 * \return Number of cities in the current task.
 *
 * \sa columnCount(), rowCount(), setNumCities()
 */
quint16 CTSPModel::numCities() const
{
    return nCities;
}

/*!
 * \brief Randomizes the table by setting all its values to random ones.
 *
 *  Uses TSPSG settings to determine random values range.
 *
 * \sa clear()
 */
void CTSPModel::randomize()
{
int randMin = settings->value("Task/RandMin", DEF_RAND_MIN).toInt();
int randMax = settings->value("Task/RandMax", DEF_RAND_MAX).toInt();
    if (settings->value("Task/SymmetricMode", DEF_SYMMETRIC_MODE).toBool()) {
        for (int r = 0; r < nCities; r++)
            for (int c = 0; c < r; c++)
                table[c][r] = table[r][c] = rand(randMin, randMax);
    } else {
        for (int r = 0; r < nCities; r++)
            for (int c = 0; c < nCities; c++)
                if (r != c)
                    table[r][c] = rand(randMin, randMax);
    }
    emit dataChanged(index(0,0), index(nCities - 1, nCities - 1));
}

/*!
 * \brief Returns the row count in the table.
 * \return Number of rows in the table.
 *
 *  Actually, this function returns the number of cities in the current task.
 *
 * \sa columnCount(), numCities()
 */
int CTSPModel::rowCount(const QModelIndex &) const
{
    return nCities;
}

/*!
 * \brief Saves current task to \a fname.
 * \param fname The name of the file to seve to.
 * \return \c true on success, otherwise \c false.
 *
 * \sa loadTask()
 */
bool CTSPModel::saveTask(const QString &fname)
{
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
QFile f(fname);
    if (!f.open(QIODevice::WriteOnly)) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), QString(tr("Unable to create task file.\nError: %1\nMaybe, file is read-only?")).arg(f.errorString()));
        f.remove();
        return false;
    }
QDataStream ds(&f);
    ds.setVersion(QDataStream::Qt_4_4);
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // File signature
    ds << TSPT;
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // File version
    ds << TSPT_VERSION;
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // File metadata version
    ds << TSPT_META_VERSION;
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // Metadata
    ds << OSID;
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // Number of cities
    ds << nCities;
    if (f.error() != QFile::NoError) {
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
        f.close();
        f.remove();
        return false;
    }
    // Costs
    for (int r = 0; r < nCities; r++)
        for (int c = 0; c < nCities; c++)
            if (r != c) {
                ds << static_cast<double>(table[r][c]); // We cast to double because double may be float on some platforms and we store double values in file
                if (f.error() != QFile::NoError) {
                    QApplication::restoreOverrideCursor();
                    QMessageBox::critical(QApplication::activeWindow(), tr("Task Save"), tr("Unable to save task.\nError: %1").arg(f.errorString()));
                    f.close();
                    f.remove();
                    return false;
                }
            }
    f.close();
    QApplication::restoreOverrideCursor();
    return true;
}

/*!
 * \brief Sets the \a role data for the item at \a index to \a value.
 * \param index The index of the item to set data at.
 * \param value The value of the item data to be set.
 * \param role The role of the item to set data for.
 * \return \c true on success, otherwise \c false.
 *
 * \sa data()
 */
bool CTSPModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::EditRole && index.row() != index.column()) {
        if ((value.toString().compare(INFSTR) == 0)
                || (value.toString().contains(QRegExp("^[^0-9]+$", Qt::CaseInsensitive)))) {
            table[index.row()][index.column()] = INFINITY;
        } else {
bool ok;
double tmp = value.toDouble(&ok);
            if (!ok || tmp < 0)
                return false;
            else {
                table[index.row()][index.column()] = tmp;
                if (settings->value("Task/SymmetricMode", DEF_SYMMETRIC_MODE).toBool())
                    table[index.column()][index.row()] = tmp;
            }
        }
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

/*!
 * \brief Sets number of cities in the current task to \a n.
 * \param n Number of cities to set to.
 *
 * \sa numCities()
 */
void CTSPModel::setNumCities(int n)
{
    if (n == nCities)
        return;
    emit layoutAboutToBeChanged();
    table.resize(n);
    for (int k = 0; k < n; k++) {
        table[k].resize(n);
    }
    if (n > nCities)
        for (int k = nCities; k < n; k++)
            table[k][k] = INFINITY;
    nCities = n;
    emit layoutChanged();
}

/* Privates **********************************************************/

inline bool CTSPModel::loadError(QDataStream::Status status)
{
QString err;
    if (status == QDataStream::Ok)
        return false;
    else if (status == QDataStream::ReadPastEnd)
        err = tr("Unexpected end of file. The file could be corrupted.");
    else if (status == QDataStream::ReadCorruptData)
        err = tr("Corrupt data read. The file could be corrupted.");
    else
        err = tr("Unknown error. The file could be corrupted.");
    QApplication::restoreOverrideCursor();
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n" + err);
    QApplication::restoreOverrideCursor();
    return true;
}

bool CTSPModel::loadTSPT(QDataStream *ds)
{
    // Skipping signature
    ds->skipRawData(sizeof(TSPT));
    if (loadError(ds->status()))
        return false;
    // File version
quint8 version;
    *ds >> version;
    if (loadError(ds->status()))
        return false;
    if (version > TSPT_VERSION) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
            + tr("File version (%1) is newer than this version of %3 supports (%2).\n"
                 "Please, try to update the application. Also, the file could be corrupted.")
                 .arg(version).arg(TSPT_VERSION).arg(QApplication::applicationName()));
        QApplication::restoreOverrideCursor();
        return false;
    }
    // Skipping metadata
    ds->skipRawData(TSPT_META_SIZE);
    if (loadError(ds->status()))
        return false;
    // Number of cities
quint16 size;
    *ds >> size;
    if (loadError(ds->status()))
        return false;
    if (size < 3) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
            + tr("Unexpected data read. The file could be corrupted."));
        QApplication::restoreOverrideCursor();
        return false;
    }
    if (size > settings->value("Tweaks/MaxNumCities", MAX_NUM_CITIES).toInt()) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        if (settings->contains("Tweaks/MaxNumCities")) {
            QMessageBox::critical(
                        QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
                        + tr("Your Tweaks/MaxNumCities setting in tspsg.ini is currently set to %1"
                             " but the task you're trying to load contains %2 cities.\n"
                             "Please, set Tweaks/MaxNumCities setting to at least %2"
                             " to be able to load this task.")
                        .arg(settings->value("Tweaks/MaxNumCities").toInt()).arg(size));
        } else {
            QMessageBox::critical(
                        QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
                        + tr("The maximum number of cities this version of %1 supports is %2"
                             " but the task you're trying to load contains %3.\n"
                             "You might be using an old version of the application or the file"
                             " could be corrupted.")
                        .arg(QApplication::applicationName()).arg(MAX_NUM_CITIES).arg(size));
        }
        QApplication::restoreOverrideCursor();
        return false;
    }
    if (nCities != size) {
        setNumCities(size);
        emit numCitiesChanged(size);
    }

double x; // We need this as double may be float on some platforms and we store double values in file
    // Travel costs
    for (int r = 0; r < size; r++)
        for (int c = 0; c < size; c++)
            if (r != c) {
                *ds >> x;
                table[r][c] = x;
                if (loadError(ds->status())) {
                    clear();
                    return false;
                }
            }
    emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
    QApplication::restoreOverrideCursor();
    return true;
}

bool CTSPModel::loadZKT(QDataStream *ds)
{
    // Skipping signature
    ds->skipRawData(sizeof(ZKT));
    if (loadError(ds->status()))
        return false;
    // File version
quint16 version;
    ds->readRawData(reinterpret_cast<char *>(&version),2);
    if (loadError(ds->status()))
        return false;
    if (version > ZKT_VERSION) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
            + tr("File version (%1) is newer than this version of %3 supports (%2).\n"
                 "Please, try to update the application. Also, the file could be corrupted.")
                 .arg(version).arg(TSPT_VERSION).arg(QApplication::applicationName()));
        QApplication::restoreOverrideCursor();
        return false;
    }
    // Number of cities
quint8 size;
    ds->readRawData(reinterpret_cast<char *>(&size),1);
    if (loadError(ds->status()))
        return false;
    if ((size < 3) || (size > 5)) {
        QApplication::restoreOverrideCursor();
        QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
        QMessageBox::critical(QApplication::activeWindow(), tr("Task Load"), tr("Unable to load task:") + "\n"
            + tr("Unexpected data read. The file could be corrupted."));
        QApplication::restoreOverrideCursor();
        return false;
    }
    if (nCities != size) {
        setNumCities(size);
        emit numCitiesChanged(size);
    }
    // Travel costs
double val;
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            if ((r != c) && (r < size) && (c < size)) {
                ds->readRawData(reinterpret_cast<char *>(&val),8);
                if (loadError(ds->status())) {
                    clear();
                    return false;
                }
                table[r][c] = val;
            } else {
                ds->skipRawData(8);
                if (loadError(ds->status())) {
                    clear();
                    return false;
                }
            }
    emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
    QApplication::restoreOverrideCursor();
    return true;
}

inline double CTSPModel::rand(int min, int max) const
{
double r;
    if (settings->value("Task/FractionalRandom", DEF_FRACTIONAL_RANDOM).toBool()) {
double x = qPow(10, settings->value("Task/FractionalAccuracy", DEF_FRACTIONAL_ACCURACY).toInt());
        r = (double)qRound((double)qrand() / RAND_MAX * (max - min) * x) / x;
    } else
        r = qRound((double)qrand() / RAND_MAX * (max - min));
    return min + r;
}
