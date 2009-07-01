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

#ifndef TSPMODEL_H
#define TSPMODEL_H

#include "globals.h"

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
	quint16 numCities() const;
	void setNumCities(int);
	void clear();
	void loadTask(QString);
	void saveTask(QString);
	void randomize();
signals:
	void numCitiesChanged(int);
private:
	QSettings *settings;
	double table[MAX_CITIES][MAX_CITIES];
	quint16 nCities;
	int rand(int, int) const;
	bool loadError(QDataStream::Status) const;
	void loadZKT(QDataStream *);
	void loadTSPT(QDataStream *);
};

#endif // TSPMODEL_H
