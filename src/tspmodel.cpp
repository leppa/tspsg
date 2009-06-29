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

quint16 CTSPModel::numCities() const
{
	return nCities;
}

void CTSPModel::setNumCities(int n)
{
// int randMin = settings->value("MinCost",DEF_RAND_MIN).toInt();
// int randMax = settings->value("MaxCost",DEF_RAND_MAX).toInt();
	if (n == nCities)
		return;
	emit layoutAboutToBeChanged();
	if (n > nCities) {
		for (int r = 0; r < nCities; r++) {
			for (int c = nCities; c < n; c++)
				if (r == c)
					table[r][c] = INFINITY;
				else
					table[r][c] = 0; // rand(randMin,randMax);
		}
		for (int r = nCities; r < n; r++) {
			for (int c = 0; c < n; c++)
				if (r == c)
					table[r][c] = INFINITY;
				else
					table[r][c] = 0; // rand(randMin,randMax);
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

void CTSPModel::loadTask(QString fname)
{
QFile f(fname);
	f.open(QIODevice::ReadOnly);
QDataStream ds(&f);
	ds.setVersion(QDataStream::Qt_4_4);
quint32 sig;
	ds >> sig;
	ds.device()->reset();
	if (sig == TSPT)
		loadTSPT(&ds);
	else if ((sig >> 16) == ZKT)
		loadZKT(&ds);
	else
		QMessageBox(QMessageBox::Critical,trUtf8("Task Load"),trUtf8("Unable to load task:\nUnknown file format or file is corrupted."),QMessageBox::Ok).exec();
	f.close();
}

void CTSPModel::loadTSPT(QDataStream *ds)
{
	// Skipping signature
	ds->skipRawData(sizeof(TSPT));
	// File version
quint8 version;
	*ds >> version;
	if (version > TSPT_VERSION) {
		QMessageBox(QMessageBox::Critical,trUtf8("Task Load"),trUtf8("Unable to load task:\nFile version is newer than application supports.\nPlease, try to update application."),QMessageBox::Ok).exec();
		return;
	}
	// Skipping metadata
	ds->skipRawData(TSPT_META_SIZE);
	// Cities number
quint16 size;
	*ds >> size;
	if (nCities != size)
		emit numCitiesChanged(size);
	// Costs
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			if (r != c)
				*ds >> table[r][c];
	emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
}

void CTSPModel::loadZKT(QDataStream *ds)
{
	// Skipping signature
	ds->skipRawData(sizeof(ZKT));
	// File version
quint16 version;
	ds->readRawData(reinterpret_cast<char *>(&version),2);
	if (version > ZKT_VERSION) {
		QMessageBox(QMessageBox::Critical,trUtf8("Task Load"),trUtf8("Unable to load task:\nFile version is newer than application supports.\nPlease, try to update application."),QMessageBox::Ok).exec();
		return;
	}
	// Cities number
quint8 size;
	ds->readRawData(reinterpret_cast<char *>(&size),1);
	if (nCities != size)
		emit numCitiesChanged(size);
	// Costs
double val;
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
			if (r != c) {
				ds->readRawData(reinterpret_cast<char *>(&val),8);
				table[r][c] = val;
			} else
				ds->skipRawData(8);
	emit dataChanged(index(0,0),index(nCities - 1,nCities - 1));
}

void CTSPModel::saveTask(QString fname)
{
QFile f(fname);
	f.open(QIODevice::WriteOnly);
QDataStream ds(&f);
	ds.setVersion(QDataStream::Qt_4_4);
	// File signature
	ds << TSPT;
	// File version
	ds << TSPT_VERSION;
	// File metadata version
	ds << TSPT_META_VERSION;
	// Metadata
	ds << OSID;
	// Number of cities
	ds << nCities;
	// Costs
	for (int r = 0; r < nCities; r++)
		for (int c = 0; c < nCities; c++)
			if (r != c)
				ds << table[r][c];
	f.close();
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
