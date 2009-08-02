/*
 *  TSPSG: TSP Solver and Generator
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
	void clear();
	int columnCount(const QModelIndex &) const;
	QVariant data(const QModelIndex &, int) const;
	Qt::ItemFlags flags(const QModelIndex &) const;
	QVariant headerData(int, Qt::Orientation, int) const;
	bool loadTask(QString);
	quint16 numCities() const;
	void randomize();
	int rowCount(const QModelIndex &) const;
	bool saveTask(QString);
	bool setData(const QModelIndex &, const QVariant &, int);
	void setNumCities(int);

signals:
	void numCitiesChanged(int);

private:
	QSettings *settings;
	QVector<QVector<double> > table;
	quint16 nCities;
	bool loadError(QDataStream::Status);
	bool loadZKT(QDataStream *);
	bool loadTSPT(QDataStream *);
	int rand(int, int) const;
};

#endif // TSPMODEL_H
