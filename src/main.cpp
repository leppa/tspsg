/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2010 Lёppa <contacts[at]oleksii[dot]name>
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

#include "mainwindow.h"
#if QT_VERSION < 0x040500
	#ifdef _MSC_VER
		#pragma message("WARNING: You are using Qt version < 4.5. Application will not support some non-critical features.")
	#elif (defined(__GNUC__) || defined(__MINGW32__))
		#warning WARNING: You are using Qt version < 4.5. Application will not support some non-critical features.
	#else
		#error You are using Qt version < 4.5. Application will not support some non-critical features. To continue, please, comment line 31 at main.cpp.
	#endif
#endif

//#ifdef STATIC_BUILD
//	Q_IMPORT_PLUGIN(qjpeg)
//	Q_IMPORT_PLUGIN(qtiff)
//#endif

int main(int argc, char *argv[])
{
QApplication app(argc, argv);
	app.setOverrideCursor(QCursor(Qt::WaitCursor));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	app.setOrganizationName("Oleksii \"Lёppa\" Serdiuk");
	app.setOrganizationDomain("oleksii.name");
	app.setApplicationName("TSPSG: TSP Solver and Generator");
	app.setApplicationVersion(BUILD_VERSION);

	// Seeding random number generator
	qsrand(QDateTime::currentDateTime().toTime_t() ^ QCursor::pos().x() ^ QCursor::pos().y());

	// Don't load the font if it is already available
	if (!QFontDatabase().families().contains(DEF_FONT_FACE))
		QFontDatabase::addApplicationFont(":/files/DejaVuLGCSansMono.ttf");

QTranslator en;
	if (en.load("tspsg_en", PATH_L10N))
		app.installTranslator(&en);
	else if (en.load("tspsg_en", ":/l10n"))
		app.installTranslator(&en);

MainWindow mainwindow;
#ifdef HANDHELD
	mainwindow.showMaximized();
#else // HANDHELD
	mainwindow.show();
#endif // HANDHELD
	app.restoreOverrideCursor();
	return app.exec();
}
