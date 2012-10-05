/*
 *  TSPSG: TSP Solver and Generator
 *  Copyright (C) 2007-2012 Oleksii Serdiuk <contacts[at]oleksii[dot]name>
 *
 *  $Id: $Format:%h %ai %an$ $
 *  $URL: http://tspsg.info/ $
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

#include <QDateTime>
#include <QFontDatabase>
#include <QTextCodec>
#include <QTranslator>
#include "version.h"

#if QT_VERSION < QT_VERSION_CHECK(4,6,0)
#   ifdef Q_CC_MSVC
#       pragma message("WARNING: You are using Qt version < 4.6. Application will not support some non-critical features.")
#   elif defined(Q_CC_GNU)
#       warning WARNING: You are using Qt version < 4.6. Application will not support some non-critical features.
#   else
#       error You are using Qt version < 4.6. Application will not support some non-critical features. To continue, please, comment line 31 at main.cpp.
#   endif
#endif

//#ifdef STATIC_BUILD
//	#ifndef NOSVG
//		Q_IMPORT_PLUGIN(qsvgicon)
//	#endif
//#endif

int main(int argc, char *argv[])
{
#ifdef Q_OS_SYMBIAN
    // Not enough memory for solution graph generation with tasks
    // of 20 and more cities if we use non-raster graphics system.
    QApplication::setGraphicsSystem("raster");
#endif
QApplication app(argc, argv);
    app.setOverrideCursor(QCursor(Qt::WaitCursor));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
#endif
    app.setOrganizationName("Oleksii Serdiuk");
    app.setOrganizationDomain("oleksii.name");
    app.setApplicationName("TSP Solver and Generator");
    app.setApplicationVersion(BUILD_VERSION);

    // Seeding random number generator
    qsrand(QDateTime::currentDateTime().toTime_t() ^ QCursor::pos().x() ^ QCursor::pos().y());

#ifdef Q_OS_WINCE_WM
    // Qt "leaves" unpacked .ttf files after running - let's try to delete them.
QStringList files = QDir(app.applicationDirPath(), "*.ttf").entryList();
    foreach (QString file, files) {
        QFile::remove(file);
    }
#endif
    // Don't load the font if it is already available
    if (!QFontDatabase().families().contains(DEF_FONT_FACE)) {
        QFontDatabase::addApplicationFont(":/files/fonts/DejaVuLGCSansMono.ttf");
        QFontDatabase::addApplicationFont(":/files/fonts/DejaVuLGCSansMono-Bold.ttf");
    }

QTranslator en;
    if (en.load("tspsg_en", PATH_L10N))
        app.installTranslator(&en);
    else if (en.load("tspsg_en", ":/l10n"))
        app.installTranslator(&en);

MainWindow mainwindow;
#ifdef Q_OS_SYMBIAN
    //! \hack HACK: A workaround to hide Actions menu item in Symbian.
QWidgetList widgets = QApplication::allWidgets();
QWidget *w = 0;
    foreach(w, widgets) {
        w->setContextMenuPolicy(Qt::NoContextMenu);
    }
#endif

#ifdef HANDHELD
    mainwindow.showMaximized();
#ifdef Q_OS_WINCE_WM
    /*!
     * \hack HACK: For some reason showMaximized() stopped working on
     *  Windows Mobile. This workaround works all the time.
     */
    mainwindow.setWindowState(Qt::WindowMaximized);
#endif // Q_OS_WINCE_WM
#else // HANDHELD
    mainwindow.show();
#endif // HANDHELD
    app.restoreOverrideCursor();
    return app.exec();
}
