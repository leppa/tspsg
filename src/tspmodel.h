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

#ifndef TSPMODEL_H
#define TSPMODEL_H

#include <QAbstractTableModel>

class CTSPModel: public QAbstractTableModel
{
	Q_OBJECT
public:
	CTSPModel(QObject *parent = 0);
	int rowCount(const QModelIndex &) const;
	int columnCount(const QModelIndex &) const;
	QVariant headerData(int, Qt::Orientation, int) const;
	QVariant data(const QModelIndex &, int) const;
	bool setData(const QModelIndex &, const QVariant &, int);
	Qt::ItemFlags flags(const QModelIndex &) const;
	int numCities() const;
	void setNumCities(int);
	int randMin;
	int randMax;
private:
	QList<QList<double> *> table;
	int nCities;
	int rand(int, int);
};

#endif // TSPMODEL_H

