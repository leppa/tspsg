/*!
 * \file tspmodel.h
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
 *
 * \brief Defines CTSPModel class.
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

#ifndef TSPMODEL_H
#define TSPMODEL_H

#include "globals.h"

#include <QAbstractTableModel>
#include <QDataStream>
#include <QVector>

/*!
 * \brief This class implements table model for manipulating a task.
 * \author Copyright &copy; 2007-2014 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 */
class CTSPModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    CTSPModel(QObject *parent = 0);
    void clear();
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool loadTask(const QString &fname);
    quint16 numCities() const;
    void randomize();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool saveTask(const QString &fname);
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void setNumCities(int n);

signals:
    /*!
     * \brief This signal is emitted whenever the number of cities in the task changes.
     *
     * \sa setNumCities()
     */
    void numCitiesChanged(int);

private:
    QSettings *settings;
    QVector<QVector<double> > table;
    quint16 nCities;
    bool loadError(QDataStream::Status);
    bool loadTSPT(QDataStream *);
    bool loadZKT(QDataStream *);
    double rand(int, int) const;
};

#endif // TSPMODEL_H
