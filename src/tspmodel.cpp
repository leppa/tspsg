/*
 *  TSPSG - TSP Solver and Generator
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

#include <QtGui>
#include "tspmodel.h"

CTSPModel::CTSPModel(QObject *parent)
	: QAbstractTableModel(parent), nCities(0)
{
	settings = new QSettings(QSettings::IniFormat,QSettings::UserScope,"TSPSG","tspsg");
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

QVariant CTSPModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
		if (orientation == Qt::Vertical)
			return trUtf8("City %1").arg(section + 1);
		else
			return trUtf8("%1").arg(section + 1);
	return QVariant();
}

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
			if (table[index.row()][index.column()] == INFINITY)
				return trUtf8(INFSTR);
			else
				// HACK: Converting to string to prevent spinbox in edit mode
				return QVariant(table[index.row()][index.column()]).toString();
		else
			return QVariant();
	} else if (role == Qt::UserRole)
		return table[index.row()][index.column()];
	return QVariant();
}

bool CTSPModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;
	if (role == Qt::EditRole && index.row() != index.column()) {
		if (value.toString().compare(INFSTR) == 0)
			table[index.row()][index.column()] = INFINITY;
		else {
bool ok;
double tmp = value.toDouble(&ok);
			if (!ok || tmp < 0)
				return false;
			else
				table[index.row()][index.column()] = tmp;
		}
		emit dataChanged(index,index);
		return true;
	}
	return false;
}

Qt::ItemFlags CTSPModel::flags(const QModelIndex &index) const
{
Qt::ItemFlags flags = QAbstractItemModel::flags(index);
	if (index.row() != index.column())
		flags |= Qt::ItemIsEditable;
	return flags;
}

int CTSPModel::numCities() const
{
	return nCities;
}

void CTSPModel::setNumCities(int n)
{
int randMin = settings->value("MinCost",DEF_RAND_MIN).toInt();
int randMax = settings->value("MaxCost",DEF_RAND_MAX).toInt();
	if (n == nCities)
		return;
	emit layoutAboutToBeChanged();
	if (n > nCities) {
		for (int r = 0; r < nCities; r++) {
			for (int c = nCities; c < n; c++)
				if (r == c)
					table[r][c] = INFINITY;
				else
					table[r][c] = rand(randMin,randMax);
		}
		for (int r = nCities; r < n; r++) {
			for (int c = 0; c < n; c++)
				if (r == c)
					table[r][c] = INFINITY;
				else
					table[r][c] = rand(randMin,randMax);
		}
	}
	nCities = n;
	emit layoutChanged();
}

void CTSPModel::clear()
{
	for (int r = 0; r < nCities; r++)
		for (int c = 0; c < nCities; c++)
			if (r != c)
				table[r][c] = 0;
	emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
}

void CTSPModel::randomize()
{
int randMin = settings->value("MinCost",DEF_RAND_MIN).toInt();
int randMax = settings->value("MaxCost",DEF_RAND_MAX).toInt();
	for (int r = 0; r < nCities; r++)
		for (int c = 0; c < nCities; c++)
			if (r != c)
				table[r][c] = rand(randMin,randMax);
	emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
}
