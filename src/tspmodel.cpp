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

#include "tspmodel.h"
#include <QTGui>

CTSPModel::CTSPModel(QObject *parent)
	: QAbstractTableModel(parent), randMin(1), randMax(10)
{
}

int CTSPModel::rand(int min, int max)
{
	return min + (int)(((float)qrand() / RAND_MAX) * max);
}

int CTSPModel::rowCount(const QModelIndex &) const
{
	return nCities;
}

int CTSPModel::columnCount(const QModelIndex &) const
{
	return nCities;
}

QVariant CTSPModel::headerData(int section, Qt::Orientation, int role) const
{
	if (role == Qt::DisplayRole)
		return trUtf8("Ãîðîä %1").arg(section + 1);
	return QVariant();
}

QVariant CTSPModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::TextAlignmentRole)
		return int(Qt::AlignCenter);
	else if (role == Qt::DisplayRole) {
		if (index.row() == index.column())
			return trUtf8("---");
		if (index.row() < nCities && index.column() < nCities)
			return table.at(index.row())->at(index.column());
		else
			return QVariant();
	}
	return QVariant();
}

bool CTSPModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	return true;
}

Qt::ItemFlags CTSPModel::flags(const QModelIndex &index) const
{
Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.row() == index.column())
		flags |= Qt::ItemIsEditable;
	return flags;
}

int CTSPModel::numCities() const
{
	return nCities;
}

void CTSPModel::setNumCities(int n)
{
	if (n > nCities) {
		foreach(QList<double> *row,table) {
			for (int k = nCities; k < n; k++)
				row->append(rand(randMin,randMax));
		}
		for (int k = nCities; k < n; k++) {
QList<double> *row = new QList<double>[n];
			foreach(double cell,*row)
				cell = rand(randMin,randMax);
			table.append(row);
		}
	} else if (n < nCities) {
		// TODO: Shrinking table
	}
	nCities = n;
}

