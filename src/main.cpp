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

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
	app.setOrganizationName("..::Lёppsville::..");
	app.setOrganizationDomain("www.leppsville.com");
	app.setApplicationName("TSPSG");
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
MainWindow mainwindow;
	mainwindow.show();
	return app.exec();
}
